#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "string/string.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "disk/streamer.h"
uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

extern void problem();
extern void enable_int();
uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
    //notice the "reverse" order because of endiness;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}
void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    //notice that making it a vector of a 16 bits makes things easier when addressing
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}




void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}
static struct Paging_4GB_Chunk * kernel_chunk =0;

void kernel_main()
{
    terminal_initialize();
    print("Hello world!\ntest\n");

    kheap_init();

    disk_search_and_init();
    // Initialize the interrupt descriptor table
    idt_init();


    //commenting for now to test malloc
    
    //set interrupt desc table
    //print("Hello world2",2,0);

    //problem();

    


    /* sample allocation test, set the breakpoints in gdb to see
    void* ptr = kmalloc(50);
    void* ptr2 = kmalloc(5000);
    void* ptr3 = kmalloc(4000);
    kfree(ptr);
    void* ptr4 = kmalloc(50);

    if (ptr || ptr2 || ptr3 || ptr4){
    //dumb skip warning        
    }
    print("success ");

    */

    //paging
     kernel_chunk = paging_new_4gb( PAGING_ACCESS_ALL_ACCESS_LEVEL |PAGING_IS_PRESENT | PAGING_IS_WRITABLE);
     paging_switch( paging_4g_chunk_get_directory(kernel_chunk));
   
   
    
    enable_paging();
    char buf[512];
    disk_search_and_init();
    disk_read_block(disk_get(0),0,1,buf);

     //outb(0x60, 0xff);

    /*
      char * page = kzalloc(4096);
      //set this recent allocated page to be the one for address 0x1000
    paging_set(paging_4g_chunk_get_directory(kernel_chunk),(void *)0x1000, (uint32_t)page | PAGING_ACCESS_ALL_ACCESS_LEVEL| PAGING_IS_PRESENT | PAGING_IS_WRITABLE);
   
    char * modifypage = (char *) 0x1000;
    modifypage[0] = 'A';
    modifypage[1] = 'B';
    print(modifypage);
    print(page);
    will print the same content in both, since they are the same real memory address
    */
     
    enable_int();

    struct path_root*  root_path  = pathparser_parse("0:/bin/shell.exe",NULL);
    
    int driveNo=5;
    const char *  pathPart;
    if (root_path){
        driveNo = root_path->drive_no;
        pathPart = root_path->first->part;

        if (driveNo!=0){
            print("drive no is not zero");
        }
        print("\n");
        print("/");
        print("\n");
        print(pathPart);
        print("\n");
        print("/");
        print("\n");



        const char * pathPart2 = root_path->first->next->part;

        print(pathPart2);
        print("\n");

        print("no error");

        


    }

    if (driveNo && pathPart){

    }

    struct disk_stream * stream = diskstream_new(0);
    diskstream_seek(stream,0x400);
    unsigned char * c = kmalloc(1024);
    diskstream_read(stream,&c,1024);
    //now have to concat next 1MB to disk file, then write the FileSystem somehow there
    while (1)
    {
       print("");
    }

}
 
