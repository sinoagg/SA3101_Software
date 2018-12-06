//==============================================================================
//
// Title:		main.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/8/11 at 10:57:42 by �� ��.
// Copyright:	���ɹ������ѧ. All Rights Reserved.
//
//==============================================================================

#ifndef __main_H__
#define __main_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
#define MEASURE_UART_RX_LEN 24
#define	CONTROL_UART_RX_LEN 15

//==============================================================================
// Types

//==============================================================================
// External variables
static unsigned char measUartTxBuf1[32];
static unsigned char measUartRxBuf1[1024];
static unsigned char measUartTxBuf2[32];
static unsigned char measUartRxBuf2[1024];
extern int timeNum;											//��ʱ����		
extern char USART_RX_Buffer1[20];							//�汾��
extern unsigned char measure_Uart_Flag; 					//��ѧ�������ڽ���ʱҪ�������ݲ�ѯ 
extern unsigned char control_Uart_Flag; 					//�����������ڽ���ʱҪ�������ݲ�ѯ
extern unsigned char control_Flag; 							//�����������ڽ���ʱҪ�������ݲ�ѯ,���û�ѡ���Ƿ������
//==============================================================================
// Global functions
int CVICALLBACK AbnmDCThreadFunction (void *functionData);		 									//�������̻߳ص�����


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __main_H__ */
