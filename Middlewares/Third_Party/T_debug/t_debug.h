#ifndef __T_DEBUG_H
#define __T_DEBUG_H

#include "stdint.h"


typedef struct{
	char name[16];
	int (*fun)(char *f,...);
}_cmd;

#define DEBUG_CONSOLE_SCANF_MAX_LOG_LEN 40
#define DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN 128
#define Ac_log(...) DbgConsole_Printf(__VA_ARGS__)

void LOG_Init(void);
void shell(void);
int DbgConsole_Printf(const char *fmt_s, ...);
int DbgConsole_Scanf(char *fmt_ptr, ...);

void t_recv_data(uint8_t *data,uint32_t len);
void t_send_data(uint8_t *data,uint32_t len);




#endif
