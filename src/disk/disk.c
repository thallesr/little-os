#include "io/io.h"
int disk_read_sector(int lba, int totalSectors, void * buffer){

    outb(0x1F6,(lba >> 24) | 0xE0);
    outb(0x1F2, totalSectors);
    outb(0x1F3, (unsigned char)(lba && 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);

    unsigned short* ptr = (unsigned short*) buffer;
    for (int b = 0; b < totalSectors ;b++){
        
        //wait for buffer to be ready
        char bufferStatus = insb(0x1F7);
        while (!(bufferStatus & 0x08))
        {
            bufferStatus = insb(0x1F7);
        }
        //copy from hd to memory
        //copying 512 bytes, going 2 by 2
        for (int i = 0; i < 256 ; i++){
            *ptr = insw(0x1F0);
            ptr++;
        }
        
    }
    return 0;
}