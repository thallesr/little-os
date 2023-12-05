#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
#include "../memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;


void kheap_init(){
    //declared heap and heap table not a pointer, so they are allocated

    //i want N blocks of BLOCK_SIZE
    int total_table_entries = (PEACH_OS_HEAP_SIZE_BYTES) / (PEACH_OS_HEAP_BLOCK_SIZE);
    //cast to any pointer , there is no special treatment. some region that is empty, only control is the total which we wanted , ch
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY *) (PEACH_OS_HEAP_TABLE_ADDRESS);

    kernel_heap_table.total = total_table_entries;

    void * end = (void *) (PEACH_OS_HEAP_ADDRESS + PEACH_OS_HEAP_SIZE_BYTES);

    int res = heap_create(&kernel_heap, (void *) (PEACH_OS_HEAP_ADDRESS), end , &kernel_heap_table);
    if (res < 0){
        print("Failed to create heap\n");
    }
}

void* kmalloc(size_t size)
{
    return heap_malloc(&kernel_heap, size);
}

void kfree (void* ptr){
    heap_free(&kernel_heap, ptr);
}

void * kzalloc(size_t size){
    void * ptr = heap_malloc(&kernel_heap, size);
    if (ptr<= 0) return ptr;
    memset(ptr,0,size);
    return ptr;
}