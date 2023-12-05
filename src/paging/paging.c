#include "paging.h"
#include "memory/heap/kheap.h"

//static means not accessigle out of this file :
static uint32_t* current_directory = 0;

//here instead of  the header to not be exposed
void paging_load_directory (  uint32_t * directory);


struct paging_4g_chunk * paging_new_4gb(uint8_t flags){
    uint32_t * directory  = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_PAGES_PER_DIRECTORY);
    uint32_t offset = 0;

    for (int i=0;i< PAGING_TOTAL_PAGES_PER_DIRECTORY;i++){
        
        uint32_t * entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

        for (int j=0;j<PAGING_TOTAL_ENTRIES_PER_TABLE;j++){
            entry = (uint32_t *)((offset + PAGING_PAGE_SIZE * j) | flags); //first operand is always bigger or equal 4096(PAGING_SIZE), so we have 12 bits free
        }
        offset += PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITABLE;
        //does mark all directories with writable , otherwise pages inside are not writable even if set
    }

    struct paging_4g_chunk * chunk_4gb = kzalloc(sizeof(struct paging_4g_chunk));
    chunk_4gb -> directory_entry = directory;  
    return chunk_4gb;

}

uint32_t * paging_4g_chunk_get_directory (struct paging_4g_chunk * chunk){
    return chunk-> directory_entry;
    //is idea here about hidding the internals of the chunk. i guess it is if we just use void * outside
}

void paging_switch(uint32_t * directory){
    paging_load_directory(directory);
    current_directory = directory;
}