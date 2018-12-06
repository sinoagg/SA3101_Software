//==============================================================================
//
// Title:		Timer.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/16 at 17:33:45 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <ansi_c.h>
#include <utility.h>
#include "Graph.h"
#include "GraphDisp.h"
#include "LoadPanel.h"
#include "Plot.h"
#include "protocol.h"
#include "Cgs_mt.h"
#include "main.h"
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
float controlTime = 0;
int timeNum = 0;
float a;
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
//int CVICALLBACK TimerCallback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
//{
//	switch (event)
//	{
//		case EVENT_TIMER_TICK:
//			timeNum++;
//			controlTime++;
//			if(timeNum == 10)
//			{
//					if(measure_Uart_Flag == 1)//���ڽ���ʱҪ�������ݲ�ѯ
//					{
//						ProtocolQuery( measureComPort, select_Addr1, select_Addr2, measUartTxBuf1,  measUartTxBuf2);
//					}
//					if(control_Uart_Flag == 1)
//					{
//						Read_CGS_Value(controlComPort);
//					}
//			timeNum = 0;
//			}
//			SetGraphX_Axis(GRAPHDISP_GRAPH1,&Graph);		
//			SetGraphX_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp);
//			SetGraphY_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp); 
//		//	if(controlTime*10 == TestPara1.SampleDelay)
//		//	{  
//		//	a = a + TestPara1.SampleDelay*0.001;       
//		//	*(Graph_Temp.pCurveArray->pDotX++)=a; 		 //��������ʱ��
//		//	*(Graph.pCurveArray->pDotX++)=a;
//	    // 
//		//	//*((Graph_Temp.pCurveArray + 1)->pDotX++) = (controlTime*0.001) * TestPara1.SampleDelay*0.001;  
//		//	//*((Graph_Temp.pCurveArray + 2)->pDotX++) = (controlTime*0.001) * TestPara1.SampleDelay*0.001;

//		//
//		//	printf("%f\n", a); 
//		//	controlTime = 0;
//		//	}
//			
//		
//			break; 
//	}
//	return 0;
//}
//����Ϊ�޸�֮ǰ�Ĵ��룬����Ϊ�޸Ĳ��Դ��롣
int CVICALLBACK TimerCallback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
		
			
			if(measure_Uart_Flag == 1)//���ڽ���ʱҪ�������ݲ�ѯ
			{
				ProtocolQuery( measureComPort, select_Addr1, select_Addr2, measUartTxBuf1,  measUartTxBuf2);
			}
			SetGraphX_Axis(GRAPHDISP_GRAPH1,&Graph);
			
			
			SetGraphX_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp);
			
			
			
			SetGraphY_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp); 
			
			
			break; 
	}
	return 0;
}
