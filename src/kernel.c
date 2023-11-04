#include "kernel.h"
extern void problem();

size_t  stringSize(char * stringContent){
    size_t i =0;
    while (stringContent[i]){
        i++;
    }
    return i;
    
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
            video_men[y * VGA_WIDTH + x ] = terminal_make_char(' ',15);
        }
    }
    

}

void write(char * content, int lineStart, int columnStart){

    uint16_t * video_men = (uint16_t *) INITIAL_VIDEO_MEMORY;
    for(int i=0;i< stringSize(content);i++){
        video_men[ lineStart * VGA_WIDTH + columnStart + i] 
            = terminal_make_char(content[i],15);
        
    }



}

void kernel_main()
{
    terminal_initialize();
    //QEMU allows me to write past what would be the vertical limit 
    write("Hello world",VGA_HEIGHT-1,0);

    idt_init();
    //set interrupt desc table
    problem();


}
