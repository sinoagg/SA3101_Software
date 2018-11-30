//==============================================================================
//
// Title:		Uart.h
// Purpose:		A short description of the interface.
//
// Created on:	18/9/27 at 11:44:29 by 斌 冯.
// Copyright:	浙江鼎奕科技发展有限公司. All Rights Reserved.
//
//==============================================================================

#ifndef __Uart_H__
#define __Uart_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables
extern int TimerID; 
extern int logFlag;  
extern int rowIndex; 
extern int rowIndex2;
extern int rowIndex3;
//==============================================================================
// Global functions
void SubString(char* str1,char* str2,int start,int length);
void PrintEdition(char USART_RX_Buffer1[20]);
int CheckPortStatus(unsigned char portNumber, unsigned char uartRxLen, void (*pFunc)(int, int, void*));
int CheckOpenCom(unsigned char* measureComPort,unsigned char* controlComPort,unsigned char* measure_Uart_Flag,unsigned char* control_Uart_Flag);
void CheckCloseCom(int SA3102_UART_Status,unsigned char measureComPort,unsigned char controlComPort);

void ReadSerialPort(unsigned char* measureComPort,unsigned char* controlComPort);
void MeasureDeviceQuery(unsigned char* measureComPort);
void CtrlDeviceQuery(unsigned char* controlComPort);

int CheckMeasurePort(unsigned char* measureComPort,unsigned char i);
int CheckControlPort(unsigned char* controlComPort,unsigned char i);

void CVICALLBACK ComCallback(int portNumber ,int eventMask, void * callbackData);
void CVICALLBACK CtrlComCallback(int portNumber, int eventMask, void * callbackData);
int CVICALLBACK TimerCallback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Uart_H__ */
