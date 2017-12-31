/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sfmmHelper.h>
#include <errno.h>

/**
 * You should store the heads of your free lists in these variables.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
free_list seg_free_list[4] = {
    {NULL, LIST_1_MIN, LIST_1_MAX},
    {NULL, LIST_2_MIN, LIST_2_MAX},
    {NULL, LIST_3_MIN, LIST_3_MAX},
    {NULL, LIST_4_MIN, LIST_4_MAX}
};

int sf_errno = 0;
int isFirst = 0;
int sbrk_counter = 0;

void *sf_malloc(size_t size) {
    if(size <= 0) {
        sf_errno = EINVAL;
        return NULL;
    }
    else if(size > ((PAGE_SZ * 4) -16)) {
        sf_errno = ENOMEM;
        return NULL;
    }
    else {
        if(isFirst == 0) {
            sf_sbrk();
            sbrk_counter++;

            //start of the heap.
            void *heap_start = get_heap_start();
            //end of the heap.
            void *heap_end = get_heap_end();

            //gebin sbrk block is a big free block.
            sf_free_header *free_block_head = (sf_free_header*)heap_start;
            //set header attribute.
            size_t block_size = PAGE_SZ >> 4;
            set_head(free_block_head, 0, 0, block_size);
            free_block_head->next = NULL;
            free_block_head->prev = NULL;
            //set the footer attribute
            sf_footer *free_block_footer = (sf_footer*)(heap_end - 8);
            set_footer(free_block_footer, 0, 0, block_size, size);


            size_t free_block_size = free_block_head->header.block_size << 4;
            //determine which list it goes to and inserts at the beginning of the list.
            if(free_block_size <= LIST_1_MAX) {;
                insert_free_block(free_block_head, 0);
            }
            else if(free_block_size >= LIST_2_MIN && free_block_size <= LIST_2_MAX) {
                insert_free_block(free_block_head, 1);
            }
            else if(free_block_size >= LIST_3_MIN && free_block_size <= LIST_3_MAX) {
                insert_free_block(free_block_head, 2);
            }
            else {
                insert_free_block(free_block_head, 3);
            }

            isFirst++;
        }
        size_t align_size = get_alignment_size(size);
        // the total nedded space.
        size_t total_size = align_size + 16;
        void *found_space = NULL;
        int listNum = 0;

        /*continue action until a space is found in memory or errors.*/
        while(found_space == NULL) {
            //finds a space in corresponding list.
            if(total_size >= LIST_4_MIN) {
                found_space = fit(total_size, 3);
                listNum = 3;
            }
            else if(total_size >= LIST_3_MIN) {
                found_space = fit(total_size, 2);
                listNum = 2;
            }
            else if(total_size >= LIST_2_MIN) {
                found_space = fit(total_size, 1);
                listNum = 1;
            }
            else {
                found_space = fit(total_size, 0);
                listNum = 0;
            }

            listNum = listNum;
            //a space is found to place the request size.
            if(found_space != NULL) {
                sf_free_header *found_head = (sf_free_header*)found_space;

                size_t free_block_size = found_head->header.block_size << 4;
                size_t difference = free_block_size - total_size;

                if(difference < 32) {
                    sf_footer *found_footer = (sf_footer*)(found_space + total_size - 8);
                    set_head(found_head, 1, 1, total_size >> 4);
                    found_head -> prev = NULL;
                    found_head -> next = NULL;
                    set_footer(found_footer, 1, 1, total_size >> 4, size);
                    remove_free_block(found_head, checkList(free_block_size));
                }
                else {
                    //split the list.
                    remove_free_block(found_space, checkList(free_block_size));
                    set_head(found_head, 1, 1, total_size >> 4);
                    found_head->prev = NULL;
                    found_head->next = NULL;

                    sf_footer *found_footer = (sf_footer*)(found_space + total_size - 8 );
                    set_footer(found_footer, 1, 1, total_size >> 4, size);
                    // the header and footer of the new free block.
                    sf_free_header *new_free_head = found_space + total_size;
                    sf_footer *new_footer = (sf_footer*)(found_space + free_block_size - 8);
                    set_head(new_free_head, 0, 0, difference >> 4);
                    new_free_head->next = NULL;
                    new_free_head->prev = NULL;
                    set_footer(new_footer, 0, 0, difference >> 4, size);
                    insert_free_block(new_free_head, checkList(difference));
                }
                return found_space + 8;
            }
            else {
                if(sbrk_counter < 4) {
                    //ask for new page.
                    void *page_start = sf_sbrk();
                    sbrk_counter++;

                    //get the footer of previous block.
                    sf_footer *prev_footer = (sf_footer*)(page_start - 8);
                    //if previous is a free block
                    if(prev_footer->allocated == 0) {
                        size_t block_size = (prev_footer->block_size) << 4;
                        //printf("block size from footer %d\n", (int)block_size);
                        sf_free_header *prev_block_head = (sf_free_header*)page_start - block_size;
                        remove_free_block(prev_block_head, checkList(block_size));

                        size_t new_block_size = block_size + PAGE_SZ;
                        prev_block_head->header.block_size = new_block_size >> 4;

                        sf_footer *new_footer = (sf_footer*)get_heap_end() - 8;
                        set_footer(new_footer, 0, 0, new_block_size >> 4, size);
                        insert_free_block(prev_block_head, checkList(new_block_size));
                    }
                    //if the precious block is allocated.
                    else {
                        sf_free_header *free_header = (sf_free_header*)page_start;
                        insert_free_block(free_header, checkList(PAGE_SZ));
                    }

                }
                else {
                    errno = ENOMEM;
                    return NULL;
                }
            }
        }

        return NULL;
    }
}


void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void *ptr) {
	if(ptr == NULL){
        abort();
    }
    else {
        sf_header *start_heap = (sf_header*)get_heap_start();
        sf_footer *end_heap = (sf_footer*)get_heap_end() - 8;

        sf_header *head = (sf_header*)(ptr - 8);
        size_t block_size = head->block_size << 4;

        sf_footer *footer = (sf_footer*)(ptr + block_size - 8);
        size_t request_size = footer->requested_size;

        if(head < start_heap) {
            abort();
        }
        else if(footer > end_heap) {

            abort();
        }
        else if(head->allocated != footer->allocated) {
            abort();
        }
        else if(head->padded != footer->padded) {
            abort();
        }
        else if(head->allocated == 0) {
            abort();
        }
        else if((request_size + 16 != block_size) && head->padded == 0) {
            abort();
        }
        //valid pointer.
        else {
            sf_free_header *free_head = (sf_free_header*)head;
            set_head(free_head, 0, 0, block_size >> 4);
            footer = (sf_footer*)(ptr + block_size - 16);
            set_footer(footer, 0, 0, block_size >> 4, request_size);
            insert_free_block(free_head, checkList(block_size));

            coalese(ptr);
        }
        return;
    }
}

size_t get_padding_size(size_t size) {
    if(size%16 == 0) {
        return 0;
    }
    else {
        return 16 - (size%16);
    }
}

size_t get_alignment_size(size_t size) {
    if(size%16 == 0) {
        return size;
    }
    else {
        return size + get_padding_size(size);
    }
}

int checkList(size_t size) {
    int listNum = 0;
    if(size >= LIST_4_MIN) {
        listNum = 3;
    }
    else if(size >= LIST_3_MIN) {
        listNum = 2;
    }
    else if(size >= LIST_2_MIN) {
        listNum = 1;
    }
    else {
        listNum = 0;
    }
    return listNum;
}

void insert_free_block(sf_free_header *block_pointer, int listNum) {
    if(seg_free_list[listNum].head == NULL) {
        seg_free_list[listNum].head = block_pointer;
        block_pointer->next = NULL;
        block_pointer->prev = NULL;
    }
    else {
        block_pointer->next = seg_free_list[listNum].head;
        seg_free_list[listNum].head->prev = block_pointer;
        seg_free_list[listNum].head = block_pointer;
    }
}

void *fit(size_t size, int listNum) {
    //iterator
    int i = listNum;
    size_t currSize = 0;
    //iterate through all lists.
    while(i < FREE_LIST_COUNT) {

        free_list currList = seg_free_list[i];
        if(currList.head != NULL) {
            sf_free_header *currBlock = currList.head;

            while(currBlock != NULL) {
                currSize = currBlock->header.block_size << 4;
                if(size <= currSize) {
                    return currBlock;
                }
                currBlock = currBlock->next;
            }
            i++;

        }
        else {
            i++;

        }
    }
    return NULL;
}

void remove_free_block(sf_free_header *freeHead, int listNum) {
    //free block is the first block.
    free_list currList = seg_free_list[listNum];

    if(freeHead->prev == NULL) {
        if(freeHead->next == NULL) {
            currList.head = NULL;
            freeHead->next = NULL;
            freeHead->prev = NULL;
        }
        else {
            currList.head = freeHead->next;
            currList.head->prev = NULL;
            freeHead->next = NULL;
        }
    }
    //if it's the end of the list.
    else if(freeHead->next == NULL){
        freeHead->prev->next = NULL;
        freeHead->prev = NULL;
    }
    else {
        freeHead->next->prev = freeHead->prev;
        freeHead->prev->next = freeHead->next;
        freeHead->prev = NULL;
        freeHead->next = NULL;
    }
    seg_free_list[listNum] = currList;
}

void set_head(sf_free_header *block_pointer, int allocated, int padded, size_t block_size) {
    block_pointer->header.allocated = allocated;
    block_pointer->header.padded = padded;
    block_pointer->header.block_size = block_size;
    block_pointer->header.unused = 0;
}

void set_footer(sf_footer *block_footer, int allocated, int padded, size_t block_size, size_t request_size) {
    block_footer->allocated = allocated;
    block_footer->padded = padded;
    block_footer->block_size = block_size;
    block_footer->requested_size = request_size;
}

void coalese(void* ptr) {
    sf_free_header *currBlock = (sf_free_header*)(ptr - 8);
    sf_free_header *nextBlock = currBlock->next;

    size_t currSize = currBlock->header.block_size << 4;
    size_t nextSize = nextBlock->header.block_size << 4;

    //next block is a free block.
    if(nextBlock->header.allocated == 0) {
        remove_free_block(nextBlock, checkList(nextSize));
        currBlock->header.block_size = (currSize + nextSize) >> 4;
        sf_footer *new_footer = (sf_footer*)ptr + nextSize + currSize - 8;
        set_footer(new_footer, 0, 0, (currSize+nextSize)>>4, 0);
        insert_free_block(currBlock, checkList(currBlock->header.block_size));
    }
}