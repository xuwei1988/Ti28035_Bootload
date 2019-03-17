#include"canmsgblock.h"
#include"assertEx.h"
DEFINE_THIS_MODULE(canmsgblock)

MsgPiece canbuffSto[MSG_PIECE_NUM];
MemPool canPool;

MsgPiece *rCHead = (MsgPiece *)0;
MsgPiece *sCHead = (MsgPiece *)0;

void Can_poolInit(void *poolSto, Uint16 poolSize, Uint16 blockSize){
    MemPool_init(&canPool, poolSto, poolSize, blockSize);
}

void Can_msgInsert(MsgPiece *pm){
    Uint16 prio = (Uint16)pm->msg.cob_id.bit.prio;
    MsgPiece **pp = &sCHead;
    MsgPiece *p;
    //step1: 检查是否已有接收数据
    INT_LOCK();
    if(!sCHead){
        sCHead = pm;
        pm->pNext = (MsgPiece *)0;
    }else{
        while(prio >= (Uint16)(*pp)->msg.cob_id.bit.prio
                && (*pp))
            pp = &(*pp)->pNext;
        p = *pp;
        *pp = pm;
        pm->pNext = p;
    }
    INT_UNLOCK();
}

Uint16 Can_msgPop(Message *pMsg){
    MsgPiece *p = sCHead;
    //step1: 判断是否有消息
    INT_LOCK();
    if(!sCHead){
        INT_UNLOCK();
        return 0;
    }
    sCHead = sCHead->pNext;
    INT_UNLOCK();
    memcpy(pMsg, &p->msg, sizeof(*pMsg));
    MemPool_put(&canPool, p);
    return 1;
}

/* 2017.03.08 获取链表元素个数 －－*/
Uint16 Can_msgNum(void){
    MsgPiece *p = sCHead;
    Uint16 n = 0;
    while(p){
        n++;
        p = p->pNext;
    }
    return n;
}








