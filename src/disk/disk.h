#ifndef DISK_H
#define DISK_H

int disk_read_sector(int lba, int totalSectors, void * buffer);

#endif