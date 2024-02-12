.definelabel osWritebackDCache, 0x80264AF0
.definelabel osInvalDCache, 0x80264B70
.definelabel osRecvMesg, 0x80264550
.definelabel osPiStartDma, 0x802647D0
.definelabel osCreateMesgQueue, 0x802642A0
.definelabel osInvalICache, 0x80002BF0
.definelabel osWriteBackDCacheAll, 0x80263BE0
.definelabel D_803FFE10, 0x803FFE10
.definelabel func_80254008, 0x80254008
.definelabel D_8000E800, 0x8000E800
.definelabel func_802405f0, 0x802405f0
.definelabel rarezip_uncompress, 0x8023E0C4
.definelabel D_8027BF2C, 0x8027BF2C
.definelabel D_8027BF30, 0x8027BF30
.definelabel D_8002D500, 0x8002D500
.definelabel bzero, 0x800020F0
.definelabel inptr, 0x80007288
.definelabel wp, 0x8000728C
.definelabel func_800005c0, 0x800005C0
.definelabel osPiRawStartDma, 0x80002000

.headersize 0x7FFFF400
.org 0x80003140
.importobj "build/dma.o" //overwrites a leo function for the disk drive which is unused as far as i know

.org 0x80000524 //dma func is at 802405F0
//802405F0 a0 = ram addr destination, a1 = rom addr, a2 = size
LUI a0, 0x0100
ORI a0, a0, 0x0000
LI a1, PAYLOAD_START
LI a2, PAYLOAD_END
JAL dma_copy
SUBU a2, a2, a1
J hookedBootCode
NOP

.include "asm/object_files.asm"