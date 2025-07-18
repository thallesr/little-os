#include "streamer.h"
#include "../memory/heap/kheap.h"
#include "../config.h"

struct disk_stream * diskstream_new ( int disk_id){
    struct disk * disk = disk_get(disk_id);
    if(!disk){
        return 0;
    }

    struct disk_stream * streamer = kzalloc(sizeof(struct disk_stream));
    streamer-> pos =0;
    streamer -> disk = disk;
    return streamer;

}
int diskstream_seek(struct disk_stream* stream, int pos){
    stream->pos = pos;
    return 0;
}
int diskstream_read(struct disk_stream * stream, void * out, int total){
    int sector = stream-> pos  / PEACH_OS_SECTOR_SIZE;
    int offset = stream-> pos % PEACH_OS_SECTOR_SIZE;
    char buf[PEACH_OS_SECTOR_SIZE];

    int res = disk_read_block(stream-> disk, sector, 1 , buf);
    if ( res> 0 ){
        int total_to_read = total > PEACH_OS_SECTOR_SIZE ? PEACH_OS_SECTOR_SIZE : total;
        for (int i=0; i< total_to_read; i++){
            *(char *) out ++ = buf[offset+i];
        }

        stream-> pos += total_to_read;
        if (total > PEACH_OS_SECTOR_SIZE){
                res = diskstream_read(stream, out , total - PEACH_OS_SECTOR_SIZE);
        }
    }
        return res;
};


void diskstream_close(struct disk_stream * streamer)
{
    kfree(streamer);
};
