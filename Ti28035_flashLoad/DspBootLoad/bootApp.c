#include "bootApp.h"
#include"Flash2803x_API_Library.h"





Uint32 crc_table[256];
static void init_crc_table(void);
static Uint32 crc32(Uint32 crc, unsigned char *buffer, unsigned int size);

static void init_crc_table(void) {
    Uint32 c;
    Uint32 i, j;

    for (i = 0; i < 256; i++) {
        c = (Uint32)i;
        for (j = 0; j < 8; j++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[i] = c;
    }
}


static Uint32 crc32(Uint32 crc, unsigned char *buffer, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
    }
    //return (crc ^ 0xffffffff);
    return (crc);
}










/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 鍏ㄥ眬鍙橀噺 ----------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/

Uint8 bootState = 0;                                        //bootload鍒濆鐘舵�
#pragma DATA_SECTION(bootData, "GlobalData");
BootData bootData;

/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 灞�儴鍙橀噺 ----------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/
static DSPImfor     dspImfor;                               //DSP鑺墖鑷韩淇℃伅
//搴旂敤绋嬪簭鎵�渶鍙橀噺
static Uint8        isDspConnected = 0;                     //DSP鑺墖鏄惁宸茬粡杩炴帴
static Uint8        isDspRequired = 0;                      //DSP鑺墖鏄惁琚姹傚崌绾�

//boot 绋嬪簭鎵�渶鍙橀噺
static BurnImfor    burnImfor;                              //DSP褰撳墠鐑у叆鏁版嵁鐨勪俊鎭�
static FLASH_ST FlashStatus;

/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 灞�儴鍑芥暟澹版槑-------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/
static void Boot_DetectPro(BootFrame *bf);
static void Boot_RequirePro(BootFrame *bf);
static void Boot_DataPro(BootFrame *bf);
static void Boot_endPro(BootFrame *bf);

static Uint16 Boot_addVerify(Uint16 len);

/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 鍏ㄥ眬鍑芥暟瀹炵幇-------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/

/* BootLoad 妯″潡鍒濆鍖栧嚱鏁�-------------------------------------------------*/
void BootApp_init(void){
    //step1: 灏嗚繛鎺ヤ俊鎭拰璇锋眰淇℃伅娓�
    isDspConnected = 0;
    isDspRequired = 0;
    //step2: 鎶婃帴鏀舵暟鎹殑鏁版嵁鍧椾俊鎭竻0
    bzero((Uint8 *)&burnImfor, sizeof(burnImfor));
    //step3: 娓呯┖鑺墖鍩烘湰淇℃伅
    bzero((Uint8 *)&dspImfor, sizeof(dspImfor));

    init_crc_table();
}


/* DSPImfor 鐩稿叧鐨勫嚱鏁�----------------------------------------------------*/
Uint8 BootApp_setDspSeries(char *s){            //璁剧疆DSP绫诲瀷
    int i = 0;
    int len = sizeof(dspImfor.DSPSeries);
    while(*s){
        dspImfor.DSPSeries[i++] = *s++;
        if(i >= len - 1)
            return BOOTAPP_ERR;
    }
    dspImfor.DSPSeries[i] = '\0';
    return BOOTAPP_OK;
}

Uint8 BootApp_setDspTips(char *s){
    int i = 0;
    int len = sizeof(dspImfor.tips);
    while(*s){
        dspImfor.tips[i++] = *s++;
        if(i >= len - 1)
            return BOOTAPP_ERR;
    }
    dspImfor.tips[i] = '\0';
    return BOOTAPP_OK;
}

void BootApp_setDspVersion(Uint32 version){
    dspImfor.softVersion = version;
}

void BootApp_setDspFunction(Uint8 function){
    dspImfor.function = function;
}

void BootApp_setDspState(Uint8 isbusy){
    dspImfor.isBusy = isbusy;
}


/* 鑺墖Bootload甯歌澶勭悊鍑芥暟 -------------------------------------------------*/
void BootApp_handle(void){
    if(isDspConnected && isDspRequired){
        isDspConnected = 0;
        isDspRequired = 0;
        bootState = BOOT_SM_LOADING;
        BootApp_replay(0);
    }
}


void BootApp_SM(void){
    switch(bootState){
        case BOOT_SM_IDLE:{
            BootApp_handle();
            break;
        }
        case BOOT_SM_LOADING:{
            //鍙戦�涓�釜淇″彿缁檇sp璁╁叾杩涘叆鍗囩骇鐣岄潰

            break;
        }
    }

}

/* 鑺墖鍦ㄦ帴鏀跺埌bootload鐩稿叧娑堟伅涔嬪悗鐨勬秷鎭鐞�--------------------------------*/
void BootApp_recievePro(BootFrame *bf){
    switch(bf->type){
        case BOOT_MCU_DETECT:{                  //鏀跺埌MCU鏌ヨ甯�
            Boot_DetectPro(bf);
            break;
        }
        case BOOT_MCU_REQUIRE:{                 //鏀跺埌MCU鍗囩骇鍛戒护
            Boot_RequirePro(bf);
            break;
        }
        case BOOT_MCU_DATA:{                    //鏀跺埌MCU鐪熸鐨勫崌绾ф暟鎹�
            Boot_DataPro(bf);
            break;
        }
        case BOOT_MCU_END:{                     //鏀跺埌MCU鍗囩骇缁撴潫鏍囧織
            Boot_endPro(bf);
            break;
        }
        default:
            break;

    }

}

/* 鑺墖鍦ㄦ帴鏀跺埌MCU浼犻�鐨勭湡姝ｅ崌绾ф暟鎹椂鍊欑殑鍥炲簲 --------------------------------*/
Uint16 BootApp_replay(Uint16 res){
    BootFrame bf;
    bf.type = BOOT_DSP_REPLY;
    bf.len = 1;      //鍙敤浜嗕竴涓暟鎹綅锛屼繚瀛樺彂閫佹鏁�
    bf.data[0] = res;
    SendImfor imfor;
    Can_CommandSendToMonitor(&imfor, 0, BOOTLOAD_PROTOCOL, (Uint32)(&bf.data[bf.len]) - (Uint32)(&bf));
    return Can_post(&imfor, &bf);
}

/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 灞�儴鍑芥暟瀹炵幇-------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/
static void Boot_DetectPro(BootFrame *bf){
    BootFrame sf;
    SendImfor imfor;
    //step0: 鍏堝啓濂藉彂缁橫CU甯х殑绫诲瀷
    sf.type = BOOT_DSP_RECV;
    //step1: 鍏堝垽鏂槸鍚﹂渶瑕佸彂閫佸叏閮―SP淇℃伅
    if(bf->data[0] == BOOT_DETECT_ALL){
        isDspConnected = 1;                             //DSP宸茬粡鍜孧CU鎴愬姛杩炴帴
        sf.len = sizeof(dspImfor);
        memcpy(sf.data, &dspImfor, sf.len);
    }
    if(bf->data[0] == BOOT_DETECT_STATE){
        sf.len = 1;
        sf.data[0] = dspImfor.isBusy;
    }

    Can_CommandSendToMonitor(&imfor, 0, BOOTLOAD_PROTOCOL, (Uint32)(&sf.data[sf.len]) - (Uint32)(&sf));
    Can_post(&imfor, &sf);
}

static void Boot_RequirePro(BootFrame *bf){
    isDspRequired = 1;
}

Uint32 chksum = 0;
Uint32 chkres = 0xFFFFFFFF;
static Uint8 addZeroCnt = 0;
Uint32 frameNum = 0;
#pragma DATA_SECTION(frameNum, "GlobalData1");
static void Boot_DataPro(BootFrame *bf){
//    Uint8 res = 0;
    Uint16 *pdata;
    Uint16 len = 0;
    Uint16 res = 0;
    if(1){

        frameNum++;

        /* 2017.10.20 crc32*/
        Uint8 crcBuff[24];
        Uint8 i = 0;

        if(frameNum == 1){
            for(i = 0; i < 2; i++){
                crcBuff[2*i] = (bf->data[i]>>8);
                crcBuff[2*i+1] = (bf->data[i] & 0x00FF);
            }
            chksum = (((Uint32)crcBuff[0])<<24) + (((Uint32)crcBuff[1])<<16) + (((Uint32)crcBuff[2])<<8) + crcBuff[3];
        }


        //step2: 鏁版嵁澶勭悊
        if(frameNum == 3){ //杩欐槸绗竴甯�

            /*2017.10.20 */
            for(i = 0; i < 12; i++){
                crcBuff[2*i] = (bf->data[i]>>8);
                crcBuff[2*i+1] = (bf->data[i] & 0x00FF);
            }
            chkres = crc32(chkres, (unsigned char *)crcBuff, 24);

            burnImfor.blkTotSize = bf->data[11];
            burnImfor.blkLeftSize = bf->data[11];
            len = 0;
        }

        if(frameNum == 4){
            for(i = 0; i < 2; i++){
                crcBuff[2*i] = (bf->data[i]>>8);
                crcBuff[2*i+1] = (bf->data[i] & 0x00FF);
            }
            chkres = crc32(chkres, (unsigned char *)crcBuff, 4);

            burnImfor.blkBeginAdd = bf->data[0];
            burnImfor.blkBeginAdd <<= 16;
            burnImfor.blkBeginAdd += bf->data[1];
            burnImfor.blkCurrAdd = burnImfor.blkBeginAdd;
            pdata = &(bf->data[2]);
            if(burnImfor.blkLeftSize < bf->len - 2)
                len = burnImfor.blkLeftSize;
            else
                len = bf->len - 2;
        }

        if(frameNum > 4){
            if(burnImfor.blkLeftSize == 0){
                burnImfor.blkTotSize = bf->data[0];
                burnImfor.blkLeftSize = bf->data[0];

                burnImfor.blkBeginAdd = bf->data[1];
                burnImfor.blkBeginAdd <<= 16;
                burnImfor.blkBeginAdd += bf->data[2];
                burnImfor.blkCurrAdd = burnImfor.blkBeginAdd;

                /*2017.10.20 */
                if(burnImfor.blkBeginAdd){
                    for(i = 0; i < 3; i++){
                        crcBuff[2*i] = (bf->data[i]>>8);
                        crcBuff[2*i+1] = (bf->data[i] & 0x00FF);
                    }
                    chkres = crc32(chkres, (unsigned char *)crcBuff, 6);

                }else{
                    if(addZeroCnt++ == 0){
                        crcBuff[0] = 0x00;
                        crcBuff[1] = 0x00;
                        chkres = crc32(chkres, (unsigned char *)crcBuff, 2);
                        if((chkres + chksum) != 0xFFFFFFFF){
                            addZeroCnt = 0;
                            res = 4;
                        }
                    }
                }

                if(burnImfor.blkBeginAdd == APP_FLAG_ADD){
                       /* 2017.09.12 ---------------------------------------------------------------------------------------------*/
                       crcBuff[0] = 0xAA;
                       crcBuff[1] = 0xAA;
                       chkres = crc32(chkres, (unsigned char *)crcBuff, 2);
                       /* end ----------------------------------------------------------------------------------------------------*/

                       burnImfor.blkLeftSize = 0;
                       BootApp_replay(0);
                       return;
                   }



                pdata = &(bf->data[3]);
                if(burnImfor.blkLeftSize < bf->len - 3)
                    len = burnImfor.blkLeftSize;
                else
                    len = bf->len - 3;
            }else{
                pdata = &(bf->data[0]);
                if(burnImfor.blkLeftSize < bf->len)
                    len = burnImfor.blkLeftSize;
                else
                    len = bf->len;
            }

        }

        //鎷疯礉鍒癴lash
        if(len > 0){
            /* 2017.10.20 */
            for(i = 0; i < len; i++){
                crcBuff[2*i] = (pdata[i]>>8);
                crcBuff[2*i+1] = (pdata[i] & 0x00FF);
            }
            chkres = crc32(chkres, (unsigned char *)crcBuff, len*2);


            if(Boot_addVerify(len)){
                res = 7;

            }else{
                INT_LOCK();
                res = Flash_Program((Uint16 *)burnImfor.blkCurrAdd, pdata, len, &FlashStatus);
                INT_UNLOCK();

                INT_LOCK();
                res = Flash_Verify((Uint16 *)burnImfor.blkCurrAdd, pdata, len, &FlashStatus);
                INT_UNLOCK();
            }

        }
        burnImfor.blkLeftSize -= len;
        if(burnImfor.blkLeftSize){
            burnImfor.blkCurrAdd += len;
        }

    }


    if(res){
        Uint16 a = 0;
        a++;

    }


    BootApp_replay(res);
}

static void Boot_endPro(BootFrame *bf){
    frameNum = 0;
    bootData.isNeed = 0x00000000;                       //娓呯┖boot鍗囩骇鏍囧織
    addZeroCnt = 0;
    Uint16 data[2] = {0xAAAA};
    Uint16 res = 0;

    INT_LOCK();
    res = Flash_Program((Uint16 *)APP_FLAG_ADD, data, 1, &FlashStatus);
    INT_UNLOCK();

    INT_LOCK();
    res = Flash_Verify((Uint16 *)APP_FLAG_ADD, data, 1, &FlashStatus);
    INT_UNLOCK();

    if(res)
        BootApp_replay(res);


    JumpToApp();
}



static Uint16 Boot_addVerify(Uint16 len){
    if(burnImfor.blkCurrAdd < 0x3E8000 || (burnImfor.blkCurrAdd + len) > 0x3F5FFF)
        return 1;
    return 0;

}



















