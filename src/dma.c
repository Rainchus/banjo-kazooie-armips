#include "../include/main.h"

#define	PHYS_TO_K1(x)	((u32)(x)|0xA0000000)	/* physical to kseg1 */
#define	IO_READ(addr)		(*(vu32 *)PHYS_TO_K1(addr))

#define PI_BASE_REG		0x04600000

/* PI DRAM address (R/W): [23:0] starting RDRAM address */
#define PI_DRAM_ADDR_REG	(PI_BASE_REG+0x00)	/* DRAM address */

/* PI pbus (cartridge) address (R/W): [31:0] starting AD16 address */
#define PI_CART_ADDR_REG	(PI_BASE_REG+0x04)

/* PI read length (R/W): [23:0] read data length */
#define PI_RD_LEN_REG		(PI_BASE_REG+0x08)

/* PI write length (R/W): [23:0] write data length */
#define PI_WR_LEN_REG		(PI_BASE_REG+0x0C)
#define	PI_STATUS_ERROR		0x04
#define	PI_STATUS_IO_BUSY	0x02
#define	PI_STATUS_DMA_BUSY	0x01
/*
 * PI status (R): [0] DMA busy, [1] IO busy, [2], error
 *           (W): [0] reset controller (and abort current op), [1] clear intr
 */
#define PI_STATUS_REG		(PI_BASE_REG+0x10)

#define	OS_READ			0		/* device -> RDRAM */
#define	OS_WRITE		1		/* device <- RDRAM */
#define	OS_OTHERS		2		/* for Leo disk only */

s32 osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size);

u32 osPiGetStatus(void) {
    return IO_READ(PI_STATUS_REG);
}

void dma_copy(void* romAddr, void* ramAddr, u32 dmaSize) {
    osPiRawStartDma(OS_READ, (u32)romAddr, ramAddr, dmaSize);
    while(osPiGetStatus() & PI_STATUS_DMA_BUSY);
}