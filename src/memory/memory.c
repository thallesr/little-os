#include "memory.h"

void* memset(void* ptr, int c, size_t size)
{
    char* c_ptr = (char*) ptr;
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = (char) c;
    }
    return ptr;
    
}

int memcmp(void * s1, void * s2, int total){

    char * c1 = s1;
    char * c2 = s2;
    while( total-- > 0){

        if ( *c1++ != *c2++){
            return c1[-1] < c2[-1] ? -1 : 1;
            //read this def
            //looks like for same size string it returns -1 
            //if s1 is alphabetical first for example, 1 otherwise
        }
    }

    return 0;
}

