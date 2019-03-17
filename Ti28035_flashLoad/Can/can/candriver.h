#ifndef CAN_CANDRIVER_H_
#define CAN_CANDRIVER_H_
#include"canport.h"
#include"canmsgblock.h"

/* 数据量设定--------------------------------------------------------------------------------------*/
#define DATA_COMMAND            0
#define DATA_REQUEST            1
#define MAX_MSG_PRIO            7           //不能改变


#define ERROR_MSG               0
#define PART_MSG                1
#define COMPLETE_MSG            2
#define NO_MSG                  3

/* 数据接收链表的表头数量 ---------------------------------------------------------------------------*/
#if(DEVICE_TYPE == SYSTEM)
    #define MSG_SRC_NUM             ((SYSTEM_NUM - 1) + POWER_NUM + MONITOR_NUM) * 8
#endif

#if(DEVICE_TYPE == POWER)
    #define MSG_SRC_NUM             (SYSTEM_NUM + MONITOR_NUM) * 8
#endif

#if(DEVICE_TYPE == MONITOR)
    #define MSG_SRC_NUM             (SYSTEM_NUM + POWER_NUM) * 8
#endif


/* 目的地址----------------------------------------------------------------------------------------*/
typedef struct{
    Uint16 type;            //机型
    Uint16 ID;              //ID
}Address;

/* 待发送数据头-------------------------------------------------------------------------------------*/
typedef struct {
    Uint8   prio;           //优先级
    Address dest;           //目的地址
    Uint8   protocol;       //所用协议
    Uint8   dataType;       //读还是写
    Uint16  len;            //数据长度
}SendImfor;


/* 初始化函数 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
void Can_nodeInit(Uint16 type, Uint16 ID, Uint16 masterID, Uint16 broadcastMask);
void Can_globalInit(void);
Uint16 Can_push(SendImfor *sendImfor, void *pMsg);
Pair Can_driverFilterCalc(NodeImfor *nodeimfor);
void Can_getNodeImfor(NodeImfor *nodeImfor, Uint16 data);
Uint16 Can_msgPro(Message *msg);

/* 数据目的地址设定函数 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
#if(DEVICE_TYPE == POWER)
    void Can_RequestSendToMaster(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandSendToMaster(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_RequestSendToMonitor(SendImfor *sendImfor,
                Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandSendToMonitor(SendImfor *sendImfor,
                Uint8 prio, Uint8 protocol, Uint16 len);
#endif

#if(DEVICE_TYPE == SYSTEM)
    void Can_RequestSendToSlave(SendImfor *sendImfor, Uint16 ID,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandSendToSlave(SendImfor *sendImfor, Uint16 ID,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_RequestBroadcastToSlave(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandBroadcastToSlave(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_RequestSendToMonitor(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandSendToMonitor(SendImfor *sendImfor,
                Uint8 prio, Uint8 protocol, Uint16 len);
#endif

#if(DEVICE_TYPE == MONITOR)
    void Can_RequestSendToMaster(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandSendToMaster(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_RequestSendToSlave(SendImfor *sendImfor, Uint16 ID,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandSendToSlave(SendImfor *sendImfor, Uint16 ID,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_RequestBroadcastToSlave(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);
    void Can_CommandBroadcastToSlave(SendImfor *sendImfor,
            Uint8 prio, Uint8 protocol, Uint16 len);

#endif

extern NodeImfor canNode;
extern MsgPiece *receiveMsgLinks[MSG_SRC_NUM];
extern ReceiveImfor receiveImfor;
extern volatile Uint16 isMBoxEmpty;

#endif /* CAN_CANDRIVER_H_ */
