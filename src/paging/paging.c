#include "paging.h"
#include "memory/heap/kheap.h"

//static means not accessigle out of this file :
static uint32_t* current_directory = 0;

//here instead of  the header to not be exposed
void paging_load_directory (  uint32_t * directory);


struct Paging_4GB_Chunk * paging_new_4gb(uint8_t flags){
    uint32_t * directory  = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_PAGES_PER_DIRECTORY);
    uint32_t offset = 0;

    for (int i=0;i< PAGING_TOTAL_PAGES_PER_DIRECTORY;i++){
        
        uint32_t * entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

        for (int j=0;j<PAGING_TOTAL_ENTRIES_PER_TABLE;j++){
            entry[j] = ((offset + PAGING_PAGE_SIZE * j) | flags); //first operand is always bigger or equal 4096(PAGING_SIZE), so we have 12 bits free
        }
        offset += PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITABLE;
        //does mark all directories with writable , otherwise pages inside are not writable even if set
    }

    struct Paging_4GB_Chunk * chunk_4gb = kzalloc(sizeof(struct Paging_4GB_Chunk));
    chunk_4gb -> directory_entry = directory;  
    return chunk_4gb;

}

uint32_t * paging_4g_chunk_get_directory (struct Paging_4GB_Chunk * chunk){
    return chunk-> directory_entry;
    //is idea here about hidding the internals of the chunk. i guess it is if we just use void * outside
}

void paging_switch(uint32_t * directory){
    paging_load_directory(directory);
    current_directory = directory;
}
bool paging_is_aligned(void * address){
    return (((uint32_t) address) % PAGING_PAGE_SIZE)==0;
}

int paging_get_indexes(void * virtual_address, uint32_t* directory_index_out, uint32_t* table_index){
    if (!paging_is_aligned(virtual_address)){
       return -EINVARG;     
    }
    * directory_index_out = ((uint32_t) virtual_address)/ (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    * table_index = ((uint32_t) virtual_address) % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)/PAGING_PAGE_SIZE;
    return 0;
}
int paging_set(uint32_t* directory, void * virtual_address, uint32_t val){
    if (!paging_is_aligned(virtual_address)){
            return -EINVARG;
    }

    uint32_t directory_index = 0;
    uint32_t table_index = 0;
    int res = paging_get_indexes(virtual_address, &directory_index, &table_index);
    if (res<0) return res;
    uint32_t directoryEnty = directory[directory_index];
    uint32_t* table = (uint32_t *) (directoryEnty & 0xfffff000);
    //now we have table adress;
    table[table_index] = val;
    //setting the page entry to another that we choose, remaping 4k to something else;
    return 0;
}