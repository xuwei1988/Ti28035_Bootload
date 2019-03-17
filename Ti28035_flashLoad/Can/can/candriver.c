#include"canmsgblock.h"
#include"candriver.h"
#include"assertEx.h"

DEFINE_THIS_MODULE(candriver)
NodeImfor canNode;
MsgPiece *receiveMsgLinks[MSG_SRC_NUM];
ReceiveImfor receiveImfor;
volatile Uint16 isMBoxEmpty;

/* 局部函数 声明 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
static Uint8 Can_buffNeeds(Uint16 len);
static MsgPiece *Can_recvPush(Message *msg, NodeImfor *sNode);
static Uint8 Can_rMsgPosGet(NodeImfor *sNode);
static MsgPiece *Can_rMsgLinkGet(NodeImfor *sNode, Message *msg);
static Uint8 Can_msgAnalysis(Message *msg, NodeImfor *sNode);
static void Can_dstWrite(SendImfor *sendImfor,
        Uint8 dataType,
        Uint16 destType,
        Uint16 destID,
        Uint8 prio,
        Uint8 protocol,
        Uint16 len);

/* 全局函数 实现 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
void Can_nodeInit(Uint16 type, Uint16 ID, Uint16 masterID, Uint16 broadcastMask){
    canNode.type = type;
    canNode.ID = ID;
    canNode.masterID = masterID;
    canNode.broadcastMask = broadcastMask;
}

void Can_globalInit(void){
    bzero((Uint8 *)receiveMsgLinks, sizeof(receiveMsgLinks));
    bzero((Uint8 *)&receiveImfor, sizeof(receiveImfor));
    isMBoxEmpty = 1;
    rCHead = (MsgPiece *)0;
    sCHead = (MsgPiece *)0;
}

Uint16 Can_push(SendImfor *sendImfor, void *pMsg){
    Uint16  target, source;
    MsgPiece *mp;
    Address *dest;
    Uint8   *pdata;
    Uint8   need, free;
    Uint16  i, left;
    dest = &sendImfor->dest;
    /*step1: 计算内存块用量*/
    need = Can_buffNeeds(sendImfor->len);
    free = MemPool_freeNum(&canPool);
    if(need > free)
        return 0;
    /*step2: 生成地址*/
#if(DEVICE_TYPE == SYSTEM || DEVICE_TYPE == MONITOR)
    source = (1<<(canNode.ID)) | (canNode.type<<8);
    if(dest->type != POWER){
        target = (1<<(dest->ID)) | (dest->type<<8);
    }else{
        target = (dest->type<<8) + (canNode.ID<<4) + dest->ID;
    }
#endif


#if(DEVICE_TYPE == POWER)
    source = (canNode.type<<8) + (canNode.masterID<<4) + canNode.ID;
    REQUIRE(dest->type != POWER);  //目的地址必须是一个主机
    target = (1<<(dest->ID)) | (dest->type)<<8;
#endif

    /*step3: 批量处理满4字节的数据帧*/
    for(i = 0; i < need - 1; i++){
        mp = (MsgPiece *)MemPool_get(&canPool);
        mp->msg.cob_id.bit.source   = (Uint32)source;
        mp->msg.cob_id.bit.target   = (Uint32)target;
        mp->msg.cob_id.bit.protocol = (Uint32)sendImfor->protocol;
        mp->msg.cob_id.bit.dataType = (Uint32)sendImfor->dataType;
        mp->msg.cob_id.bit.prio     = (Uint32)sendImfor->prio;

        mp->msg.cob_id.bit.isLast   = (Uint32)0;
        pdata = (Uint8 *)pMsg + 4 *i * sizeof(Uint16);
        memcpy(mp->msg.data, pdata, 4 * sizeof(Uint16));
        mp->msg.len = 4;
        mp->msg.rtr = 0;

        Can_msgInsert(mp);
    }

    /*step4: 处理最后一帧数据 */
    left = sendImfor->len - 4*i;
    mp = (MsgPiece *)MemPool_get(&canPool);

    mp->msg.cob_id.bit.source   = (Uint32)source;
    mp->msg.cob_id.bit.target   = (Uint32)target;
    mp->msg.cob_id.bit.protocol = (Uint32)sendImfor->protocol;
    mp->msg.cob_id.bit.dataType = (Uint32)sendImfor->dataType;
    mp->msg.cob_id.bit.prio     = (Uint32)sendImfor->prio;

    mp->msg.cob_id.bit.isLast = 1;
    pdata = (Uint8 *)pMsg + 4 *i * sizeof(Uint16);
    memcpy(mp->msg.data, pdata, left * sizeof(Uint16));
    mp->msg.len = left;
    mp->msg.rtr = 0;
    Can_msgInsert(mp);

    return 1;
}


/* 发送函数 系列 -----------------------------------------------------------------------------------*/
/* 以下一系列函数，可以精简， 但是并没有这么做*/

#if(DEVICE_TYPE == POWER || DEVICE_TYPE == MONITOR)
    void Can_RequestSendToMaster(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        Can_dstWrite(sendImfor, DATA_REQUEST,SYSTEM,canNode.masterID,prio,protocol,len);
    }

    void Can_CommandSendToMaster(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        Can_dstWrite(sendImfor, DATA_COMMAND,SYSTEM,canNode.masterID,prio,protocol,len);
    }

#endif

#if(DEVICE_TYPE == POWER)
    void Can_RequestSendToMonitor(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        Can_dstWrite(sendImfor, DATA_REQUEST,MONITOR,canNode.masterID,prio,protocol,len);
    }

    void Can_CommandSendToMonitor(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        Can_dstWrite(sendImfor, DATA_COMMAND,MONITOR,canNode.masterID,prio,protocol,len);
    }
#endif

#if(DEVICE_TYPE == SYSTEM || DEVICE_TYPE == MONITOR)
    void Can_RequestSendToSlave(SendImfor *sendImfor, Uint16 ID,
            Uint8 prio, Uint8 protocol, Uint16 len){
        REQUIRE(ID <= 14);
        REQUIRE(prio <= MAX_MSG_PRIO && protocol <= 0xF);
        Can_dstWrite(sendImfor, DATA_REQUEST,POWER,ID,prio,protocol,len);
    }

    void Can_CommandSendToSlave(SendImfor *sendImfor, Uint16 ID,
            Uint8 prio, Uint8 protocol, Uint16 len){
        REQUIRE(ID <= 14);
        REQUIRE(prio <= MAX_MSG_PRIO && protocol <= 0xF);
        Can_dstWrite(sendImfor, DATA_COMMAND,POWER,ID,prio,protocol,len);
    }

    void Can_RequestBroadcastToSlave(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){;
        REQUIRE(prio <= MAX_MSG_PRIO && protocol <= 0xF);
        Can_dstWrite(sendImfor, DATA_REQUEST,POWER,0xF,prio,protocol,len);
    }

    void Can_CommandBroadcastToSlave(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        REQUIRE(prio <= MAX_MSG_PRIO && protocol <= 0xF);

        Can_dstWrite(sendImfor, DATA_COMMAND,POWER,0xF,prio,protocol,len);
    }
#endif

#if(DEVICE_TYPE == SYSTEM)
    void Can_RequestSendToMonitor(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        REQUIRE(prio <= MAX_MSG_PRIO && protocol <= 0xF);
        Can_dstWrite(sendImfor, DATA_REQUEST,MONITOR,canNode.ID,prio,protocol,len);
    }

    void Can_CommandSendToMonitor(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len){
        REQUIRE(prio <= MAX_MSG_PRIO && protocol <= 0xF);
        Can_dstWrite(sendImfor, DATA_REQUEST,MONITOR,canNode.ID,prio,protocol,len);

    }
#endif



/* end---------------------------------------------------------------------------------------------*/

Pair Can_driverFilterCalc(NodeImfor *nodeimfor){
    Pair pair;
#if(DEVICE_TYPE == SYSTEM || DEVICE_TYPE == MONITOR)
//    pair.filter = ((Uint32)(0x100 + (1<<nodeimfor->ID)))<<10;
//    pair.mask = ~(((Uint32)(0x300 + (1<<nodeimfor->ID)))<<10);
    pair.filter = ((Uint32)(((nodeimfor->type)<<8) + (1<<nodeimfor->ID)))<<10;
    pair.mask = ~(((Uint32)(0x300 + (1<<nodeimfor->ID)))<<10);
#endif

#if(DEVICE_TYPE == POWER)
    Uint32 type = nodeimfor->type<<8;
    Uint32 master = nodeimfor->masterID<<4;
    pair.filter = (type + master)<<10;
    pair.mask = 0xFFF03FFF;
#endif
    return pair;
}

void Can_getNodeImfor(NodeImfor *nodeImfor, Uint16 data){
    nodeImfor->type = (data&0x0300)>>8;
    if(nodeImfor->type == SYSTEM || nodeImfor->type == MONITOR){
        int temp, i;
        nodeImfor->masterID = 0;

        temp = (Uint16)(data&0x00FF);
        if(temp == 0x00FF){                     //这里不好，默认了系统之间的广播地址为0xFF
            nodeImfor->ID = temp;
            return;
        }
        for(i = 0; i < 8; i++){
            if((temp>>i)&1){
                nodeImfor->ID = i;
                break;
            }
        }
    }

    if(nodeImfor->type == POWER){
        nodeImfor->masterID = (Uint16)((data&0x00F0)>>4);
        nodeImfor->ID = (Uint16)(data&0x000F);
    }
}

Uint16 Can_msgPro(Message *msg){
    NodeImfor targetNode, sourceNode;
    Uint16 target, source;


    /*step1: 分解标识符号*/
    target = (Uint16)msg->cob_id.bit.target;             //目标地址
    source = (Uint16)msg->cob_id.bit.source;             //源地址

    /*step2: 根据标识符获取targetNode 和sourceNode信息*/
    Can_getNodeImfor(&targetNode, target);
    Can_getNodeImfor(&sourceNode, source);
    /*step3: 根据自身设备信息进行软件仲裁*/
#if(DEVICE_TYPE == SYSTEM || DEVICE_TYPE == MONITOR)
    if(targetNode.type == canNode.type){
        if(sourceNode.type != targetNode.type){
            if(targetNode.ID == canNode.ID){
                return Can_msgAnalysis(msg, &sourceNode) ? COMPLETE_MSG : PART_MSG;
            }else{
                return ERROR_MSG;
            }
        }else{
            if(targetNode.ID == canNode.ID || targetNode.ID == canNode.broadcastMask){
                return Can_msgAnalysis(msg, &sourceNode) ? COMPLETE_MSG : PART_MSG;
            }else{
                return ERROR_MSG;
            }
        }
    }else{
        return ERROR_MSG;
    }
#endif

#if(DEVICE_TYPE == POWER)
    if(targetNode.type == canNode.type){                    //类型是否相同
        if(targetNode.masterID == canNode.masterID){        //主机是否相同
            if(targetNode.ID == canNode.ID || targetNode.ID == canNode.broadcastMask){
                //在此写入接收信息的代码
                return Can_msgAnalysis(msg, &sourceNode) ? COMPLETE_MSG : PART_MSG;

            }else{
                return ERROR_MSG;
            }
        }else{
            return ERROR_MSG;
        }
    }else{
        return ERROR_MSG;
    }
#endif
}

static Uint8 Can_msgAnalysis(Message *msg, NodeImfor *sNode){
    if(!msg->cob_id.bit.isLast){        //不是最后一片数据
        Can_recvPush(msg, sNode);
        return 0;
    }else{                              //如果是最后一片数据
        MsgPiece *pcurr;
        MsgPiece *pnext;
        Uint8 i = 0;
        pcurr = Can_rMsgLinkGet(sNode, msg);
        /* step1: 保存信息来源和协议*/
        memcpy(&receiveImfor.sNode, sNode, sizeof(*sNode));
        receiveImfor.protocol = (Uint8)msg->cob_id.bit.protocol;
        receiveImfor.dataType = (Uint8)msg->cob_id.bit.dataType;
        /* step2: 拷贝之前存在链表中的数据信息*/
        while(pcurr){
            pnext = pcurr->pNext;
            memcpy((Uint8 *)receiveImfor.data + i, pcurr->msg.data, 4 * sizeof(Uint16));
            /* step3: 将内存返还给系统*/
            MemPool_put(&canPool, pcurr);
            pcurr = pnext;
            i += 4 * sizeof(Uint16);
        }
        /* step4: 完成最后一片数据的组装 */
        memcpy((Uint8 *)receiveImfor.data + i, msg->data, msg->len * sizeof(Uint16));
        i += msg->len;
        receiveImfor.len = i;
        return 1;
    }
}

/* 局部函数 实现 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
static Uint8 Can_buffNeeds(Uint16 len){
    Uint8 n;
    n = len/4;
    if(len % 4)
        n++;
    return n;
}


/* 基本发送数据的帧头填写函数 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
void Can_dstWrite(SendImfor *sendImfor,
                Uint8 dataType,
                Uint16 destType,
                Uint16 destID,
                Uint8 prio,
                Uint8 protocol,
                Uint16 len){
    sendImfor->dataType = dataType;
    sendImfor->dest.type = destType;
    sendImfor->dest.ID = destID;

    //根据参数， 初始化以下数据
    sendImfor->prio = prio;
    sendImfor->protocol = protocol;
    sendImfor->len = len;
}


/* 向接收链表添加接收数据 -------------------------------------------------------*/
static MsgPiece *Can_recvPush(Message *msg, NodeImfor *sNode){
    MsgPiece *s;
    MsgPiece **pLink;       //链表指针
    Uint8 n;
    /* step1: 先获取一段内存，并将内容复制进去*/
    s = (MsgPiece *)MemPool_get(&canPool);
    /* 如果内存已经用完， 返回空指针*/
    if(s == (MsgPiece *)0){
        return s;
    }
    memcpy(&s->msg, msg, sizeof(*msg));

    /* step2: 计算该存放在哪里*/
    n = Can_rMsgPosGet(sNode);
    n += msg->cob_id.bit.prio;

    pLink = &receiveMsgLinks[n];
    INT_LOCK();
    while(*pLink)
        pLink = &(*pLink)->pNext;
    *pLink = s;
    s->pNext = (MsgPiece *)0;
    INT_UNLOCK();
    return s;
}

/* 返回一个链表表头 ------------------------------------------------------------------------------------*/
static MsgPiece *Can_rMsgLinkGet(NodeImfor *sNode, Message *msg){
    MsgPiece *s;
    Uint8 n;
    /* step1: 计算该存放在哪里*/
    n = Can_rMsgPosGet(sNode);
    n += msg->cob_id.bit.prio;
    s = receiveMsgLinks[n];
    receiveMsgLinks[n] = (MsgPiece *)0;
    return s;
}

/* 返回一个存储位置 ------------------------------------------------------------------------------------*/
static Uint8 Can_rMsgPosGet(NodeImfor *sNode){
    Uint8 n;
#if(DEVICE_TYPE == SYSTEM)
    switch(sNode->type){
        case SYSTEM:{
            n = ((sNode->ID)<<3);
            break;
        }
        case POWER:{
            n = (((SYSTEM_NUM - 1) + sNode->ID) << 3);      //7为前面的SYSTEM的预留位
            break;
        }
        case MONITOR:{
            n = (((SYSTEM_NUM - 1) + POWER_NUM) << 3);  //15为前面POWER的预留位
            break;
        }
    }
#endif

#if(DEVICE_TYPE == POWER)
    switch(sNode->type){
        case SYSTEM:{
            n = 0;
            break;
        }
        case MONITOR:{
            n = 8;
        }
    }
#endif

#if(DEVICE_TYPE == MONITOR)
    switch(sNode->type){
        case SYSTEM:{
            n = 0;
            break;
        }
        case POWER:{
            n = ((1 + sNode->ID) << 3);
        }
    }
#endif

    return n;
}


