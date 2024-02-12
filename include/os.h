#ifndef _OS_H_
#define _OS_H_

#include "types.h"

typedef void *	OSMesg;
typedef s32	OSPri;
typedef s32	OSId;
typedef union	{ struct { f32 f_odd; f32 f_even; } f; f64 d; }	__OSfp;

typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;

typedef struct {
	u64	at, v0, v1, a0, a1, a2, a3;
	u64	t0, t1, t2, t3, t4, t5, t6, t7;
	u64	s0, s1, s2, s3, s4, s5, s6, s7;
	u64	t8, t9,         gp, sp, s8, ra;
	u64	lo, hi;
	u32	sr, pc, cause, badvaddr, rcp;
	u32	fpcsr;
	__OSfp	 fp0,  fp2,  fp4,  fp6,  fp8, fp10, fp12, fp14;
	__OSfp	fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;

typedef struct OSThread_s {
	struct OSThread_s	*next;		/* run/mesg queue link */
	OSPri			priority;	/* run/mesg queue priority */
	struct OSThread_s	**queue;	/* queue thread is on */
	struct OSThread_s	*tlnext;	/* all threads queue link */
	u16			state;		/* OS_STATE_* */
	u16			flags;		/* flags for rmon */
	OSId			id;		/* id for debugging */
	int			fp;		/* thread has used fp unit */
	__OSThreadprofile_s     *thprof;        /* workarea for thread profiler */
	__OSThreadContext	context;	/* register/interrupt mask */
} OSThread;

typedef struct OSMesgQueue_s {
	OSThread	*mtqueue;	/* Queue to store threads blocked
					   on empty mailboxes (receive) */
	OSThread	*fullqueue;	/* Queue to store threads blocked
					   on full mailboxes (send) */
	s32		validCount;	/* Contains number of valid message */
	s32		first;		/* Points to first valid message */
	s32		msgCount;	/* Contains total # of messages */
	OSMesg		*msg;		/* Points to message buffer array */
} OSMesgQueue;

typedef struct {
        u16 		type;		/* Message type */
        u8 		pri;		/* Message priority (High or Normal) */
        u8		status;		/* Return status */
	OSMesgQueue	*retQueue;	/* Return message queue to notify I/O 
					 * completion */
} OSIoMesgHdr;

typedef struct {
	u32		errStatus;	/* error status */
        void     	*dramAddr;      /* RDRAM buffer address (DMA) */
	void		*C2Addr;	/* C2 buffer address */
	u32		sectorSize;	/* size of transfering sector */
	u32		C1ErrNum;	/* total # of C1 errors */
	u32		C1ErrSector[4];	/* error sectors */
} __OSBlockInfo;

typedef struct {
	u32     	cmdType;       	/* for disk only */
	u16     	transferMode;   /* Block, Track, or sector?   */
	u16		blockNum;	/* which block is transfering */
	s32     	sectorNum;      /* which sector is transfering */
	u32     	devAddr;        /* Device buffer address */
	u32		bmCtlShadow;	/* asic bm_ctl(510) register shadow ram */
	u32		seqCtlShadow;	/* asic seq_ctl(518) register shadow ram */
	__OSBlockInfo	block[2];	/* bolck transfer info */
} __OSTranxInfo;


typedef struct OSPiHandle_s {
        struct OSPiHandle_s     *next;  /* point to next handle on the table */
        u8                      type;   /* DEVICE_TYPE_BULK for disk */
        u8                      latency;        /* domain latency */
        u8                      pageSize;       /* domain page size */
        u8                      relDuration;    /* domain release duration */
        u8                      pulse;          /* domain pulse width */
	u8			domain;		/* which domain */
        u32                     baseAddress;    /* Domain address */
        u32                     speed;          /* for roms only */
        /* The following are "private" elements" */
        __OSTranxInfo           transferInfo;	/* for disk only */
} OSPiHandle;

typedef struct {
	OSIoMesgHdr	hdr;		/* Message header */
	void *		dramAddr;	/* RDRAM buffer address (DMA) */
	u32		devAddr;	/* Device buffer address (DMA) */
	u32 		size;		/* DMA transfer size in bytes */
	OSPiHandle	*piHandle;	/* PI device handle */
} OSIoMesg;

extern void		osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32		osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32		osJamMesg(OSMesgQueue *, OSMesg, s32);
extern s32		osRecvMesg(OSMesgQueue *, OSMesg *, s32);

extern void		osInvalDCache(void *, s32);
extern void		osInvalICache(void *, s32);
extern void		osWritebackDCache(void *, s32);
extern void		osWritebackDCacheAll(void);

extern s32		osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32,
				     OSMesgQueue *);

extern void		osWritebackDCacheAll(void);

#endif //_OS_H_