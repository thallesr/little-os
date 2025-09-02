#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "kernel.h"

struct filesystem* filesystem[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem_slot()
{
    int i = 0;
    for ( i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystem[i] == 0)
        {
            return &filesystem[i];
        }
        /* code */
    }
    return 0;
}
void fs_insert_filesystem(struct filesystem* filesystem){
    struct filesystem** fs;
    fs = fs_get_free_filesystem_slot();
    if (!fs){
        while (1)
        {
           print("Problem getting free filesystem slot ");
        }
        
    }

    *fs = filesystem;
}

//static as in built into the kernel
static void fs_static_load(){
    //fs_insert_filesystem(fat16_init());

}
void fs_load()
{
    memset(filesystem,0,sizeof(filesystem));
    fs_static_load();
}
void fs_init(){
    memset(file_descriptors,0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor ** desc_out){

    int res = -ENOMEM;
    for(int i =0 ; i< PEACHOS_MAX_FILE_DESCRIPTORS; i++){
        if (file_descriptors[i]== 0){
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            
            //want it to start at 1
            desc->index = i+1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;
        }

    }
    return res;

}

static struct file_descriptor* file_get_descriptor(int fd){
    if ((fd <= 0) || (fd >= PEACHOS_MAX_FILE_DESCRIPTORS))
    {
        return 0;
    }

    //descriptor starts at 1
    int index = fd -1 ; 

    return file_descriptors[index];
}

struct filesystem* fs_resolve(struct disk* disk){
     struct filesystem* fs =0 ;
    for(int i =0; i< PEACHOS_MAX_FILESYSTEMS; i++){
        if (filesystem[i] !=0 && filesystem[i]-> resolve(disk) == 0){
            fs = filesystem [i];
            break;
        }
    }
    return fs;
     
}