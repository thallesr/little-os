FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.o  ./build/io/io.asm.o ./build/memory/heap.o ./build/memory/kheap.o ./build/paging/paging.o ./build/paging/paging.asm.o ./build/disk/disk.o ./build/fs/pparser.o ./build/string/string.o ./build/disk/streamer.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc ${FLAGS} -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o
./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin
./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/paging/paging.asm.o: ./src/paging/paging.asm
	nasm -f elf -g ./src/paging/paging.asm -o ./build/paging/paging.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc ${INCLUDES} -I./src/idt -I./src/memory -I./src/io -I./src/paging ${FLAGS} -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt/idt.o: ./src/idt/idt.c
	i686-elf-gcc ${INCLUDES} -I./src/idt ${FLAGS} -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc ${INCLUDES} -I./src/memory ${FLAGS} -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.o: ./src/io/io.c
	i686-elf-gcc ${INCLUDES} -I./src/io ${FLAGS} -std=gnu99 -c ./src/io/io.c -o ./build/io/io.o

./build/memory/heap.o: ./src/memory/heap/heap.c 
	i686-elf-gcc ${INCLUDES} -I./src/memory/heap -I./src/memory ${FLAGS} -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap.o

./build/memory/kheap.o: ./src/memory/heap/kheap.c 
	i686-elf-gcc ${INCLUDES} -I./src/memory/heap ${FLAGS} -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/kheap.o

./build/paging/paging.o: ./src/paging/paging.c
	i686-elf-gcc ${INCLUDES} -I./src/paging ${FLAGS} -std=gnu99 -c ./src/paging/paging.c -o ./build/paging/paging.o

./build/disk/disk.o: ./src/disk/disk.c
	i686-elf-gcc ${INCLUDES} -I./src/disk ${FLAGS} -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o

./build/disk/streamer.o: ./src/disk/streamer.c
	i686-elf-gcc ${INCLUDES} -I./src/disk ${FLAGS} -std=gnu99 -c ./src/disk/streamer.c -o ./build/disk/streamer.o


./build/fs/pparser.o: ./src/fs/pparser.c
	i686-elf-gcc ${INCLUDES} -I./src/fs  ${FLAGS} -std=gnu99 -c ./src/fs/pparser.c -o ./build/fs/pparser.o

./build/string/string.o: ./src/string/string.c
	i686-elf-gcc ${INCLUDES} -I./src/string  ${FLAGS} -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o

clean:

	rm build/idt/idt.asm.o
	rm build/idt/idt.o
	rm build/memory/memory.o
	rm build/io/io.o
	rm build/io/io.asm.o
	rm build/memory/heap.o
	rm build/memory/kheap.o
	rm build/paging/paging.o
	rm build/paging/paging.asm.o
	rm build/disk/disk.o
	rm build/fs/pparser.o
	rm build/string/string.o
	rm bin/kernel.bin  bin/os.bin
	rm build/kernel.asm.o build/kernelfull.o
	rm build/kernel.o
	rm build/disk/streamer.o



