#include <ansi_c.h>
#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include "main.h"
#include "Uart.h"
#include "MainPanelCb.h" 
#include "Table.h"
#include "LoadPanel.h" 
#include "Cgs_mt.h"
#include "Plot.h"
//==============================================================================
//
// Title:		Uart.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/9/27 at 11:44:29 by 斌 冯.
// Copyright:	浙江鼎奕科技发展有限公司. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//#include "Uart.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
RxDataTypeDef RxData1,RxData2;
int logFlag;
int rowIndex = 2;
int rowIndex2 = 2;
int rowIndex3 = 2;
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
static int CheckPortStatus(unsigned char portNumber, unsigned char uartRxLen, void (*pFunc)(int, int, void*))
{
	int status;
	status = OpenComConfig(portNumber, "", 115200, 0, 8, 1, 512, 512);	   //Config and Connect serial port
	if(status != 0) 
	{
		MessagePopup("Error","Device unconnected.");
		return -1;
	}
	else
	{
		InstallComCallback (portNumber, LWRS_RECEIVE, uartRxLen, 0, pFunc, 0);   //binding Callback function to serial input data		18 bytes received will calling for an interrupt
		SetCTSMode(portNumber, LWRS_HWHANDSHAKE_OFF);
		FlushInQ(portNumber);	   														//Clear input and output buffer
		FlushOutQ(portNumber);
		return 0;
	}
}
void CheckCloseCom(int SA3102_UART_Status,unsigned char measureComPort,unsigned char controlComPort)
{
	if(SA3102_UART_Status==1)				//如果环境设备打开的话，关闭软件时需要关闭对应的串口。用户不使用环境测量设备时就没有打开，所以就不用关闭。
	{
		CloseCom(measureComPort);
	}
	if(SA3102_UART_Status==2)				//如果电学设备打开的话，关闭软件时需要关闭对应的串口。用户不使用电学测量设备时就没有打开，所以就不用关闭。
	{
		CloseCom(controlComPort);
	}
	if(SA3102_UART_Status==3)				//如果电学和环境的测量设备都打开的话，结束时需要同时关闭
	{
		CloseCom(controlComPort);
		CloseCom(measureComPort);
	}
}
int CheckOpenCom(unsigned char* measureComPort,unsigned char* controlComPort,unsigned char* measure_Uart_Flag,unsigned char* control_Uart_Flag)
{
	int Status = 0; 
	if(CheckPortStatus(*measureComPort, 20, ComCallback)<0)
	{
		Status=0;															//串口打开失败
		*measure_Uart_Flag = 0;
		MessagePopup("Error","电学测量设备串口打开失败"); 
		//要想打开软件，需要禁用此串口的操作命令
	}
	else 
	{
		Status=1;															//串口打开成功 
		*measure_Uart_Flag = 1;
	}
	if(CheckPortStatus(*controlComPort, 15, CtrlComCallback)<0)						//判断环境测量设备对应的串口是否连接成功
	{
		*control_Uart_Flag = 0;
		Status = Status + 0;     
		MessagePopup("Error","环境测量设备串口打开失败，如需测量环境参数，请检查通信线"); 
	}
	else
	{
		Status=Status + 2;
		*control_Uart_Flag = 1;
	}
	return Status;
}
void DeviceQuery(unsigned char* measureComPort,unsigned char* controlComPort)																					//查询设备串口号
{
	char IDquery[] = {0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
	char IDquery2[] = {0x11, 0x03, 0x00, 0x02, 0x00, 0x05, 0x26, 0x99}; 
	char USART_RX_Buffer1[40];   //接收
	char USART_RX_Buffer2[40];   //接收
	unsigned char i;
	int b, strLen;
		for(i = 1; i<=10; i++)
		{
			SetBreakOnLibraryErrors (0);
			b = OpenComConfig(i, "", 115200, 0, 8, 1, 30, 30);										//设置和打开串口 
		    if(b<0 )
			{
		
			}else
			{	
			    ComWrt(i, IDquery, 30);//发送查询指令
				Delay(3);
				strLen = GetInQLen (i);
				ComRd (i,USART_RX_Buffer1,strLen);
				if((USART_RX_Buffer1[1]== '3') && (USART_RX_Buffer1[2] == '1') && (USART_RX_Buffer1[3] == '0') && (USART_RX_Buffer1[4]== '2'))
				{
					*measureComPort = i;
					FlushInQ(*measureComPort);	   														//Clear input and output buffer
					FlushOutQ(*measureComPort);
					CloseCom (*measureComPort);
					break;
				}
				else
				{
					CloseCom(i);
				}
				SetCTSMode (i, LWRS_HWHANDSHAKE_OFF);   //无硬件交互
			}
		}
		for(i = 1; i<=10; i++)
		{
			SetBreakOnLibraryErrors (0);
			b = OpenComConfig(i, "", 115200, 0, 8, 1, 30, 30);  //设置和打开串口 
		    if(b<0 )
			{
		
			}else
			{	
			    ComWrt(i, IDquery2, 8);							//发送查询指令
				Delay(3);
				strLen = GetInQLen (i);
				ComRd (i,USART_RX_Buffer2,strLen);
				if((USART_RX_Buffer2[0]== 0x11) && (USART_RX_Buffer2[1] == 0x03) && (USART_RX_Buffer2[2] == 0x0a))
				{
					*controlComPort = i;							
					FlushInQ(*controlComPort);	   				//Clear input and output buffer
					FlushOutQ(*controlComPort);
					CloseCom (*controlComPort);
					break;
				}
				else
				{
					CloseCom(i);
				}
				SetCTSMode (i, LWRS_HWHANDSHAKE_OFF);			//无硬件交互
			}
		}
}
void TestStop(enum TestMode testMode)
{
	if((RxData1.rxStopSign == 0x01) || (RxData2.rxStopSign == 0x01) || (Graph.pCurveArray->numofSmu1RxDots > Graph.pCurveArray->numOfTotalDots-1) || ((Graph.pCurveArray + 1)->numofSmu2RxDots > (Graph.pCurveArray + 1)->numOfTotalDots-1))  //任何一个停止事件产生
	{
		switch(TestPara1.testMode)
		{
			case CALIBRATION:
				
				break;
				
			case NO_SWEEP_IV:
			case NO_SWEEP_VI: 
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))													//判断用户选择了哪一个测量板
					{
						if((RxData1.rxStopSign == 0x01) && (RxData2.rxStopSign == 0x01))								//判断是否两个板子都到结束了。
						{
							RxData1.rxStopSign = 0x00;
							RxData2.rxStopSign = 0x00;
							StopKeyAction(select_Addr1,select_Addr2);													//停止按钮按下后产生的一系列动作 
						}
						//StopKeyAction(select_Addr1,select_Addr2);														//停止按钮按下后产生的一系列动作
					}
				else if(select_Addr1 == 0x01)																			//停止1号板子
					{
						RxData1.rxStopSign = 0x00;																		//清除曲线结束标志位，避免下一次数据冲突
						StopKeyAction(0x01,select_Addr2);
					}
				else if(select_Addr2 == 0x02)																			//停止2号板子
					{
						RxData2.rxStopSign = 0x00;																		//清除曲线结束标志位，避免下一次数据冲突
						StopKeyAction(0x00,select_Addr2);
					}
				break;
			case NO_SWEEP_IT:
			case NO_SWEEP_RT:
				
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))
					{
						if((Graph.pCurveArray->numofSmu1RxDots > Graph.pCurveArray->numOfTotalDots-1) && ((Graph.pCurveArray + 1)->numofSmu2RxDots > (Graph.pCurveArray + 1)->numOfTotalDots-1) == 1)
							{
								StopKeyAction(select_Addr1,select_Addr2);												//停止按钮按下后产生的一系列动作
							}
					}
				else if(select_Addr1 == 0x01)																			// 判断是哪一个板卡产生的停止，通过地址来判断。
					{
						if(Graph.pCurveArray->numOfPlotDots >= Graph.pCurveArray->numOfTotalDots)				    	//板子1
							{
								StopKeyAction(select_Addr1,0x00);														//停止按钮按下后产生的一系列动作
							}
					}
				else if(select_Addr2 == 0x02)																			//2 号板子停止命令
					{
						if((Graph.pCurveArray + 1)->numOfPlotDots >= (Graph.pCurveArray + 1)->numOfTotalDots)			//板子2
							{
								StopKeyAction(0x00,select_Addr2);														//停止按钮按下后产生的一系列动作
							}
					}
				break;
			case NO_SWEEP_VT:
				
				break;
		}
	}
}

void Getxy(unsigned char *measUartRxBuf, RxDataTypeDef* RxData1, RxDataTypeDef* RxData2) 							//判断接收源表1 源表2的数据
{
		if((*measUartRxBuf  == 0x01) &&(Graph.pCurveArray->numofSmu1RxDots <= Graph.pCurveArray->numOfTotalDots-1)) //判断是否是 01 地址传来 的数据
		{
			if(TestPara1.testMode == NO_SWEEP_IV)																	//根据不同模式 选择不同的X 轴数据
			{
				if(logFlag)
				{
					*(Graph.pCurveArray->pDotY++) = log10((double)(RxData1->rx_current.num_float));					//get y, set pointer to the next data 
				}
				else
				{
					*(Graph.pCurveArray->pDotY++) = RxData1->rx_current.num_float;									//get y, set pointer to the next data 
				}
				*(Graph.pCurveArray->pDotX++) = RxData1->rx_Theory_voltaget;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex), *(Graph.pCurveArray->pDotY-1)); 
				rowIndex++; 
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_voltage.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//显现当前档位信息 
			}
			else if(TestPara1.testMode == NO_SWEEP_VI )
			{
				*(Graph.pCurveArray->pDotX++) = RxData1->rx_Theory_current;
				*(Graph.pCurveArray->pDotY++) = RxData1->rx_voltage.num_float;										//get y, set pointer to the next data
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex), *(Graph.pCurveArray->pDotY-1)); 
				rowIndex++; 
				
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_current.num_float);							//监控电流
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//显现当前档位信息 
				
			}
			else if(TestPara1.testMode == NO_SWEEP_IT )
			{
				if(logFlag)
				{
					*(Graph.pCurveArray->pDotY++) = log10((double)(RxData1->rx_current.num_float));					//get y, set pointer to the next data 
				}
				else
				{
					*(Graph.pCurveArray->pDotY++) = RxData1->rx_current.num_float;									//get y, set pointer to the next data 
				}
				*(Graph.pCurveArray->pDotX++) = (Graph.X_Axis1++)*TestPara1.timeStep*0.001;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex),*(Graph.pCurveArray->pDotY-1)); 
				rowIndex++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_voltage.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//显现当前档位信息 
			}
			if(TestPara1.testMode == NO_SWEEP_VT )
			{
				*(Graph.pCurveArray->pDotX++) = (Graph.X_Axis1++)*TestPara1.timeStep*0.001;
				*(Graph.pCurveArray->pDotY++) = RxData1->rx_voltage.num_float;										//get y, set pointer to the next data 
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex),*(Graph.pCurveArray->pDotY-1)); 
				rowIndex++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_current.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//显现当前档位信息 
				
			}
			if(TestPara1.testMode == NO_SWEEP_RT )
			{
				*(Graph.pCurveArray->pDotX++) = (Graph.X_Axis1++)*TestPara1.timeStep*0.001;		
				*(Graph.pCurveArray->pDotY++) = (TestPara1.Voltage_Start*0.001)/RxData1->rx_current.num_float;		//get y, set pointer to the next data
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex),*(Graph.pCurveArray->pDotY-1)); 
				rowIndex++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_voltage.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//显现当前档位信息 
			}
			
			
			Graph.pCurveArray->numOfDotsToPlot++;				//number of dots to plot increase 
			Graph.pCurveArray->numofSmu1RxDots++;
			
			  
			
		}
		else if((*measUartRxBuf  == 0x02) && ((Graph.pCurveArray + 1)->numofSmu2RxDots <= (Graph.pCurveArray + 1)->numOfTotalDots-1)) //判断是否是 02 地址传来 的数据
		{
			if(TestPara2.testMode == NO_SWEEP_IV )
			{
				if(logFlag)
				{
					*((Graph.pCurveArray + 1)->pDotY++) = log10((double)(RxData2->rx_current.num_float));			//get y, set pointer to the next data 
				}
				else
				{
					*((Graph.pCurveArray + 1)->pDotY++) = RxData2->rx_current.num_float;  							//get y, set pointer to the next data 
				}
				*((Graph.pCurveArray + 1)->pDotX++) = RxData2->rx_Theory_voltaget;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_voltage.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);							//显现当前档位信息 
				
			}
			if(TestPara2.testMode == NO_SWEEP_VI )
			{
				*((Graph.pCurveArray +1)->pDotX++) = RxData2->rx_Theory_current;
				*((Graph.pCurveArray +1)->pDotY++) = RxData2->rx_voltage.num_float;										//get y, set pointer to the next data
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++; 
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_current.num_float);							//监控电流
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);						    //显现当前档位信息
				
			}
			if(TestPara2.testMode == NO_SWEEP_IT )
			{
				if(logFlag)
				{
					*((Graph.pCurveArray + 1)->pDotY++) = log10((double)(RxData2->rx_current.num_float));
				}
				else
				{
					*((Graph.pCurveArray + 1)->pDotY++) = RxData2->rx_current.num_float; 
				}
				*((Graph.pCurveArray + 1)->pDotX++) = (Graph.X_Axis2++)*TestPara2.timeStep*0.001;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);    
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++; 
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_voltage.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);							//显现当前档位信息 
			}
			if(TestPara2.testMode == NO_SWEEP_VT )
			{
				*((Graph.pCurveArray +1)->pDotX++) = (Graph.X_Axis2++)*TestPara1.timeStep*0.001;
				*((Graph.pCurveArray +1)->pDotY++) = RxData2->rx_voltage.num_float;										//get y, set pointer to the next data 
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_current.num_float);							//监控电流
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);						    //显现当前档位信息
			}
			if(TestPara2.testMode == NO_SWEEP_RT )
			{
				*((Graph.pCurveArray + 1)->pDotX++) = (Graph.X_Axis2++)*TestPara2.timeStep*0.001;
				*((Graph.pCurveArray + 1)->pDotY++) = (TestPara2.Voltage_Start*0.001)/RxData2->rx_current.num_float;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);    
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_voltage.num_float);							//监控电压
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);							//显现当前档位信息
				
			}
			(Graph.pCurveArray+1)->numOfDotsToPlot++;				 													//number of dots to plot increase
			(Graph.pCurveArray+1)->numofSmu2RxDots++;
			
		}
		//**********************************************************    判断第一个点Y轴数据点来确定初始Y轴坐标轴范围  ***************************************************************************//	
			if(select_Addr1 == 0x01 && select_Addr2 == 0x02)
			{
				if((Graph.pCurveArray + 1)->numOfDotsToPlot == 1)
				{
					if(*((Graph.pCurveArray + 1)->pDotY-1) > 0)																											//判断Y轴数据位 为正值
					{
						Graph.pGraphAttr->yAxisTail = (*((Graph.pCurveArray + 1)->pDotY-1))+(*((Graph.pCurveArray + 1)->pDotY-1)*0.00009);
						Graph.pGraphAttr->yAxisHead = (*((Graph.pCurveArray + 1)->pDotY-1))-(*((Graph.pCurveArray + 1)->pDotY-1)*0.00009);
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
					}
					else if(*((Graph.pCurveArray + 1)->pDotY-1) < 0 && (Graph.pCurveArray + 1)->numOfDotsToPlot < 1)																									//判断Y轴数据为负值
					{
						Graph.pGraphAttr->yAxisTail = *((Graph.pCurveArray + 1)->pDotY-1) - *((Graph.pCurveArray + 1)->pDotY-1)*0.00009;
						Graph.pGraphAttr->yAxisHead = *((Graph.pCurveArray + 1)->pDotY-1) + *((Graph.pCurveArray + 1)->pDotY-1)*0.00009;
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
					}
				}
				
				if(Graph.pCurveArray->numOfDotsToPlot == 1)
				{
					if(*(Graph.pCurveArray->pDotY-1) > 0)																												//判断Y轴数据位 为正值
					{
						Graph.pGraphAttr->yAxisTail = (*(Graph.pCurveArray->pDotY-1))+(*(Graph.pCurveArray->pDotY-1)*0.00009);
						Graph.pGraphAttr->yAxisHead = (*(Graph.pCurveArray->pDotY-1))-(*(Graph.pCurveArray->pDotY-1)*0.00009);
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
					}
					else if(*(Graph.pCurveArray->pDotY-1) < 0 && Graph.pCurveArray->numOfDotsToPlot < 1)																											//判断Y轴数据为负值
					{
						Graph.pGraphAttr->yAxisTail = *(Graph.pCurveArray->pDotY-1) - *(Graph.pCurveArray->pDotY-1)*0.00009;
						Graph.pGraphAttr->yAxisHead = *(Graph.pCurveArray->pDotY-1) + *(Graph.pCurveArray->pDotY-1)*0.00009;
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
					}
				}
			}
			else if(select_Addr2 == 0x02 && (Graph.pCurveArray + 1)->numOfDotsToPlot < 2 && (*((Graph.pCurveArray + 1)->pDotY) != 0))								//判断点数。必须是第一个点
			{
				if(*((Graph.pCurveArray + 1)->pDotY-1) > 0)																											//判断Y轴数据位 为正值
				{
					Graph.pGraphAttr->yAxisTail = (*((Graph.pCurveArray + 1)->pDotY-1))+(*((Graph.pCurveArray + 1)->pDotY-1)*0.00009);
					Graph.pGraphAttr->yAxisHead = (*((Graph.pCurveArray + 1)->pDotY-1))-(*((Graph.pCurveArray + 1)->pDotY-1)*0.00009);
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
				}
				else if(*((Graph.pCurveArray + 1)->pDotY-1) < 0)																									//判断Y轴数据为负值
				{
					Graph.pGraphAttr->yAxisTail = *((Graph.pCurveArray + 1)->pDotY-1) - *((Graph.pCurveArray + 1)->pDotY-1)*0.00009;
					Graph.pGraphAttr->yAxisHead = *((Graph.pCurveArray + 1)->pDotY-1) + *((Graph.pCurveArray + 1)->pDotY-1)*0.00009;
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
				}
			}
			else if(select_Addr1 == 0x01 && Graph.pCurveArray->numOfDotsToPlot < 2 && (*(Graph.pCurveArray->pDotY) != 0))																							
			{
				if(*(Graph.pCurveArray->pDotY-1) > 0)																												//判断Y轴数据位 为正值
				{
					Graph.pGraphAttr->yAxisTail = (*(Graph.pCurveArray->pDotY-1))+(*(Graph.pCurveArray->pDotY-1)*0.00009);
					Graph.pGraphAttr->yAxisHead = (*(Graph.pCurveArray->pDotY-1))-(*(Graph.pCurveArray->pDotY-1)*0.00009);
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//设置  X  轴的范围
				}
				else if(*(Graph.pCurveArray->pDotY-1) < 0)																											//判断Y轴数据为负值
				{
					Graph.pGraphAttr->yAxisTail = *(Graph.pCurveArray->pDotY-1) - *(Graph.pCurveArray->pDotY-1)*0.00009;
					Graph.pGraphAttr->yAxisHead = *(Graph.pCurveArray->pDotY-1) + *(Graph.pCurveArray->pDotY-1)*0.00009;
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);					//设置  X  轴的范围
				}
			}
		//**********************************************************    判断第一个点Y轴数据点来确定初始Y轴坐标轴范围  ***************************************************************************//	
}
void CVICALLBACK ComCallback(int portNumber, int eventMask, void * callbackData)
{
	measure_Uart_Flag = 0;
		int status;
		int rxNum;																							  
		int i=0;																								//接收时屏蔽查询，关闭数据查询
		//RxDataTypeDef RxData1,RxData2;
		//RxData1.rxStopSign=0x00;
		//RxData2.rxStopSign=0x00;
		rxNum = GetInQLen(measureComPort);  																	//读取串口中发送来的数据数量
		//if(rxNum>500) rxNum=500;																				//防止超过内存范围
		while(rxNum>=MEASURE_UART_RX_LEN)
		{	 
				status = ComRd(measureComPort, (char *)measUartRxBuf1, MEASURE_UART_RX_LEN); 				
				ProtocolGetData(measUartRxBuf1+i*MEASURE_UART_RX_LEN, &RxData1,&RxData2);						//get data from uart buffer ,并且判断是否是源表1或2 数据，分别放入相应的缓存里
				Getxy(&measUartRxBuf1[i*MEASURE_UART_RX_LEN], &RxData1, &RxData2);								//从串口传来的数据中取出  X与Y轴 的数据
				rxNum -=MEASURE_UART_RX_LEN;
		}
							
		//PlotCurve1(&Graph, graphDispPanel, GRAPHDISP_GRAPH1, 0);	//  增加参数    曲线总数目   第一条曲线标号 //画图   电学测量曲线图
		PlotCurve(&Graph, graphDispPanel, GRAPHDISP_GRAPH1);									 	//   
		TestStop(TestPara1.testMode);
	measure_Uart_Flag = 1; 																			//开启查询数据
}
void CVICALLBACK CtrlComCallback(int portNumber, int eventMask, void * callbackData)
{
	control_Uart_Flag = 0;																			//接收时屏蔽查询，不在查询数据     
		int rxNum;
		int i,j = 0;
		Rx_CGS_DataTypeDef Rx_CGS_Data;
		Rx_CGS_Data.heating_stage_temp = 0;
		Rx_CGS_Data.environmental_humidity = 0;
		Rx_CGS_Data.pressure = 0;
		rxNum = GetInQLen(controlComPort);  															//读取串口中发送来的数据数量
		if(rxNum>500) rxNum=500;																		//防止超过内存范围
		while(rxNum>=15)
		{
			if((Graph.pCurveArray->numOfPlotDots >0) || ((Graph.pCurveArray + 1)->numOfPlotDots >0))
			{
				ComRd(controlComPort, (char *)meas_CGS_UartRxBuf, 15);
				ProtocolGet_CGS_Data(meas_CGS_UartRxBuf, &Rx_CGS_Data);									//从串口中取出环境测量参数
				if(Rx_CGS_Data.rxDevAddr == 0x11)
				{
					
								if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))								//判断环境参数的坐标是跟随 源表1  还是源表2 的横坐标
								{
									j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
								}
								if(select_Addr1 == 0x01)
								{
									j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
								}
								if(select_Addr2 == 0x02)
								{
									j = (Graph.pCurveArray + 1)->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots; 
								}
						for(i = 0; i<j; i++)				  
						{
							if((TestPara1.testMode == NO_SWEEP_VT) || (TestPara1.testMode == NO_SWEEP_RT) || (TestPara1.testMode == NO_SWEEP_IT) || (TestPara2.testMode == NO_SWEEP_RT) || (TestPara2.testMode == NO_SWEEP_VT) || (TestPara2.testMode == NO_SWEEP_IT))
							{
								*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.timeStep*0.001;
								*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.timeStep*0.001; 
								*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.timeStep*0.001;
								Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + 1;										//1*TestPara1.Current_Step;
							}
							if((TestPara1.testMode == NO_SWEEP_IV) || (TestPara1.testMode == NO_SWEEP_VI) || (TestPara2.testMode == NO_SWEEP_IV) || (TestPara2.testMode == NO_SWEEP_VI))
							{
								*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1;
								*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1; 
								*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1;
								Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + TestPara2.Current_Step;					//1*TestPara1.Current_Step;  
							}
							
							
						    *(Graph_Temp.pCurveArray->pDotY++)		 = Rx_CGS_Data.heating_stage_temp;				//热台温度      
							*((Graph_Temp.pCurveArray + 1)->pDotY++) = Rx_CGS_Data.environmental_humidity;			//环境湿度
							
							if(Rx_CGS_Data.pressure > 10500)														//限制压强显示，标准大气压。
							{
								Rx_CGS_Data.pressure = 10500;		  
							}
							*((Graph_Temp.pCurveArray + 2)->pDotY++) = Rx_CGS_Data.pressure * 0.001;				//压强
					
				
							SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 5, 1, 3), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (5,rowIndex3), *(Graph_Temp.pCurveArray->pDotY-1));
							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (6,rowIndex3), *((Graph_Temp.pCurveArray + 1)->pDotY-1));
							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (7,rowIndex3), Rx_CGS_Data.pressure);							//压强
							rowIndex3++;
				
							Graph_Temp.pCurveArray->numOfDotsToPlot++;  
							(Graph_Temp.pCurveArray +1)->numOfDotsToPlot++;
							(Graph_Temp.pCurveArray +2)->numOfDotsToPlot++;
						}
				}
			}
			else
			{
			FlushInQ(controlComPort);	   											
			FlushOutQ(controlComPort);
			}
			rxNum -=15;  
		}
		SetCtrlVal (hEnvResultPanel, ENVIRPANEL_STR_TEMP, Rx_CGS_Data.heating_stage_temp);				//热台温度
		SetCtrlVal (hEnvResultPanel, ENVIRPANEL_STR_HUM,  Rx_CGS_Data.environmental_humidity);			//环境湿度
		SetCtrlVal (hEnvResultPanel, ENVIRPANEL_STR_PRES, Rx_CGS_Data.pressure * 0.001);				//压强
		PlotCurve_Temp(&Graph_Temp, graphDispPanel, GRAPHDISP_GRAPH2);									//画曲线图
	control_Uart_Flag = 1;																				//接收时屏蔽查询，不在查询数据
}

