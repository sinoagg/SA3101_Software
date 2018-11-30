//==============================================================================
//
// Title:		Protocol.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/9 at 9:58:35 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <utility.h>
#include <userint.h>
#include "Protocol.h"
#include "LoadPanel.h"
#include "ResultMenuPanel.h"
#include "IdVdsPanel.h"
#include "IdVgsPanel.h"
#include "I_T_Panel.h"
#include <rs232.h>
#include "table.h"
#include "Plot.h"
//==============================================================================
// Constants

//==============================================================================
// Types
enum MsgType
{
	MSG_TYPE_NULL			=0,
	MSG_TYPE_SETTING		=0x11,			//设置命令				
	MSG_TYPE_RUN			=0x12,			//起始命令
	MSG_TYPE_STOP			=0x13,			//停止命令
	MSG_TYPE_QUERY			=0x14,			//轮询命令
	MSG_TYPE_CALIBRATION	=0x15,			//校准命令
	MSG_TYPE_REPORT_ID		=0xFF   		//ID查询命令
};
//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
TestParaTypeDef TestPara1;  
TestParaTypeDef TestPara2;
unsigned char select_Addr1 = 0X01;   		//由用户选择是否使用此 源表 1 	  默认	0x01 为 选中，0x00为不选中，不使用源表 1 进行测量
unsigned char select_Addr2 = 0X02;   		//由用户选择是否使用此 源表 2	  默认	0x02 为 选中，0x00为不选中，不使用源表 2 进行测量
unsigned char measure_Uart_Flag = 0;
unsigned char control_Uart_Flag = 0;
extern unsigned char control_Flag = 0;


//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return
static unsigned char GetXorCheckVal(unsigned char* pUartBuf, unsigned char lenth)							//计算校验值
{
	unsigned char i, xorCheck=0;
	for(i=0;i<lenth;i++)
		xorCheck^=*(pUartBuf+i);	
	return xorCheck;	
}
/*


	功能：分别向  地址1 地址2 放入 设置命令


*/
void PrepareCfgTxData(TestParaTypeDef* pTestPara1,TestParaTypeDef* pTestPara2,unsigned char devAddr1, unsigned char devAddr2, unsigned char expType, unsigned char* measUartTxBuf1,unsigned char* measUartTxBuf2)	//Make config info to UART Tx Buffer
{
	if(devAddr1 == 0x01)
	{
		*measUartTxBuf1=devAddr1; 																			//设备地址
		*(measUartTxBuf1+1)=0x11;																			//命令类型  0Graph_Temp.X_Axis11设置  0Graph_Temp.X_Axis12起始		0Graph_Temp.X_Axis13停止	0Graph_Temp.X_Axis14轮询	0Graph_Temp.X_Axis15校准	0XFF查询
		*(measUartTxBuf1+2)=expType;																		//测试类型
		*(measUartTxBuf1+3)=(pTestPara1->outputRelay&0x0F) | ((pTestPara1->sweepSpeed<<4)&0xf0);			//输出继电器状态     
		//将源表 1 的 电压 开始 结束 间隔参数 放入数组中准备发送
		*(measUartTxBuf1+4)=(unsigned char)(pTestPara1->Voltage_Start >> 8) ;	   
		*(measUartTxBuf1+5)=(unsigned char)(pTestPara1->Voltage_Start & 0xFF);
		*(measUartTxBuf1+6)=(unsigned char)(pTestPara1->Voltage_Stop >> 8) ;
		*(measUartTxBuf1+7)=(unsigned char)(pTestPara1->Voltage_Stop & 0xFF );
		*(measUartTxBuf1+8)=(unsigned char)(pTestPara1->Voltage_Step >> 8 ) ;
		*(measUartTxBuf1+9)=(unsigned char)(pTestPara1->Voltage_Step & 0xFF);
		//将源表 1 的 电流 开始 结束 间隔参数 放入数组中准备发送 
		*(measUartTxBuf1+10)=(unsigned char)(pTestPara1->Current_Start >> 8 );	    
		*(measUartTxBuf1+11)=(unsigned char)(pTestPara1->Current_Start & 0xFF);
		*(measUartTxBuf1+12)=(unsigned char)(pTestPara1->Current_Stop >> 8) ;
		*(measUartTxBuf1+13)=(unsigned char)(pTestPara1->Current_Stop & 0xFF );
		*(measUartTxBuf1+14)=(unsigned char)(pTestPara1->Current_Step >> 8) ;
		*(measUartTxBuf1+15)=(unsigned char)(pTestPara1->Current_Step & 0xFF);
		//将源表 1 的 其他参数 放入数组中准备发送
		*(measUartTxBuf1+16)=(unsigned char)(pTestPara1->quietTime >>8);
		*(measUartTxBuf1+17)=(unsigned char)(pTestPara1->quietTime &0xFF);
		*(measUartTxBuf1+18)=(unsigned char)(pTestPara1->timeStep >>8);
		*(measUartTxBuf1+19)=(unsigned char)(pTestPara1->timeStep&0xFF);
		*(measUartTxBuf1+20)=(unsigned char)(pTestPara1->runTime >>8);
		*(measUartTxBuf1+21)=(unsigned char)(pTestPara1->runTime&0xFF);
		*(measUartTxBuf1+22)=(unsigned char)(pTestPara1->sampleRate >>8);
		*(measUartTxBuf1+23)=(unsigned char)(pTestPara1->sampleRate&0xFF);
		*(measUartTxBuf1+24)=(unsigned char)(pTestPara1->sampleNumber >>8);
		*(measUartTxBuf1+25)=(unsigned char)(pTestPara1->sampleNumber&0xFF);
		*(measUartTxBuf1+26)=(unsigned char)(pTestPara1->rangeMode&0xFF);
		*(measUartTxBuf1+27)=pTestPara1->maxRange;  													//量程使用最大档位    	pTestPara1->maxRange 
		*(measUartTxBuf1+28)=pTestPara1->minRange;  													//量程使用最小档位 	    pTestPara1->minRange 
		*(measUartTxBuf1+29)=(unsigned char)((pTestPara1->SampleDelay)>>8);								//采样延时
		*(measUartTxBuf1+30)=(unsigned char)((pTestPara1->SampleDelay)&0xFF); 
		*(measUartTxBuf1+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1); 		//校验值
	}
	if(devAddr2 == 0x02)
	{ 
		*measUartTxBuf2=devAddr2; 																		//设备地址
		*(measUartTxBuf2+1)=0x11;																		//命令类型  0Graph_Temp.X_Axis11设置  0Graph_Temp.X_Axis12起始		0Graph_Temp.X_Axis13停止	0Graph_Temp.X_Axis14轮询	0Graph_Temp.X_Axis15校准	0XFF查询
		*(measUartTxBuf2+2)=expType;																	//测试类型
		*(measUartTxBuf2+3)=(pTestPara2->outputRelay&0x0F) | ((pTestPara2->sweepSpeed<<4)&0xf0);		//输出继电器状态 
		//将源表 2 的 电压 开始 结束 间隔参数 放入数组中准备发送 													  
		*(measUartTxBuf2+4)=(unsigned char)(pTestPara2->Voltage_Start >> 8);	   
		*(measUartTxBuf2+5)=(unsigned char)(pTestPara2->Voltage_Start & 0xFF);
		*(measUartTxBuf2+6)=(unsigned char)(pTestPara2->Voltage_Stop >> 8) ;
		*(measUartTxBuf2+7)=(unsigned char)(pTestPara2->Voltage_Stop & 0xFF );
		*(measUartTxBuf2+8)=(unsigned char)(pTestPara2->Voltage_Step >> 8 ) ;
		*(measUartTxBuf2+9)=(unsigned char)(pTestPara2->Voltage_Step & 0xFF);
		//将源表 2 的 电流 开始 结束 间隔参数 放入数组中准备发送
		*(measUartTxBuf2+10)=(unsigned char)(pTestPara2->Current_Start >> 8 );	    
		*(measUartTxBuf2+11)=(unsigned char)(pTestPara2->Current_Start & 0xFF);
		*(measUartTxBuf2+12)=(unsigned char)(pTestPara2->Current_Stop >> 8) ;
		*(measUartTxBuf2+13)=(unsigned char)(pTestPara2->Current_Stop & 0xFF );
		*(measUartTxBuf2+14)=(unsigned char)(pTestPara2->Current_Step >> 8) ;
		*(measUartTxBuf2+15)=(unsigned char)(pTestPara2->Current_Step & 0xFF);
		//将源表 2  的 其他参数 放入数组中准备发送
		*(measUartTxBuf2+16)=(unsigned char)(pTestPara2->quietTime >>8);
		*(measUartTxBuf2+17)=(unsigned char)(pTestPara2->quietTime&0xFF);
		*(measUartTxBuf2+18)=(unsigned char)(pTestPara2->timeStep >>8);
		*(measUartTxBuf2+19)=(unsigned char)(pTestPara2->timeStep&0xFF);
		*(measUartTxBuf2+20)=(unsigned char)(pTestPara2->runTime >>8);
		*(measUartTxBuf2+21)=(unsigned char)(pTestPara2->runTime&0xFF);
		*(measUartTxBuf2+22)=(unsigned char)(pTestPara2->sampleRate >>8);
		*(measUartTxBuf2+23)=(unsigned char)(pTestPara2->sampleRate&0xFF);
		*(measUartTxBuf2+24)=(unsigned char)(pTestPara2->sampleNumber >>8);
		*(measUartTxBuf2+25)=(unsigned char)(pTestPara2->sampleNumber&0xFF);
		*(measUartTxBuf2+26)=(unsigned char)(pTestPara2->rangeMode&0xFF); 
		*(measUartTxBuf2+27)=pTestPara2->maxRange;  													//量程使用最大档位    	pTestPara1->maxRange 
		*(measUartTxBuf2+28)=pTestPara2->minRange;  													//量程使用最小档位 	    pTestPara1->minRange
		*(measUartTxBuf2+29)=(unsigned char)((pTestPara2->SampleDelay)>>8);								//采样延时
		*(measUartTxBuf2+30)=(unsigned char)((pTestPara2->SampleDelay)&0xFF); 
		*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1); 		//校验位
	}
}
/*


	1,分别向1、2号板子发送    起始命令
	2,板子地址可以选择使用单源表还是双源表


*/
void ProtocolRun(unsigned char measureComPort, unsigned char devAddr1, unsigned char devAddr2, unsigned char* measUartTxBuf1, unsigned char* measUartTxBuf2)
{
	if((devAddr1 == 0x01) && (devAddr2 == 0x02))
	{
		*measUartTxBuf1=0x00;									//广播地址，同时启动
		*(measUartTxBuf1+1)=MSG_TYPE_RUN;
		*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
		Delay(0.05);
	}
	else if(devAddr1 == 0x01)
	{
		*measUartTxBuf1 = devAddr1;
		*(measUartTxBuf1+1) = MSG_TYPE_RUN;
		*(measUartTxBuf1+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
		Delay(0.05);	
	}
	else if(devAddr2 == 0x02)
	{
		*measUartTxBuf2=devAddr2;
		*(measUartTxBuf2+1)=MSG_TYPE_RUN;
		*(measUartTxBuf2+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
		Delay(0.05);
	}
}
/*

	1,分别向1、2号板子发送    停止命令
	2,板子地址可以选择使用单源表还是双源表

*/

void ProtocolStop(unsigned char measureComPort, unsigned char devAddr1, unsigned char devAddr2, unsigned char* measUartTxBuf1, unsigned char* measUartTxBuf2)
{
	if((devAddr1 == 0x01) && (devAddr2 == 0x02))
	{
		*measUartTxBuf1=0x00;										//广播地址，同时停止
		*(measUartTxBuf1+1)=MSG_TYPE_STOP;
		*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
		Delay(0.02);
	}
	else if(devAddr1 == 0x01)
	{
		*measUartTxBuf1 = devAddr1;
		*(measUartTxBuf1+1) = MSG_TYPE_STOP;
		*(measUartTxBuf1+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
		Delay(0.02); 
	}
	else if(devAddr2 == 0x02)
	{
		*measUartTxBuf2 = devAddr2;
		*(measUartTxBuf2+1) = MSG_TYPE_STOP;
		*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
		Delay(0.02); 
	}
}
/*


	1,分别向1、2号板子发送    查询命令
	2,板子地址可以选择使用单源表还是双源表


*/
void ProtocolQuery(unsigned char measureComPort, unsigned char devAddr1,unsigned char devAddr2, unsigned char* measUartTxBuf1, unsigned char* measUartTxBuf2)
{
	
	if(control_Flag == 0)
	{
		if((devAddr1 == 0x01) && (devAddr2 == 0x02))
		{
			*measUartTxBuf1=devAddr1;
			*(measUartTxBuf1+1)=0x14;
			*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			Delay(0.1);
			*measUartTxBuf2=devAddr2;
			*(measUartTxBuf2+1)=0x14;
			*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
		}
		else if(devAddr1 == 0x01) 
		{
			*measUartTxBuf1=devAddr1;
			*(measUartTxBuf1+1)=0x14;
			*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			Delay(0.1); 
			*measUartTxBuf1=devAddr1;
			*(measUartTxBuf1+1)=0x14;
			*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
		}
		else if(devAddr2 == 0x02) 
		{
			*measUartTxBuf2=devAddr2;
			*(measUartTxBuf2+1)=0x14;
			*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
			Delay(0.1);
			*measUartTxBuf2=devAddr2;
			*(measUartTxBuf2+1)=0x14;
			*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
		}
 	} 
	else
	{
		if((devAddr1 == 0x01) && (devAddr2 == 0x02) && (control_Flag == 1))
		{
			*measUartTxBuf1=devAddr1;
			*(measUartTxBuf1+1)=0x14;
			*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			Delay(0.066);
			*measUartTxBuf2=devAddr2;
			*(measUartTxBuf2+1)=0x14;
			*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
			Delay(0.066);
			Read_CGS_Value(controlComPort);
	
		}
		else if((devAddr2 == 0x02) && ((control_Flag == 1)))
		{
		  	*measUartTxBuf2=devAddr2;
			*(measUartTxBuf2+1)=0x14;
			*(measUartTxBuf2+SA31_UART_TX_LEN-1) = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
			Delay(0.1);
			Read_CGS_Value(controlComPort); 
		}
		else if((devAddr1 == 0x01) && ((control_Flag == 1)))
		{
			*measUartTxBuf1=devAddr1;
			*(measUartTxBuf1+1)=0x14;
			*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			Delay(0.1); 
			Read_CGS_Value(controlComPort);  

		}
	}
	
	

}
/*


	1,分别向1、2号板子发送    校准命令
	2,板子地址可以选择使用单源表还是双源表


*/
void ProtocolCalibrate(unsigned char measureComPort, unsigned char devAddr1, unsigned char* measUartTxBuf1, unsigned char devAddr2, unsigned char* measUartTxBuf2)
{
	if(devAddr1 == 0x01) 
	{
		*measUartTxBuf1=devAddr1;
		*(measUartTxBuf1+1)=0x15;
		*(measUartTxBuf1+SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
	}
	if(devAddr2 == 0x02) 
	{
		*measUartTxBuf2=devAddr2;
		*(measUartTxBuf2+1)=0x15;
		*(measUartTxBuf2 + SA31_UART_TX_LEN-1)=GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
		ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
	}
}
/*


	1,分别接收来的数据是 1或2号板子的数据。
	2,将判断后得到的数据分别放入到相应源表所定义的结构体中。


*/
void ProtocolGetData(unsigned char* pUartRxBuf, RxDataTypeDef* pRxData1, RxDataTypeDef* pRxData2)			//Get data from UART Rx Buffer
{
	unsigned char Addr;
	Addr = *pUartRxBuf;  																					//读取 设备 地址  0X01 1号板子   0X02  2号板子
		if(Addr == 0x01)
		{
			pRxData1->rxDevAddr=*pUartRxBuf; 																//1号板子设备地址 
			pRxData1->rxStopSign=*(pUartRxBuf+1);
		
			if(*(pUartRxBuf+2) & 0x80 )
			{
				pRxData1->rx_Theory_voltaget = (((int)(*(pUartRxBuf+2) << 8))|*(pUartRxBuf+3)) - 65536;
			}
			else
			{
				pRxData1->rx_Theory_voltaget = ((int)(*(pUartRxBuf+2) << 8))|*(pUartRxBuf+3); 
			}
	
			if(*(pUartRxBuf+4) & 0x80 ) 
			{
				pRxData1->rx_Theory_current =  (((int)(*(pUartRxBuf+4) << 8))|*(pUartRxBuf+5)) - 65536;   
			}
			else
			{
				pRxData1->rx_Theory_current =  ((int)(*(pUartRxBuf+4) << 8))|*(pUartRxBuf+5);
			}
			pRxData1->rx_voltage.num_uchar[3] = *(pUartRxBuf+6);
			pRxData1->rx_voltage.num_uchar[2] = *(pUartRxBuf+7); 
			pRxData1->rx_voltage.num_uchar[1] = *(pUartRxBuf+8); 
			pRxData1->rx_voltage.num_uchar[0] = *(pUartRxBuf+9);
	
			pRxData1->rx_current.num_uchar[3] = *(pUartRxBuf+10); 
			pRxData1->rx_current.num_uchar[2] = *(pUartRxBuf+11);
			pRxData1->rx_current.num_uchar[1] = *(pUartRxBuf+12);
			pRxData1->rx_current.num_uchar[0] = *(pUartRxBuf+13);
	
			pRxData1->rx_Default.num_uchar[3] = *(pUartRxBuf+14); 
			pRxData1->rx_Default.num_uchar[2] = *(pUartRxBuf+15);
			pRxData1->rx_Default.num_uchar[1] = *(pUartRxBuf+16);
			pRxData1->rx_Default.num_uchar[0] = *(pUartRxBuf+17);
		
			pRxData1->rangeSelect = *(pUartRxBuf+18);
			
			pRxData1->realTime=*(pUartRxBuf+19);   															//真实采样时间  
			pRxData1->realTime<<=8;
			pRxData1->realTime|=*(pUartRxBuf+20);   
			pRxData1->realTime<<=8;
			pRxData1->realTime|=*(pUartRxBuf+21);
			pRxData1->realTime<<=8;
			pRxData1->realTime|=*(pUartRxBuf+22);
		}
		if(Addr == 0x02)
		{
			pRxData2->rxDevAddr=*pUartRxBuf; 																//1号板子设备地址 
			pRxData2->rxStopSign=*(pUartRxBuf+1);
	
			if(*(pUartRxBuf+2) & 0x80 )
			{
				pRxData2->rx_Theory_voltaget = (((int)(*(pUartRxBuf+2) << 8))|*(pUartRxBuf+3)) - 65536;
			}
			else
			{
				pRxData2->rx_Theory_voltaget = ((int)(*(pUartRxBuf+2) << 8))|*(pUartRxBuf+3); 
			}
			if(*(pUartRxBuf+4) & 0x80 ) 
			{
				pRxData2->rx_Theory_current =  (((int)(*(pUartRxBuf+4) << 8))|*(pUartRxBuf+5)) - 65536;   
			}
			else
			{
				pRxData2->rx_Theory_current =  ((int)(*(pUartRxBuf+4) << 8))|*(pUartRxBuf+5);
			}
			pRxData2->rx_voltage.num_uchar[3] = *(pUartRxBuf+6);
			pRxData2->rx_voltage.num_uchar[2] = *(pUartRxBuf+7); 
			pRxData2->rx_voltage.num_uchar[1] = *(pUartRxBuf+8); 
			pRxData2->rx_voltage.num_uchar[0] = *(pUartRxBuf+9);
	
			pRxData2->rx_current.num_uchar[3] = *(pUartRxBuf+10); 
			pRxData2->rx_current.num_uchar[2] = *(pUartRxBuf+11);
			pRxData2->rx_current.num_uchar[1] = *(pUartRxBuf+12);
			pRxData2->rx_current.num_uchar[0] = *(pUartRxBuf+13);
	
			pRxData2->rx_Default.num_uchar[3] = *(pUartRxBuf+14); 
			pRxData2->rx_Default.num_uchar[2] = *(pUartRxBuf+15);
			pRxData2->rx_Default.num_uchar[1] = *(pUartRxBuf+16);
			pRxData2->rx_Default.num_uchar[0] = *(pUartRxBuf+17);
			pRxData2->rangeSelect = *(pUartRxBuf+18);
			
			pRxData2->realTime=*(pUartRxBuf+19);   															//真实采样时间  
			pRxData2->realTime<<=8;
			pRxData2->realTime|=*(pUartRxBuf+20);   
			pRxData2->realTime<<=8;
			pRxData2->realTime|=*(pUartRxBuf+21);
			pRxData2->realTime<<=8;
			pRxData2->realTime|=*(pUartRxBuf+22);
		}
}
