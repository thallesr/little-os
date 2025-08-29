 #ifndef DISK_H
#define DISK_H
typedef unsigned int PEACH_OS_DISK_TYPE;

//Represents a disk in the system
#define PEACH_OS_DISK_TYPE_REAL 0

struct disk
{
    PEACH_OS_DISK_TYPE type;
    int sector_size;
};
typedef struct disk Disk;

int disk_read_block(Disk * disk, unsigned int lba, int total, void * buf);

struct disk* disk_get(int index);

void disk_search_and_init();



#endif