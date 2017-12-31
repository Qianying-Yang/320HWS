#include "queue.h"
#include <stdio.h>
#include <errno.h>

queue_t *create_queue(void) {
    int queueSize = sizeof(queue_t);
    queue_t *theQueue = calloc(1, queueSize);
    if(theQueue == NULL) {
        return NULL;
    }
    else {
        //sets the queue
        theQueue->front = NULL;
        theQueue->rear = NULL;
        int isInit = pthread_mutex_init(&theQueue->lock, NULL);
        if(isInit != 0) {
            return NULL;
        }
        else {
            sem_init(&theQueue->items, 0, 0);
            theQueue->invalid = false;
            return theQueue;
        }
    }
    return NULL;
}

bool invalidate_queue(queue_t *self, item_destructor_f destroy_function) {
    if(self == NULL || self->invalid == true) {
        errno = EINVAL;
        return false;
    }
    else {
        pthread_mutex_lock(&self->lock);
        queue_node_t *currNode = self->front;
        while(currNode != NULL) {
            destroy_function(currNode->item);
            queue_node_t *temp = currNode->next;
            free(currNode);
            currNode = temp;
        }
        self->front = NULL;
        self->rear = NULL;
        self->invalid = true;
        pthread_mutex_unlock(&self->lock);
        return true;
    }

}

bool enqueue(queue_t *self, void *item) {
    // is parameter valid
    if(self == NULL || item == NULL || self->invalid == true) {
        errno = EINVAL;
        return false;
    }
    //creates a new node
    queue_node_t *node = calloc(1, sizeof(queue_node_t));
    node->item = item;
    //if queue is empty
    if(self->rear == NULL){
        sem_post(&self->items);
        pthread_mutex_lock(&self->lock);
        self->rear = node;
        self->front = node;
        pthread_mutex_unlock(&self->lock);

    }
    else {
        sem_post(&self->items);
        pthread_mutex_lock(&self->lock);
        self->rear->next = node;
        self->rear = node;
        pthread_mutex_unlock(&self->lock);
    }

    return true;
}

void *dequeue(queue_t *self) {
    if(self == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    else {
        if(self->invalid == true) {
            errno = EINVAL;
            return NULL;
        }
        else {
            sem_wait(&self->items);
            pthread_mutex_lock(&self->lock);
            if(self->front == NULL) {
                pthread_mutex_unlock(&self->lock);
                return NULL;
            }
            else {
            queue_node_t *temp = self->front;
            self->front = temp->next;

            if(self->front == NULL) {
                self->rear = NULL;
            }
            void *removedItem = temp->item;
            free(temp);
            pthread_mutex_unlock(&self->lock);
            return removedItem;
            }
        }
    }
}
