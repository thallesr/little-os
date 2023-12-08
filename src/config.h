#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10
//defined in kernel.asm


#define PEACHOS_TOTAL_INTERRUPTS 512



//100MB
#define PEACH_OS_HEAP_SIZE_BYTES (1024 * 1024 * 100)
//4K REMEMBER, changing this value below forces many changes, currently any allocation is 
//aligned in a page
#define PEACH_OS_HEAP_BLOCK_SIZE (1024 * 4)

#define PEACH_OS_HEAP_ADDRESS 0x01000000 // start 0x01000000 	end ???????? 	 qntity ???????? (whatever exists) 	RAM -- free for use 	More Extended memory 1

#define PEACH_OS_HEAP_TABLE_ADDRESS 0x00007E00 //from os dev start0x00007E00 	end0x0007FFFF 	480.5 KiB 	Conventional memory,  usable memory , 640 KiB RAM ("Low memory") 

#endif
