#ifndef __MYAC_H
#define __MYAC_H
#ifdef __cplusplus
		extern "C" {
#endif 			
#include "stdint.h"	

			
			
typedef  enum{
	AC_ERR=-1,
	AC_OK,
}_myAc;			
			
#define LIST_MAX_NUM	10
typedef struct{
	uint8_t num;
	char *d[LIST_MAX_NUM];
}_stringList;




int Ac_pf(char *format,...);
			
uint16_t Crc16_ModbusFarst(uint8_t *puchMsg,uint16_t usDataLen);	
uint16_t Crc16_Modbus(uint8_t *puchMsg,uint16_t usDataLen);
uint8_t U8checkSum(uint8_t *a,uint8_t len);	
_myAc Bin2HexStr(uint32_t bin,uint8_t bits,char* ret);			
_myAc HexStr2Bin(char *a ,uint8_t bits,void *ret);
_stringList *mtStrDiv(char *a, char *b);
void FreeStringList(_stringList *list);
/**/			
			
#ifdef __cplusplus
				}
    #endif
#endif
