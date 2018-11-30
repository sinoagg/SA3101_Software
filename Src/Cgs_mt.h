//==============================================================================
//
// Title:		Cgs_mt.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/7/31 at 15:52:21 by 斌 何.
// Copyright:	内蒙古民族大学. All Rights Reserved.
//
//==============================================================================

#ifndef __Cgs_mt_H__
#define __Cgs_mt_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
#define UART_CGS_RX_LEN 8
//==============================================================================
// Types
typedef struct
{
	unsigned char rxDevAddr;						//received Device Address

	unsigned int pressure;					//压力值
	
	int heating_stage_temp; 				//热台温度
	
	unsigned int Evaporation_temp;			//蒸发温度
	
	unsigned int environmental_humidity;	//环境湿度
	
	int environment_temp;					//环境温度 

}Rx_CGS_DataTypeDef;
//==============================================================================
// External variables
extern unsigned char meas_CGS_UartTxBuf[];
extern unsigned char meas_CGS_UartRxBuf[];
//==============================================================================
// Global functions
void Read_CGS_Value(unsigned char measureComPort);
void ProtocolGet_CGS_Data(unsigned char* meas_CGS_UartRxBuf, Rx_CGS_DataTypeDef* Rx_CGS_Data);	//Get data from UART Rx Buffer
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Cgs_mt_H__ */
