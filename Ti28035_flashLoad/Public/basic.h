#ifndef PUBLIC_BASIC_H_
#define PUBLIC_BASIC_H_

#include"DSP2803x_Device.h"
#include"mempool.h"

#define INT_LOCK()                  asm("    SETC INTM ")
#define INT_UNLOCK()                asm("    CLRC INTM ")

#define DIM(array_) (sizeof(array_) / sizeof(array_[0]))

/* 定义内存块大小 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
#define BLOCK_SIZE                  260
#define BLOCK_DATA_SIZE             (BLOCK_SIZE - 4)
#define BLOCK_NUM                   12

typedef struct MemBlockTag{
    Uint16 a[BLOCK_SIZE];
}MemBlock;


char * NumToString(int n, char *str);
void bzero(Uint8 *ptr, Uint16 len);
void mem_set(Uint8 *ptr, Uint8 data, Uint16 len);


void cluster_PoolInit(void *poolSto, Uint16 poolSize, Uint16 blockSize);
/* 全局的内存块 ------------------------------------------------------------*/
extern MemBlock clusterbuffSto[BLOCK_NUM];
extern MemPool clusterPool;


#endif /* PUBLIC_BASIC_H_ */
