#ifndef CANDSP_H_
#define CANDSP_H_
#include"canport.h"
#include"candriver.h"

#define FRAME_CORRECT           0
#define FRAME_ERROR             1

void can_dspInit(void);
Uint16 Can_dspInterrupt(void);
Uint8 Can_dspSend(void);
Uint16 Can_post(SendImfor *sendImfor, void *pMsg);

#endif /* CANDSP_H_ */
