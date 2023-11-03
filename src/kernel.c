#include "kernel.h"


void kernel_main()
{
    terminal_initialize();

}

uint16_t terminal_make_char(char character,char colour){
    return (colour<< 8) | character;
    //notice the "reverse" order because of endiness;
}


void terminal_initialize(){
    uint16_t * video_men = (uint16_t *) INITIAL_VIDEO_MEMORY;
    //notice that making it a vector of a 16 bits makes things easier when addressing
    for (int y  = 0; y < VGA_HEIGHT; y++)
    {
        for(int x = 0; x < VGA_WIDTH; x++){
            video_men[y * VGA_WIDTH + x ] = terminal_make_char('A',15);
        }
    }
    

}
