#ifndef CAN_CANMSGBLOCK_H_
#define CAN_CANMSGBLOCK_H_

#include"canport.h"

/* Can数据包----------------------------------------------------------------------------------------*/
typedef struct{
    union COB_ID{
        Uint32 all;
        struct{
            Uint32 source   :10;            //源地址
            Uint32 target   :10;            //目的地址
            Uint32 protocol :4;         //协议类型
            Uint32 isLast   :1;         //是否是最后一个数据片
            Uint32 dataType :1;         //读或是写
            Uint32 prio     :3;         //数据优先级
        }bit;
    }cob_id;
    Uint16 len;
    Uint16 rtr;
    Uint16 data[4];
}Message;

/* 数据接收链表 ------------------------------------------------------------------------------*/
typedef struct MsgPieceTag{
    struct MsgPieceTag *pNext;
    Message msg;
}MsgPiece;

void Can_poolInit(void *poolSto, Uint16 poolSize, Uint16 blockSize);
void Can_msgInsert(MsgPiece *pm);
Uint16 Can_msgPop(Message *pMsg);
/* 2017.03.08 获取链表元素个数 －－*/
Uint16 Can_msgNum(void);

extern MsgPiece canbuffSto[MSG_PIECE_NUM];
extern MemPool canPool;
extern MsgPiece *rCHead;
extern MsgPiece *sCHead;
#endif /* CAN_CANMSGBLOCK_H_ */
