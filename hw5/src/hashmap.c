#include "utils.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MAP_KEY(base, len) (map_key_t) {.key_base = base, .key_len = len}
#define MAP_VAL(base, len) (map_val_t) {.val_base = base, .val_len = len}
#define MAP_NODE(key_arg, val_arg, tombstone_arg) (map_node_t) {.key = key_arg, .val = val_arg, .tombstone = tombstone_arg}

hashmap_t *create_map(uint32_t capacity, hash_func_f hash_function, destructor_f destroy_function) {
    hashmap_t *map = calloc(1, sizeof(hashmap_t));
    if(map == NULL) {
        return NULL;
    }
    else {
        int writeLock = pthread_mutex_init(&map->write_lock, NULL);
        if(writeLock != 0) {
            return NULL;
        }
        int fieldsLock = pthread_mutex_init(&map->fields_lock, NULL);
        if(fieldsLock != 0) {
            return NULL;
        }
        map_node_t *nodes = calloc(capacity, sizeof(map_node_t));
        if(nodes == NULL) {
            return NULL;
        }
        map->nodes = nodes;
        map->capacity = capacity;
        map->hash_function = hash_function;
        map->destroy_function = destroy_function;
        map->size = 0;
        map->num_readers = 0;
        map->invalid = false;
        return map;
    }

}

bool put(hashmap_t *self, map_key_t key, map_val_t val, bool force) {
    if(key.key_base == NULL || key.key_len == 0 || self == NULL) {
        errno = EINVAL;
        return false;
    }
    else if(val.val_base == NULL || val.val_len == 0) {
        errno = EINVAL;
        return false;
    }
    // no error
    else {
        pthread_mutex_lock(&self->write_lock);
        if(self == NULL || self->invalid == true) {
            errno = EINVAL;
            pthread_mutex_unlock(&self->write_lock);
            return false;
        }
        else if((self->size == self->capacity) && force == false) {
            errno = EINVAL;
            pthread_mutex_unlock(&self->write_lock);
            return false;
        }
        //key index
        int index = get_index(self, key);
        map_node_t *currNode = self->nodes + (index);

        //four conditions of the thing.
        if(currNode->key.key_base == NULL) {
            //locks the inserting process
            currNode->key = key;
            currNode->val = val;
            currNode->tombstone = false;
            self->size++;
        }
        else {
            //map is full and force is true
            if((self->size == self->capacity) && force == true) {
                self->destroy_function(currNode->key, currNode->val);
                currNode->key = key;
                currNode->val = val;
            }
            else if(currNode->key.key_len == key.key_len) {
                if(memcmp(currNode->key.key_base, key.key_base, key.key_len) == 0) {
                    //printf("%s\n", "HERE TRYIING TO UPDATE");
                    self->destroy_function(currNode->key, currNode->val);
                    currNode->key = key;
                    currNode->val = val;
                }
            }
            //sequential search
            int checkNodes = 0;
            while(checkNodes != self->capacity) {
                //empty slot found
                if(index == self->capacity) {
                    index = 0;
                }
                if(currNode->key.key_base == NULL) {
                    currNode->key = key;
                    currNode->val = val;
                    self->size++;
                    break;
                }
                else if(currNode->tombstone == true) {
                    if(key.key_base == NULL || key.key_len == 0) {
                        self->destroy_function(currNode->key, currNode->val);
                    }

                    currNode->key = key;
                    currNode->val = val;
                    currNode->tombstone = false;
                    self->size++;
                    break;
                }
                else {
                    index++;
                    currNode = self->nodes + index;
                }
                checkNodes++;
            }
        }
        pthread_mutex_unlock(&self->write_lock);
        return true;
    }
}

map_val_t get(hashmap_t *self, map_key_t key) {
    map_val_t result= MAP_VAL(NULL, 0);
    if(self == NULL || key.key_base == NULL) {
        errno = EINVAL;
        return result;
    }
    pthread_mutex_lock(&self->fields_lock);
    if(self->invalid == true) {
        errno = EINVAL;
        pthread_mutex_unlock(&self->fields_lock);
        return result;
    }
    // no error case
    else {
        self->num_readers++;
        if(self->num_readers == 1) {
            pthread_mutex_lock(&self->write_lock);
        }
        pthread_mutex_unlock(&self->fields_lock);
    }

    for(int i = 0; i<self->capacity; i++) {
        map_node_t *node = self->nodes + i;
        // if the node is the node we are looking for
        if(node->key.key_len == key.key_len && node->tombstone == false) {
            if(memcmp(node->key.key_base, key.key_base, key.key_len) == 0) {

                pthread_mutex_lock(&self->fields_lock);
                self->num_readers--;
                if(self->num_readers == 0) {
                    pthread_mutex_unlock(&self->write_lock);
                }      //printf("%d\n", (int)node->val.val_len);
                pthread_mutex_unlock(&self->fields_lock);
                return node->val;
            }
        }
    }

    pthread_mutex_lock(&self->fields_lock);
    self->num_readers--;
    if(self->num_readers == 0) {
        pthread_mutex_unlock(&self->write_lock);
    }
    pthread_mutex_unlock(&self->fields_lock);
    return result;
}

map_node_t delete(hashmap_t *self, map_key_t key) {
    map_key_t thekey = MAP_KEY(NULL, 0);
    map_val_t val = MAP_VAL(NULL, 0);
    map_node_t result = MAP_NODE(thekey, val, false);

    if(self == NULL || key.key_base == NULL) {
        errno = EINVAL;
        return result;
    }
    pthread_mutex_lock(&self->write_lock);
    if(self->invalid == true) {
        errno = EINVAL;
        pthread_mutex_unlock(&self->write_lock);
        return result;
    }
    // no errors
    else {
        for(int i = 0; i<self->capacity; i++) {
            map_node_t *node = self->nodes + i;
            // if the node is the node we are looking for
            if(key.key_len == node->key.key_len) {
                if(memcmp(node->key.key_base, key.key_base, node->key.key_len) == 0 && node->tombstone == false) {
                    node->tombstone = true;
                    pthread_mutex_unlock(&self->write_lock);
                    return *node;
                }
            }
        }
        pthread_mutex_unlock(&self->write_lock);
        return result;
    }

}

bool clear_map(hashmap_t *self) {
	if(self == NULL) {
        errno = EINVAL;
        return false;
    }
    pthread_mutex_lock(&self->write_lock);
    if(self->invalid == true) {
        errno = EINVAL;
        pthread_mutex_unlock(&self->write_lock);
        return false;
    }
    // no error cases
    else {
        for(int i = 0; i<self->capacity; i++) {
            map_node_t *node = self->nodes + i;
            node->tombstone = false;
            self->destroy_function(node->key, node->val);

        }
        self->size = 0;
        pthread_mutex_unlock(&self->write_lock);
        return true;
    }
}

bool invalidate_map(hashmap_t *self) {
    if(self == NULL) {
        errno = EINVAL;
        return false;
    }
    pthread_mutex_lock(&self->write_lock);
    if(self->invalid == true) {
        errno = EINVAL;
        pthread_mutex_unlock(&self->write_lock);
        return false;
    }
    //no error case
    else {
        for(int i = 0; i<self->capacity; i++) {
            map_node_t *node = self->nodes + i;
            node->tombstone = false;
            if(node->key.key_base != NULL) {
                self->destroy_function(node->key, node->val);
            }

        }
        // printf("%s\n", "233333333333333333333");
        //free the node array pointer.
        self->size = 0;
        // if(self->nodes != NULL) {
        //     free(self->nodes);
        // }
        self->invalid = true;
        pthread_mutex_unlock(&self->write_lock);
        return true;
    }
}