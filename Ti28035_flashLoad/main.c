#include"boot.h"
#include"candsp.h"
#include"bootApp.h"
#include"crc.h"

int main(void) {
    Uint8 reson = isBootLoadNeed();
    Uint8 dspState = 0;
    frameNum = 0;

    //step1: Judge if bootloader needed??
    if(!reson){  //程序不需要升级
        JumpToApp();
//        asm(" LB #0x3EBEDA"); //这是.out文件生成的pc指针入口地址，这个入口地址使得程序在运行一次main()之前的函数！！！！
    }

    //step2: if do need
    DSP_initBasic();
    BootApp_init();
    BootApp_setDspSeries("DSP28035");
    BootApp_setDspTips("I Need DSP28035.a00!");
    BootApp_setDspVersion(1000);
    BootApp_setDspFunction(2);
    BootApp_setDspState(0);

    DSP_initFlashAPI();

    //step3: Clear Sector(B/C/D/E/F/G/H)
    eraseAppFlashSectors();

//    int i = 0, j = 0;
//    for(i = 0; i < 1000; i++){
//        for(j = 0; j < 2000; j++)
//            ;
//
//    }

    if(reson == BOOT_NO_APP){
        dspState = BOOT_WAITTING;
        bootState = BOOT_SM_IDLE;
        BootApp_setDspState(dspState);
    }

    if(reson == BOOT_APP_REQ){
        dspState = BOOT_UPDATING;
        bootState = BOOT_SM_LOADING;
        BootApp_replay(0);
    }

    for(;;){
        BootApp_SM();
    }
}






































