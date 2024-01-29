#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk.h"

struct disk_stream
{
    int pos;
    struct disk* disk;
};
#endif