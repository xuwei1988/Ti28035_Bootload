#ifndef CAN_CANPORT_H_
#define CAN_CANPORT_H_
#include"basic.h"

/* оƬ���ʹ��� ��������������������������������������������������������������������������*/
#define DSP28035                35
#define DSP2808                 80
#define DSP28335                335

/* оƬ�����趨 ��������������������������������������������������������������������������*/
#define CHIP_TYPE               DSP28035

/* �������ʹ��� ��������������������������������������������������������������������������*/
#define SYSTEM                  1
#define POWER                   2
#define MONITOR                 3

/* ���ݻ�������� ������������������������������������������������������������������������*/
#define MSG_PIECE_NUM           30

/* �������ͣ�������������������������������������������������������������������������������*/
//#define DEVICE_TYPE             POWER
//#define DEVICE_ID               1

#define DEVICE_TYPE             SYSTEM
#define DEVICE_ID               0

//#define DEVICE_TYPE             MONITOR
//#define DEVICE_ID               0

#if(DEVICE_TYPE == SYSTEM)
    #define DEVICE_MASTERID     0
    #define DEVICE_MASK         0xFF

#endif

#if(DEVICE_TYPE == POWER)
    #define DEVICE_MASTERID     0
    #define DEVICE_MASK         0x0F
#endif

#if(DEVICE_TYPE == MONITOR)
    #define DEVICE_MASTERID     0
    #define DEVICE_MASK         0xFF
#endif

#define SYSTEM_NUM          1
#define POWER_NUM           2
#define MONITOR_NUM         1


/* 2017.04.13 Э�����Ͷ��� ------------------------------------------------------------------*/
#define BOOTLOADPROTOCOL        0x09

typedef struct{
    Uint16 type;
    Uint16 ID;
    Uint16 masterID;
    Uint16 broadcastMask;
}NodeImfor;

typedef struct{
    Uint32 filter;
    Uint32 mask;
}Pair;

typedef struct{
    NodeImfor sNode;
    Uint8 protocol;
    Uint8 dataType;
    Uint8 len;
    Uint16 data[60];
}ReceiveImfor;

#endif /* CAN_CANPORT_H_ */
