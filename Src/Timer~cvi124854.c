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
float
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int CVICALLBACK TimerCallback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			timeNum++;
			controlTime++;
			if(timeNum == 10)
			{
					if(measure_Uart_Flag == 1)//串口接收时要屏蔽数据查询
					{
						ProtocolQuery( measureComPort, select_Addr1, select_Addr2, measUartTxBuf1,  measUartTxBuf2);
					}
					if(control_Uart_Flag == 1)
					{
						Read_CGS_Value(controlComPort);
					}
			
			timeNum = 0;
			}
			//SetGraphX_Axis(GRAPHDISP_GRAPH1,&Graph);		
			//SetGraphX_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp);
			//SetGraphY_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp); 
			if(controlTime == TestPara1.SampleDelay*0.1)
			{
			*(Graph_Temp.pCurveArray->pDotX++) = *(Graph_Temp.pCurveArray->pDotX++) + TestPara1.SampleDelay*0.001; 		 //环境参数时间
		/*	*((Graph_Temp.pCurveArray + 1)->pDotX++) = (controlTime*0.001) * TestPara1.SampleDelay*0.001;  
			*((Graph_Temp.pCurveArray + 2)->pDotX++) = (controlTime*0.001) * TestPara1.SampleDelay*0.001;*/
			printf("%f\n", *(Graph_Temp.pCurveArray->pDotX-1)); 
			controlTime = 0;
			}
			
		
			break; 
	}
	return 0;
}
