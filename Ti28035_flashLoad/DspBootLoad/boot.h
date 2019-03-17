#ifndef DSPBOOTLOAD_BOOT_H_
#define DSPBOOTLOAD_BOOT_H_

#define CANBoot         0x1
#define SCIBoot         0x2

#define BOOT_MOD        CANBoot
#define APP_FLAG_ADD    0x3F5FFF
#define APP_FLAG        0xAAAA
#define REQURE_FLAG     0x55555555

#define JumpToApp()     asm(" LB #0x3F5FFD")



#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef unsigned char   Uint8;
typedef int             int16;
typedef long            int32;
typedef unsigned int    Uint16;
typedef unsigned long   Uint32;
typedef float           float32;
typedef long double     float64;
#endif

#define Device_cal      (void   (*)(void))0x3D7C80
#define PLLCR_VALUE     0x000C

#include"DSP2803x_Device.h"
#include"DSP2803x_GlobalPrototypes.h"
#include"Flash2803x_API_Library.h"
#include"candsp.h"

extern Uint16 isBootLoadNeed(void);
extern void BootComplete(void);
void eraseAppFlashSectors(void);
extern void memcopyEx(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
extern Uint16 CSM_unLock();
extern void error(Uint16 Status);
extern void DSP_initFlashAPI(void);
extern void DSP_initBasic(void);

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;


#endif /* DSPBOOTLOAD_BOOT_H_ */
