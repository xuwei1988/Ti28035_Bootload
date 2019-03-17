#include"mempool.h"
#include"assertEx.h"
DEFINE_THIS_MODULE(mempool)

typedef struct QFreeBlockTag {
    struct QFreeBlockTag *next;
} QFreeBlock;

void MemPool_init(MemPool *me, void *poolSto, Uint16 poolSize, Uint16 blockSize)
{
    QFreeBlock *fb;
    Uint16 corr;
    Uint16 nblocks;

    corr = ((Uint32)poolSto & ((Uint32)sizeof(QFreeBlock) - (Uint32)1));
    if (corr != (Uint16)0) {
        corr = (Uint16)sizeof(QFreeBlock) - corr;
        poolSize -= corr;
    }
    me->free = (void *)((Uint8 *)poolSto + corr);
    me->blockSize = (Uint16)sizeof(QFreeBlock);
    nblocks = (Uint16)1;
    while (me->blockSize < blockSize) {
        me->blockSize += (Uint16)sizeof(QFreeBlock);
        ++nblocks;
    }
    blockSize = me->blockSize;

    poolSize -= (Uint16)blockSize;
    me->nTot  = 1;
    fb        = (QFreeBlock *)me->free;
    while (poolSize >= (Uint16)blockSize) {
        fb->next = &fb[nblocks];
        fb = fb->next;
        poolSize -= (Uint16)blockSize;
        ++me->nTot;
    }

    fb->next  = (QFreeBlock *)0;
    me->nFree = me->nTot;
    me->nMin  = me->nTot;
    me->start = poolSto;
    me->end   = fb;
}

void MemPool_put(MemPool *me, void *b) {
    REQUIRE((me->start <= b) && (b <= me->end)
            && (me->nFree <= me->nTot));
    INT_LOCK();
    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free;
    me->free = b;
    ++me->nFree;
    INT_UNLOCK();
}
void *MemPool_get(MemPool *me) {
    QFreeBlock *fb;
    INT_LOCK();
    fb = (QFreeBlock *)me->free;
    if (fb != (QFreeBlock *)0) {
        me->free = fb->next;
        --me->nFree;
        if (me->nMin > me->nFree) {
            me->nMin = me->nFree;
        }
    }
    INT_UNLOCK();
    return fb;
}
