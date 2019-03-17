#ifndef PUBLIC_MEMPOOL_H_
#define PUBLIC_MEMPOOL_H_

#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef unsigned char   Uint8;
typedef int             int16;
typedef long            int32;
typedef unsigned int    Uint16;
typedef unsigned long   Uint32;
typedef float           float32;
typedef long double     float64;
#endif


#define     INT_LOCK()              asm("    SETC INTM ")
#define INT_UNLOCK()                asm("    CLRC INTM ")

typedef struct MemPoolTag {
    void *free;
    void *start;
    void *end;
    Uint16 blockSize;
    Uint16 nTot;
    Uint16 nFree;
    Uint16 nMin;
} MemPool;

/* ÄÚ´æ³Ø²Ù×÷ -----------------------------------------------------------------------------------*/
void MemPool_init(MemPool *me, void *poolSto, Uint16 poolSize, Uint16 blockSize);
void *MemPool_get(MemPool *me);
void MemPool_put(MemPool *me, void *b);
inline Uint16 MemPool_freeNum(MemPool *me){
    return me->nFree;
}




#endif /* PUBLIC_MEMPOOL_H_ */
