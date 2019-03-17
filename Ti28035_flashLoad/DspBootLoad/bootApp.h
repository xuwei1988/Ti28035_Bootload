#ifndef DSPBOOTLOAD_BOOTAPPEX_H_
#define DSPBOOTLOAD_BOOTAPPEX_H_

#include "basic.h"
#include "boot.h"


/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 鐢ㄥ埌鐨勭浉鍏冲畯 ------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/

/* BootLoad 杩炴帴鏂瑰紡浠ｅ彿 -----------------------------------------------------*/
#define BOOT_SCI                        0x01
#define BOOT_CAN                        0x02
#define BOOT_SPI                        0x03
#define BOOT_INTERNET                   0x04

/* BootLoad 鏂瑰紡閫夋嫨 --------------------------------------------------------*/
#define BOOT_MODE                       BOOT_CAN

/* 鍗囩骇鎵�渶鐨勫ご鏂囦欢 ---------------------------------------------------------*/
#if(BOOT_MODE == BOOT_SCI)

#endif

#if(BOOT_MODE == BOOT_CAN)
#include"candsp.h"
#include"canmsgblock.h"
#endif

#if(BOOT_MODE == BOOT_SPI)

#endif

#if(BOOT_MODE == BOOT_INTERNET)

#endif

/* 搴旂敤绋嬪簭鎵�渶鐨勫畾涔�---------------------------------------------------------*/
#define BOOTAPP_ERR                     1
#define BOOTAPP_OK                      0

#define BOOTAPP_DSPFUNCTION_MASTER      0
#define BOOTAPP_DSPFUNCTION_INV         1               //閫嗗彉
#define BOOTAPP_DSPFUNCTION_REC         2               //鏁存祦


/* DSP鑺墖鐘舵� --------------------------------------------------------------*/
#define BOOT_WORKING                    0               //妯″潡绌洪棽
#define BOOT_FREE                       1               //妯″潡宸ヤ綔
#define BOOT_WAITTING                   2               //绛夊緟鍗囩骇
#define BOOT_UPDATING                   3               //鍗囩骇涓�

/* BootLoad 鐘舵�鏈轰唬鍙�------------------------------------------------------*/
#define BOOT_SM_IDLE                    0x00            //绌洪棽鐘舵�
#define BOOT_SM_LOADING                 0x01            //鍗囩骇涓�

#define BOOT_NO_APP                     0x01
#define BOOT_APP_REQ                    0x02

/* BootLoad 鍗忚鐮�----------------------------------------------------------*/
#define BOOTLOAD_PROTOCOL               0x09                       //Bootload绫诲瀷鏁版嵁鐨勫崗璁爜

#define BOOT_MCU_REQUIRE                1                           //MCU -> DSP 锛堣姹侱SP鍗囩骇锛�
#define BOOT_MCU_DATA                   2                           //MCU -> DSP 锛圡CU浼犻�缁橠SP鐨勭湡姝ｆ暟鎹級
#define BOOT_DSP_REPLY                  3                           //DSP -> MCU 锛圖SP瀵规帴鏀跺埌BOOT_MCU_DATA鐨勫簲绛旓級
#define BOOT_MCU_END                    4                           //MCU -> DSP 锛圡CU閫氱煡DSP鏁版嵁鍙戦�瀹屾瘯锛�
#define BOOT_DSP_RECV                   5                           //DSP -> MCU 锛圖SP閫氱煡MCU宸茬粡鎺ユ敹鍒癇OOT_MCU_DETECT 娑堟伅锛�
#define BOOT_MCU_DETECT                 6                           //MCU -> DSP 锛圡CU鍙戦�鏌ヨ甯э紝妫�祴DSP鐨勫瓨鍦級

//BOOT_DSP_REPLY 涓嬬殑瀛愬崗璁爜
#define BOOT_REPLY_OK                   0                           //MCU浼犻�缁橠SP鐨勭湡姝ｆ暟鎹姝ｅ父澶勭悊
#define BOOT_REPLY_DATAERR              1                           //MCU浼犻�缁橠SP鐨勬暟鎹牎楠岄敊璇�
//......
//......
//......

//BOOT_MCU_DETECT 涓嬬殑瀛愬崗璁爜
#define BOOT_DETECT_ALL                 1                           //璇锋眰DSP鐨勬墍鏈夋暟鎹�锛堝湪棣栨杩炴帴涓娇鐢級
#define BOOT_DETECT_STATE               2                           //璇锋眰DSP鐨勭姸鎬佹暟鎹�锛堝湪杩炴帴寤虹珛鍚庝娇鐢級


//Bootload绋嬪簭璺宠浆鎸囦护
#define BOOT_NEED_FLAG                  0x55555555
#define JumpToBoot()                    asm(" LB #0x3F7FF6")
#define JumpToApp()                     asm(" LB #0x3F5FFD")


/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 鐢ㄥ埌鐨勭浉鍏虫槸鏁版嵁缁撴瀯-----------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/

/* 搴旂敤绋嬪簭鍜孊oot绋嬪簭鍏敤鐨勭▼搴忓崌绾ф爣蹇椾綅 -------------------------------------*/
/* 娉� 姝ゆ爣蹇椾綅鍦ㄥ簲鐢ㄧ▼搴忎腑缃綅锛孊oot绋嬪簭閫氳繃妫�獙姝ゆ爣蹇椾綅鐨勫�鏉ュ喅瀹氭槸鍚﹀崌绾�*/
typedef struct{
    Uint32 isNeed;
}BootData;

/* boot鏁版嵁甯т俊鎭�------------------------------------------------------------*/
/* 娉� 姝ゆ暟鎹抚鏄疢CU - DSP涔嬮棿浼犺緭鐨勫簳灞傛暟鎹被鍨�*/
typedef struct{
    Uint16 type;
    Uint16 len;
    Uint16 data[100];
}BootFrame;

/* DSP杩斿洖缁橫CU鐨勮嚜韬俊鎭�----------------------------------------------------*/
/* 娉� 姝や俊鎭敤浜庡洖澶岯OOT_MCU_DETECT锛屾潵鏍囪DSP鐨勮韩浠戒俊鎭�*/
typedef struct{
    char DSPSeries[12];
    char tips[25];
    Uint32 softVersion;
    Uint8 function;
    Uint8 isBusy;
}DSPImfor;

/* boot绋嬪簭鎵�壒鏈夌殑鏁版嵁缁撴瀯 --------------------------------------------------*/
/* boot褰撳墠澶勭悊鐨刣ata鏁版嵁淇℃伅 */
typedef struct{
    Uint32 blkBeginAdd;             //瑕佺儳鍐欑殑flash鍧楃殑璧峰鍦板潃
    Uint32 blkCurrAdd;              //瑕佺儳鍐欑殑flash鍧楃殑褰撳墠鐑у啓鍒扮殑鍦板潃
    Uint32 blkTotSize;              //鐑у啓鍧楃殑澶у皬
    Uint32 blkLeftSize;             //鍧楀墿浣欏ぇ灏�
    Uint32 blkLineNume;             //褰撳墠鍧楃殑琛屾暟
}BurnImfor;

/*  --------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 鐢ㄥ埌鐨勫叏灞�嚱鏁�----------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------------------------------------------*/
/* BootLoad 妯″潡鍒濆鍖栧嚱鏁�-------------------------------------------------*/
void BootApp_init(void);
/* 鑺墖Bootload甯歌澶勭悊鍑芥暟 -------------------------------------------------*/
void BootApp_handle(void);
void BootApp_SM(void);
/* 鑺墖鍦ㄦ帴鏀跺埌bootload鐩稿叧娑堟伅涔嬪悗鐨勬秷鎭鐞�--------------------------------*/
void BootApp_recievePro(BootFrame *bf);
/* 鑺墖鍦ㄦ帴鏀跺埌MCU浼犻�鐨勭湡姝ｅ崌绾ф暟鎹椂鍊欑殑鍥炲簲 -------------------------------*/
Uint16 BootApp_replay(Uint16 res);

/* 鑺墖鍩烘湰淇℃伅褰曞叆鍑芥暟------------------------------------------------------*/
Uint8 BootApp_setDspSeries(char *s);
Uint8 BootApp_setDspTips(char *s);
void BootApp_setDspVersion(Uint32 version);
void BootApp_setDspFunction(Uint8 function);
void BootApp_setDspState(Uint8 isbusy);


extern BootData bootData;
extern Uint8 bootState;

extern Uint32 frameNum;

#endif /* DSPBOOTLOAD_BOOTAPPEX_H_ */
