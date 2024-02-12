mkdir build
mips64-elf-gcc -D_LANGUAGE_C -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -fno-builtin -fomit-frame-pointer -G0 -c src/dma.c
move dma.o build/

armips asm/main.asm
n64crc "rom/bk.mod.z64"