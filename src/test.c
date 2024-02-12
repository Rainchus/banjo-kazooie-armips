#include "../include/main.h"

//extern struct huft *D_8027BF00;
// static int _rarezip_uncompress(u8 **srcPtr, u8 **dstPtr, struct huft * arg2);
extern void* D_8027BF00;
int _rarezip_uncompress(u8 **srcPtr, u8 **dstPtr, void* arg2);

void hookCode(s32* patchAddr, void* jumpLocation) {
    jumpLocation = (void*)(u32)((((u32)jumpLocation & 0x00FFFFFF) >> 2) | 0x08000000);
    patchAddr[0] = (s32)jumpLocation; //write j instruction
    patchAddr[1] = 0; //write nop
}

void rarezip_uncompress_Hook(u8 **srcPtr, u8 **dstPtr){
    //updates in and out buffer ptrs,
    _rarezip_uncompress(srcPtr, dstPtr, D_8027BF00);
}