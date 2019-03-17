#include"candsp.h"
#include"canmsgblock.h"

/* �������� --------------------------------------------------------------------------------------------*/
static void Can_dspHardwareInit(Pair *pair);
static int Can_dspIntClear(int mailNum);
static int Can_dspBasicSend(volatile struct MBOX *newMail);
static int Can_dspBasicRecv(Message *msg, volatile struct MBOX *newMail);

void can_dspInit(void){
    Pair pair;

    /* �ײ��ʼ�� -----------------------------------------------------------------------------------*/
    /* step0: ȫ�ֱ�������*/
    Can_globalInit();

    /* step1: ��Ϣ�����ʼ��*/
    Can_poolInit(canbuffSto, sizeof(canbuffSto), sizeof(canbuffSto[0]));

    /* step2: �豸��Ϣ��ʼ��*/
    Can_nodeInit(DEVICE_TYPE,  DEVICE_ID, DEVICE_MASTERID, DEVICE_MASK);

    /* step3: ��ȡ�����������������͹�����*/
    pair = Can_driverFilterCalc(&canNode);
    /* step5: Ӳ����ʼ��*/
    Can_dspHardwareInit(&pair);

}

void Can_dspHardwareInit(Pair *pair){
    struct ECAN_REGS ECanaShadow;
    EALLOW;
#if(CHIP_TYPE == DSP28035)
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;     // Enable pull-up for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;     // Enable pull-up for GPIO31 (CANTXA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO30=3;     //CANA, Asynchronous
    GpioCtrlRegs.GPAQSEL2.bit.GPIO31=3;     //CANA, Asynchronous

    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 for CANTXA operation
#endif

#if(CHIP_TYPE == DSP2808)
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;     // Enable pull-up for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;     // Enable pull-up for GPIO31 (CANTXA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO30=3;         //CANA, Asynchronous
    GpioCtrlRegs.GPAQSEL2.bit.GPIO31=3;         //CANA, Asynchronous

    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 for CANTXA operation
#endif

#if(CHIP_TYPE == DSP28335)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;     // Enable pull-up for GPIO18 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;     // Enable pull-up for GPIO19 (CANTXA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;    // Configure GPIO18 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;    // Configure GPIO19 for CANTXA operation
#endif

    EDIS;

    /* Configure eCAN RX and TX pins for eCAN transmissions using eCAN regs*/
    EALLOW;
    ECanaRegs.CANMIM.all = 0xFFFFFFFF;      //�ж����μĴ���
    ECanaRegs.CANMIL.all = 0xFFFFFFFF;      //ECAN1INT
    ECanaRegs.CANGIM.all=0;
    ECanaRegs.CANGIM.bit.I1EN = 1;
    ECanaRegs.CANGIM.bit.I0EN=0;
    ECanaRegs.CANGIM.bit.BOIM=1;
    ECanaRegs.CANGIM.bit.GIL=1;

    ECanaRegs.CANTIOC.all=0x0008;// ECanaRegs.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANRIOC.all=0x0008;//ECanaRegs.CANRIOC.bit.RXFUNC = 1;


    ECanaRegs.CANTA.all=0xFFFFFFFF;

    ECanaRegs.CANRMP.all=0xFFFFFFFF;

    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;


    ECanaShadow.CANMC.all=ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR=1;
    ECanaShadow.CANMC.bit.DBO = 1;
    ECanaRegs.CANMC.all=ECanaShadow.CANMC.all;

    EDIS;
    while(ECanaShadow.CANES.bit.CCE==0)
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    }

    EALLOW;
    //1Mbps
    ECanaShadow.CANBTC.all=ECanaRegs.CANBTC.all;

    /* The following block is for 100 MHz SYSCLKOUT. (50 MHz CAN module clock Bit rate = 1 Mbps
       See Note at end of file. */
    //1M baudrate = SYSCLKOUT/((BRPREG+1)*(TSEG1REG+TSEG2REG+3))

#if(CHIP_TYPE == DSP28035)
    ECanaShadow.CANBTC.bit.BRPREG = 5;
#endif

#if(CHIP_TYPE == DSP2808)
    ECanaShadow.CANBTC.bit.BRPREG = 9;
#endif

#if(CHIP_TYPE == DSP28335)
    ECanaShadow.CANBTC.bit.BRPREG = 4;
#endif

    ECanaShadow.CANBTC.bit.TSEG2REG = 1;
    ECanaShadow.CANBTC.bit.TSEG1REG = 6;

    ECanaShadow.CANBTC.bit.SAM = 1;
    ECanaRegs.CANBTC.all=ECanaShadow.CANBTC.all;
    ECanaShadow.CANMC.all=ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR=0;
    ECanaShadow.CANMC.bit.ABO=1;
    ECanaShadow.CANMC.bit.PDR=0;
    ECanaShadow.CANMC.bit.WUBA=0;
    ECanaShadow.CANMC.bit.CDR=0;
    ECanaShadow.CANMC.bit.DBO=0;
    ECanaShadow.CANMC.bit.STM=0;
    ECanaShadow.CANMC.bit.SRES=0;
    ECanaShadow.CANMC.bit.MBNR=0;
    ECanaRegs.CANMC.all=ECanaShadow.CANMC.all;
    EDIS;
    while(ECanaShadow.CANES.bit.CCE==1)
    {
        ECanaShadow.CANES.all=ECanaRegs.CANES.all;
    }
    EALLOW;
    ECanaRegs.CANME.all = 0;
    EDIS;

    EALLOW;
    ECanaShadow.CANME.all=ECanaRegs.CANME.all;
    ECanaShadow.CANME.bit.ME5=0;
    ECanaShadow.CANME.bit.ME4=0;
    ECanaShadow.CANME.bit.ME3=0;

    /*2017. 1.6���������� ������������������*/



    /*2017. 1.6 end��������������������������*/

    ECanaRegs.CANME.all=ECanaShadow.CANME.all;
    EDIS;

    ECanaMboxes.MBOX5.MSGID.all = pair->filter;
    ECanaMboxes.MBOX5.MSGID.bit.AME = 1;
    ECanaMboxes.MBOX5.MSGID.bit.IDE = 1;

    ECanaMboxes.MBOX4.MSGID.all = pair->filter;
    ECanaMboxes.MBOX4.MSGID.bit.AME = 1;
    ECanaMboxes.MBOX4.MSGID.bit.IDE = 1;

    ECanaMboxes.MBOX3.MSGID.all = pair->filter;
    ECanaMboxes.MBOX3.MSGID.bit.AME = 1;
    ECanaMboxes.MBOX3.MSGID.bit.IDE = 1;


    /*2017. 1.6���������� ������������������*/





    /*2017. 1.6 end��������������������������*/

    EALLOW;
    ECanaLAMRegs.LAM3.all = pair->mask;
    ECanaLAMRegs.LAM3.bit.LAMI=1;
    ECanaLAMRegs.LAM3.bit.rsvd1=0x0003;

    /*2017. 1.6���������� ������������������*/



    /*2017. 1.6 end��������������������������*/
    EDIS;

    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC=0x0008;
    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC=0x0008;
    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC=0x0008;

    /*2017. 1.6���������� ������������������*/


    /*2017. 1.6 end��������������������������*/

    EALLOW;
    ECanaShadow.CANMD.all=ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.bit.MD5=1;
    ECanaShadow.CANMD.bit.MD4=1;
    ECanaShadow.CANMD.bit.MD3=1;

    /*2017. 1.6���������� ������������������*/


    /*2017. 1.6 end��������������������������*/
    ECanaRegs.CANMD.all=ECanaShadow.CANMD.all;

    ECanaShadow.CANOPC.all = ECanaRegs.CANOPC.all;
    ECanaShadow.CANOPC.bit.OPC5 = 1;
    ECanaShadow.CANOPC.bit.OPC4 = 1;

    /*2017. 1.6���������� ������������������*/


    /*2017. 1.6 end��������������������������*/
    ECanaRegs.CANOPC.all = ECanaShadow.CANOPC.all;

    ECanaShadow.CANME.all=ECanaRegs.CANME.all;
    ECanaShadow.CANME.bit.ME5=1;
    ECanaShadow.CANME.bit.ME4=1;
    ECanaShadow.CANME.bit.ME3=1;

    /*2017. 1.6���������� ������������������*/


    /*2017. 1.6 end��������������������������*/

    ECanaShadow.CANME.bit.ME1=1;
    ECanaRegs.CANME.all=ECanaShadow.CANME.all;
    EDIS;
}



Uint16 Can_post(SendImfor *sendImfor, void *pMsg){
    Uint16 r = 0;
    Uint16 isEmpty = sCHead ? 0 : 1;
    r = Can_push(sendImfor, pMsg);
    if(isEmpty){
        while(isMBoxEmpty == 0){
            static Uint16 i = 0;
            i++;
        }
        Can_dspSend();
    }
    return r;
}

/* �˺���ֻʹ����ֻ��һ�������������� --------------------------------------------------------------*/
/* ��������ʹ�ܼĴ����ͷ���Ĵ����ж��ĸ��Ƿ�������*/
Uint8 Can_dspSend(void){
    Uint32 enboxs;      //��ʹ�ܵ�����
    Uint32 direct;      //���䷽��
    Uint32 sendbox;
    Uint32 code;
    Uint32 maskcode;
    Uint8 *p;
    struct ECAN_REGS ECanaShadow;
    int i;
    Uint8 s;
    EALLOW;
    enboxs = ECanaRegs.CANME.all;
    direct = ECanaRegs.CANMD.all;
    EDIS;
    sendbox = enboxs^direct;
    for(i = 0; i < 32; i++){
        if(sendbox>>i & 1)
            break;
    }
    code = 1<<i;
    maskcode = ~code;

    p = ((Uint8 *)&ECanaMboxes) + i * sizeof(struct MBOX);

/**********************************************************************************************************/

    EALLOW;
    ECanaShadow.CANME.all=ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= maskcode;
    ECanaRegs.CANME.all=ECanaShadow.CANME.all;
    ECanaShadow.CANMC.all=ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CDR=1;
    ECanaRegs.CANMC.all=ECanaShadow.CANMC.all;
    EDIS;
/**********************************************************************************************************/

    s = Can_dspBasicSend((volatile struct MBOX *)p);

/**********************************************************************************************************/
    EALLOW;
    ECanaShadow.CANMD.all=ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all  &= maskcode;
    ECanaRegs.CANMD.all=ECanaShadow.CANMD.all;
    ECanaShadow.CANME.all=ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= code;
    ECanaRegs.CANME.all=ECanaShadow.CANME.all;
    EDIS;


    EALLOW;
    ECanaShadow.CANMC.all=ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CDR=0;
    ECanaRegs.CANMC.all=ECanaShadow.CANMC.all;
    ECanaShadow.CANTRS.all=ECanaRegs.CANTRS.all;
    if(s)
        ECanaShadow.CANTRS.all |= code;
    ECanaRegs.CANTRS.all=ECanaShadow.CANTRS.all;
    EDIS;
/**********************************************************************************************************/
    return s;
}

Uint16 Can_dspInterrupt(void){
    int mailNum = ECanaRegs.CANGIF1.bit.MIV1;
    if(Can_dspIntClear(mailNum)){//�����һ�����������������ж�
        Uint8 *p;
        Message msg;
        //��ȡ����ָ��
        p = ((Uint8 *)&ECanaMboxes) + mailNum * sizeof(struct MBOX);
        //�������ݣ� �����������Ƿ�Ϊ���һ֡
        if(Can_dspBasicRecv(&msg, (volatile struct MBOX *)p) == FRAME_ERROR)
            return ERROR_MSG;
        return Can_msgPro(&msg);
    }
    else{
        Can_dspSend();
        return NO_MSG;
    }
}

static int Can_dspIntClear(int mailNum){
    struct ECAN_REGS ECanaShadow;
    Uint32 code = 1 << mailNum;
    //�ж��������Ƿ��ͻ��ǽ�������
    EALLOW;
    ECanaShadow.CANMD.all=ECanaRegs.CANMD.all;
    EDIS;
    if(ECanaShadow.CANMD.all & code){           //����һ����������

        //��λ����RMP��־
        ECanaShadow.CANRMP.all=ECanaRegs.CANRMP.all;
//      ECanaShadow.CANRMP.all |= (1<<mailNum);
        ECanaRegs.CANRMP.all=ECanaShadow.CANRMP.all;
        return 1;

    }else{                                      //����һ����������
        ECanaShadow.CANTA.all=ECanaRegs.CANTA.all;
        ECanaShadow.CANTA.all |= (1<<mailNum);
        ECanaRegs.CANTA.all=ECanaShadow.CANTA.all;
        isMBoxEmpty = 1;
        return 0;
    }
}

static int Can_dspBasicSend(volatile struct MBOX *newMail){
    Message msg;
    if(!Can_msgPop(&msg))
        return 0;

    newMail->MSGCTRL.bit.DLC = msg.len<<1;
    newMail->MSGCTRL.bit.RTR = 0;

    newMail->MSGID.all = msg.cob_id.all;
    newMail->MSGID.bit.IDE = 1;

    newMail->MDL.word.LOW_WORD = msg.data[1];
    newMail->MDL.word.HI_WORD = msg.data[0];
    newMail->MDH.word.LOW_WORD = msg.data[3];
    newMail->MDH.word.HI_WORD = msg.data[2];
    isMBoxEmpty = 0;
    return 1;
}

/* ���ݽ��� -------------------------------------------------------------------------------------------*/
static int Can_dspBasicRecv(Message *msg, volatile struct MBOX *newMail){
    if(!newMail->MSGID.bit.IDE)
        return FRAME_ERROR;
    if(newMail->MSGCTRL.bit.RTR)
        return FRAME_ERROR;
    msg->cob_id.all = newMail->MSGID.all & 0x1FFFFFFF;      //��ȡ��ʶ��
    msg->len = (newMail->MSGCTRL.bit.DLC)>>1;               //�ڴ��ѽ��յ���Byte ת��ΪWord

    msg->data[0] = newMail->MDL.word.HI_WORD;
    msg->data[1] = newMail->MDL.word.LOW_WORD;
    msg->data[2] = newMail->MDH.word.HI_WORD;
    msg->data[3] = newMail->MDH.word.LOW_WORD;

    return FRAME_CORRECT;
}










