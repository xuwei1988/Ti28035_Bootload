#include"boot.h"
#include"bootApp.h"

/* 局部函数声明， 在.c文件内部使用 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
#if(BOOT_MOD == SCIBoot)
static interrupt void sci_tx_isr(void);
static interrupt void sci_rx_isr(void);
#endif
#if(BOOT_MOD == CANBoot)
static interrupt void can1_isr(void);
#endif
static interrupt void cpu_timer0_isr(void);


/* 外部函数实现， 在函数外部调用 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
/* BootLoad 是否需要启动判断函数 －－*/
Uint16 isBootLoadNeed(void){
    Uint16 *appFlagAdr = (Uint16 *)APP_FLAG_ADD;
    //step1: 判断是否已存在APP
    if(*appFlagAdr == APP_FLAG){
        if(bootData.isNeed == REQURE_FLAG)    //bootData.isNeed是被应用程序置位
            return BOOT_APP_REQ;
        else
            return 0;

    }else{
        return BOOT_NO_APP;
    }
}

void BootComplete(void){
    bootData.isNeed = 0x00000000;
}

/* 擦除应用程序所占用的flash空间*/
void eraseAppFlashSectors(void){
    Uint16 s;
    FLASH_ST FlashStatus;
    INT_LOCK();
    s = Flash_Erase((SECTORB|SECTORC|SECTORD|SECTORE|SECTORF|SECTORG), &FlashStatus);
    INT_UNLOCK();
    if(s != STATUS_SUCCESS){
        error(s);
    }
}

/* 内存copy函数 －－*/
void memcopyEx(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr){
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;
    }
    return;
}


/* 解锁函数 －－*/
Uint16 CSM_unLock(){
    volatile Uint16 temp;

    // Load the key registers with the current password
    // These are defined in Example_Flash2803x_CsmKeys.asm

    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;

    // Perform a dummy read of the password locations
    // if they match the key values, the CSM will unlock

    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;

    // If the CSM unlocked, return succes, otherwise return
    // failure.
    if ( (CsmRegs.CSMSCR.all & 0x0001) == 0) return STATUS_SUCCESS;
    else return STATUS_FAIL_CSM_LOCKED;
}

/* Flash API调用错误入口函数 －－*/
#pragma CODE_SECTION(error, "ramfuncs");
void error(Uint16 Status){
    asm(" ESTOP0");
    asm(" SB 0, UNC");
}

/* Flash API初始化函数 －－*/
void DSP_initFlashAPI(void){
    Uint16 Status;
    //step1:  Device_cal()
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;
    EDIS;

    //step2:
    // Assuming PLLSTS[CLKINDIV] = 0 (default on XRSn).  If it is not
    // 0, then the PLLCR cannot be written to.
    // Make sure the PLL is not running in limp mode
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
    {
        if (SysCtrlRegs.PLLCR.bit.DIV != PLLCR_VALUE)
        {
           EALLOW;
           // Before setting PLLCR turn off missing clock detect
           SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
           SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;  // 1/4
           SysCtrlRegs.PLLCR.bit.DIV = PLLCR_VALUE;
           EDIS;

           // Wait for PLL to lock.
           // During this time the CPU will switch to OSCCLK/2 until
           // the PLL is stable.  Once the PLL is stable the CPU will
           // switch to the new PLL value.
           //
           // This time-to-lock is monitored by a PLL lock counter.
           //
           // The watchdog should be disabled before this loop, or fed within
           // the loop.

           EALLOW;
           SysCtrlRegs.WDCR= 0x0068;
           EDIS;

           // Wait for the PLL lock bit to be set.
           // Note this bit is not available on 281x devices.  For those devices
           // use a software loop to perform the required count.

           while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) { }

           EALLOW;
           SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;  // 1/2
           SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
           EDIS;
        }
    }

    // If the PLL is in limp mode, shut the system down
    else
    {
       // Replace this line with a call to an appropriate
       // SystemShutdown(); function.
       asm("        ESTOP0");
    }

    //step3: Copy API Functions into SARAM
    //For Piccolo  NO Need!!!!!!
    memcopyEx(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

    //step5: Init ScaleFactor
    EALLOW;
    Flash_CPUScaleFactor = SCALE_FACTOR;
    EDIS;

    EALLOW;
    Flash_CallbackPtr = (void (*)(void)) 0;
    EDIS;

    //step4: Unlock the CSM
    Status = CSM_unLock();
    if(Status != STATUS_SUCCESS){
        error(Status);
    }
}

/* DSP整体初始化函数 －－*/
void DSP_initBasic(void){
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;

    /* 这里允许中断 －－*/
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    EALLOW;
    /* Timer0 中断 －－*/
    PieVectTable.TINT0 = &cpu_timer0_isr;
#if(BOOT_MOD == SCIBoot)
    /* Sci 中断 －－*/
    PieVectTable.SCITXINTA = &sci_tx_isr;
    PieVectTable.SCIRXINTA = &sci_rx_isr;
#endif

#if(BOOT_MOD == CANBoot)
    /* Can中断 －－*/
    PieVectTable.ECAN1INTA = &can1_isr;
#endif
    EDIS;
    InitCpuTimers();

#if(BOOT_MOD == SCIBoot)
    /* SCI 相关的初始化 －－*/
    InitSciGpio();
    InitSci();
#endif

#if(BOOT_MOD == CANBoot)
    /* CAN 相关的初始化 －－*/
    can_dspInit();
#endif

    /* 通用内存“族”初始化*/
    bzero((Uint8 *)clusterbuffSto,sizeof(clusterbuffSto));
    cluster_PoolInit(clusterbuffSto, sizeof(clusterbuffSto), sizeof(clusterbuffSto[0]));

    ConfigCpuTimer(&CpuTimer0, 60, 2000);
    CpuTimer0Regs.TCR.all = 0x4001;

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
#if(BOOT_MOD == SCIBoot)
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
#endif

#if(BOOT_MOD == CANBoot)
    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
#endif
    IER |= M_INT1;
    IER |= M_INT9;
    EINT;
    ERTM;
}


/* 局部函数的实现 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
#if(BOOT_MOD == SCIBoot)
/* SCI中断处理函数 －－*/
/* SCI发送中断 */
static interrupt void sci_tx_isr(void){
//
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/* SCI接收中断 */
static interrupt void sci_rx_isr(void){

    Uint16 a;
    a = SciaRegs.SCIRXBUF.all;
//

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
#endif


#if(BOOT_MOD == CANBoot)
/* CAN中断处理函数 －－*/
static interrupt void can1_isr(void){
    if(Can_dspInterrupt() == COMPLETE_MSG){
        if(receiveImfor.sNode.type == MONITOR){
            if(receiveImfor.protocol == BOOTLOAD_PROTOCOL){
                BootApp_recievePro((BootFrame *)receiveImfor.data);
            }
        }
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
#endif

/* 20ms周期中断需要执行的任务 －－－－－－－－－－－－－－－－－－－－*/
static interrupt void cpu_timer0_isr(void){

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}














