//==============================================================================
//
// Title:		Protocol.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/9 at 9:58:35 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Protocol_H__
#define __Protocol_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
#define DEFAULT_ADDR1 0x01 
#define DEFAULT_ADDR2 0x02 
		
#define SA31_UART_TX_LEN 32
#define SA31_MEASURE_UART_RX_LEN 24


//==============================================================================
// Types
typedef union										//float union
{
	
	unsigned char num_uchar[4]; 
	float num_float;
	
}Float_Union_Data;
enum TestCmd
{
	CMD_START=0x11,
	CMD_PAUSE=0x12,
	CMD_STOP=0x13,
	CMD_CONTINUE=0x14
};
enum TestMode
{
	CALIBRATION=0,
	NO_SWEEP_IV=1,
	NO_SWEEP_VI=2,
	NO_SWEEP_IT=3,
	NO_SWEEP_RT=4,
	NO_SWEEP_VT=5 
};
typedef struct
{
	unsigned char devAddr;
	enum TestCmd testCmd;
	enum TestMode testMode;
	unsigned char outputConnection;
	int Voltage_Start;									//start1
	int Voltage_Stop;									//stop1
	int Voltage_Step;									//step1
	int Current_Start;									//start2
	int Current_Stop;									//stop2
	int Current_Step;									//step2
	unsigned int quietTime;
	unsigned int timeStep;
	unsigned int runTime;								//effective only under R-t and I-t mode
	unsigned int sampleRate;
	unsigned int sampleNumber;
	int rangeMode;
	int outputRelay;
	int sweepSpeed;										//扫描速度
	unsigned char maxRange;
	unsigned char minRange;
	unsigned int SampleDelay;							//采样延时 
	float TotalDelay
}TestParaTypeDef;
		
typedef struct
{
	unsigned char rxDevAddr;						//received Device Address
	unsigned char rxStopSign;						//received Stop Sign 0x00 nonstop 0x01 curve stop 0x02 test stop
	
	int rx_Theory_voltaget;							//理论电压
	int rx_Theory_current;							//理论电流
	
	Float_Union_Data rx_voltage;					//当前采集电压
	Float_Union_Data rx_current;					//当前采集电流 
	Float_Union_Data rx_Default;					//默认数据
	long unsigned int realTime; 					//真实采样时间
	unsigned char rangeSelect;
	
}RxDataTypeDef;

typedef struct
{
	int panelHandle;
	int VdStartID;									//voltage drain start
	int VdStopID;									//voltage drain stop
	int VdStepID;									//voltage drain step
	int VgStartID;									//voltage gate start
	int VgStopID;									//voltage gate stop
	int VgStepID;									//voltage gate step
	
}ExpPanelTypeDef;
//==============================================================================
// External variables
extern TestParaTypeDef TestPara1;  
extern TestParaTypeDef TestPara2;
extern unsigned char select_Addr1;					//由用户选择是否使用此 源表 1 	  默认	0x01 为 选中，0x00为不选中，不使用源表 1 进行测量
extern unsigned char select_Addr2;					//由用户选择是否使用此 源表 2	  默认	0x02 为 选中，0x00为不选中，不使用源表 2 进行测量
extern unsigned char measUartTxBuf1[32];
extern unsigned char measUartTxBuf2[32];
extern unsigned char measureComPort;				//Serial Com Number
extern unsigned char controlComPort;				//Serial Com Number
//==============================================================================
// Global functions

void PrepareCfgTxData(TestParaTypeDef* pTestPara1,TestParaTypeDef* pTestPara2,unsigned char devAddr1, unsigned char devAddr2,unsigned char expType, unsigned char* measUartTxBuf1, unsigned char* measUartTxBuf2);
void ProtocolCfg(unsigned char measureComPort, unsigned char devAddr1, unsigned char devAddr2,unsigned char expType, unsigned char* pmeasUartTxBuf1,unsigned char* pmeasUartTxBuf2);
void ProtocolRun(unsigned char measureComPort, unsigned char devAddr1, unsigned char devAddr2, unsigned char*  measUartTxBuf1, unsigned char* measUartTxBuf2);
void ProtocolStop(unsigned char measureComPort, unsigned char devAddr1, unsigned char devAddr2, unsigned char* measUartTxBuf1, unsigned char* measUartTxBuf2); 
void ProtocolQuery(unsigned char measureComPort, unsigned char devAddr1,unsigned char devAddr2, unsigned char* measUartTxBuf1, unsigned char* measUartTxBuf2);
void ProtocolCalibrate(unsigned char measureComPort, unsigned char devAddr1, unsigned char* measUartTxBuf1, unsigned char devAddr2, unsigned char* measUartTxBuf2);
void ProtocolGetData(unsigned char* pUartRxBuf, RxDataTypeDef* pRxData1, RxDataTypeDef* pRxData2);
unsigned char GetXorCheckVal(unsigned char* pUartBuf, unsigned char lenth);   

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Protocol_H__ */
