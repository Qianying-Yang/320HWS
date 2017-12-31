#ifndef SFMM_HELPER
#define SFMM_HELPER

#include <sfmm.h>

size_t get_padding_size(size_t size);
size_t get_alignment_size(size_t size);
void insert_free_block(sf_free_header *block_pointer, int listNum);
void *fit(size_t size, int listNum);
void put(size_t request_size, void *free_block);
int checkList(size_t size);
void remove_free_block(sf_free_header *freeHead, int listNum);
void set_head(sf_free_header *block_pointer, int allocated, int padded, size_t block_size);
void set_footer(sf_footer *block_footer, int allocated, int padded, size_t block_size, size_t request_size);
void coalese(void* ptr);

#endif
