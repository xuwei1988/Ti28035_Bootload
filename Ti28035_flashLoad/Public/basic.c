#include<limits.h>
#include<mempool.h>
#include"basic.h"

MemBlock clusterbuffSto[BLOCK_NUM];
MemPool clusterPool;


void cluster_PoolInit(void *poolSto, Uint16 poolSize, Uint16 blockSize){
    MemPool_init(&clusterPool, poolSto, poolSize, blockSize);
}


char * NumToString(int n, char *str){
    Uint16 m;
    if(n == INT_MIN)
        m = INT_MAX + 1U;
    else if(n < 0)
        m = -n;
    else
        m = n;
    do
        *--str = m%10 + '0';
    while((m /= 10) > 0);
    if(n < 0)
        *--str = '-';
    return str;
}


/*2017.04.15添加 AsciiHex->hex转换 ----------------------------------------*/
Uint16 outputAsciiToHex(char *str, Uint16 *pdata, Uint16 len){
    char c = '\0';
    Uint8 num;
    Uint16 cnt = 0;
    Uint16 i = 0;

    //step1: 清内存
    for(i = 0; i < len; i++){
        pdata[i] = 0;
    }
    //step2: 开始转化
    while(c = *str++){          //未到达字符串的结尾
        if(++cnt == 6){
            pdata++;
            cnt = 0;
            continue;
        }
        if(cnt == 3)
            continue;
        if(c <= '9' && c >= '0'){
            num = c - '0';
        }
        if(c <= 'F' && c >= 'A'){
            num = c - 'A' + 10;
        }
        if(c == 0x2 || c == 0x3)
            return 0;
        if(c == 0xD || c == 0xA)
            return 1;
        if(cnt == 1)
            *pdata += num << 12;
        if(cnt == 2)
            *pdata += num << 8;
        if(cnt == 4)
            *pdata += num << 4;
        if(cnt == 5)
            *pdata += num;
    }
    return 0;
}

void bzero(Uint8 *ptr, Uint16 len){
    while (len-- != (Uint16)0) {
        *ptr++ = (Uint8)0;
    }
}

/* ���ַ����ϴ����� ����������������������������������������*/
void mem_set(Uint8 *ptr, Uint8 data, Uint16 len){
    while(len-- != (Uint16)0){
        *ptr++ = data;
    }
}


