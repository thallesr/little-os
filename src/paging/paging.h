#ifndef PAGING_H
#define PAGING_H

#define PAGING_CACHE_DISABLE            0b00010000
#define PAGING_WRITE_THROUGH            0b00001000
#define PAGING_ACCESS_ALL_ACCESS_LEVEL  0b00000100
#define PAGING_IS_WRITABLE              0b00000010
#define PAGING_IS_PRESENT               0b00000001

#define PAGING_TOTAL_PAGES_PER_DIRECTORY 1024
#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096

#include <stddef.h>
#include <stdint.h>

struct paging_4g_chunk
{
    uint32_t * directory_entry;
};

uint32_t * paging_4g_chunk_get_directory (struct paging_4g_chunk * chunk);
void paging_switch(uint32_t * directory);
void enable_paging();
#endif