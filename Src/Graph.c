//==============================================================================
//
// Title:		Graph.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/12 at 13:50:53 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <userint.h>
#include <ansi_c.h>
#include "Curve.h"
#include "Graph.h"
#include "protocol.h" 
#include "LoadPanel.h"
#include "Uart.h"
//==============================================================================
// Constants
#define YAXISRANGE 0.009
//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
Graph_TypeDef pGraph;
Graph_TypeDef* pGraph1;
Graph_TypeDef* pGraph2;
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect
/// HIRET What does your function return?
int graphInit(int graphIndex, int numOfCurve, int numOfDots, Graph_TypeDef* pGraph)
{
	int i;
	pGraph->graphIndex=graphIndex;		//画几个图
	pGraph->numOfCurve=numOfCurve;		//画几条曲线
	Curve_TypeDef* pCurveArray = (Curve_TypeDef *)malloc(numOfCurve * sizeof(Curve_TypeDef)); 	//Curve Array Saving pointers to curve structure  曲线阵列保存指针到曲线结构。
	if(pCurveArray==NULL) return -1;
	pGraph->pCurveArray=pCurveArray;
	for(i=0;i<numOfCurve;i++)		//init curve
	{
		if(curveInit(i, numOfDots, pCurveArray+i)==-1) return -1;
	}
	GraphAttrTypeDef* pGraphAttr = (GraphAttrTypeDef *)malloc(sizeof(Curve_TypeDef));
	if(pGraphAttr==NULL) return -1;
	pGraph->pGraphAttr=pGraphAttr;
	return 0;
}
int GraphDeinit(Graph_TypeDef* pGraph)
{
	int i=0;
	for(i=0;i<(pGraph->numOfCurve);i++)
	{
		curveDeinit(pGraph->pCurveArray++); 	
	}
	return 0;
}
void SetGraphX_Axis_TEMP(int control,Graph_TypeDef* pGraph)
{
	if(pGraph->pCurveArray->numOfPlotDots > 0)  
	{
		switch(TestPara1.testMode)
		{
			case NO_SWEEP_IV:
			case NO_SWEEP_VI:
				break;
			case NO_SWEEP_IT:
			case NO_SWEEP_RT:
			case NO_SWEEP_VT: 
				if(pGraph->pGraphAttr->xAxisTail <= pGraph->pCurveArray->numOfPlotDots * TestPara1.TotalDelay)												//已画点数*Step ===》实际x轴长度
				{  	
					pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
					SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, pGraph->pGraphAttr->xAxisTail);										//设置  X  轴的范围
				}
				break;
		}
	}
}
void SetGraphX_Axis(int control,Graph_TypeDef* pGraph)		   
{	
	if(pGraph->pCurveArray->numOfPlotDots > 1 || (pGraph->pCurveArray + 1)->numOfPlotDots > 1)
	{
		switch(TestPara1.testMode)
		{
			case NO_SWEEP_IV:
			case NO_SWEEP_VI:
				//IV坐标轴软件自动调整，X轴不用设置自动调整，初始化已做修改。
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02) && (rowIndex > 2) && (rowIndex2 > 2))
				{
							if(*(pGraph->pCurveArray->pDotY-1) > 0)																												//判断Y轴数据位 为正值
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
								}
							}
							else if(*(pGraph->pCurveArray->pDotY-1) < 0)																											//判断Y轴数据为负值
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
								}
							}
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																												//判断Y轴数据位 为正值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																											//判断Y轴数据为负值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
				
				}else if((select_Addr2 == 0x02) && (rowIndex2 > 2))
				{
					//************************************************* Y轴  **********************************************************//
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																												//判断Y轴数据位 为正值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																											//判断Y轴数据为负值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
				}else if((select_Addr1 == 0x01)&& (rowIndex > 2))
				{
					//************************************************* Y轴  **********************************************************//
						if(*(pGraph->pCurveArray->pDotY-1) > 0)																												//判断Y轴数据位 为正值
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
						else if(*(pGraph->pCurveArray->pDotY-1) < 0)																											//判断Y轴数据为负值
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
				}
				break;
			case NO_SWEEP_IT:
			case NO_SWEEP_RT:
			case NO_SWEEP_VT: 
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02) && (rowIndex > 2) && (rowIndex2 > 2))
				{
							if(*(pGraph->pCurveArray->pDotY-1) > 0)																											//判断Y轴数据位 为正值
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																		//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//设置  Y  轴的范围
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																		//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//设置  Y  轴的范围
								}
							}
							else if(*(pGraph->pCurveArray->pDotY-1) < 0)																									//判断Y轴数据为负值
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																		//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//设置  Y  轴的范围
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																		//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//设置  Y  轴的范围
								}
							}
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																										//判断Y轴数据位 为正值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																									//判断Y轴数据为负值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
				
				
					//************************************************* Y轴  **********************************************************//

						//************************************************* X轴  **********************************************************//
						if(pGraph->pGraphAttr->xAxisTail <= *(pGraph->pCurveArray->pDotX-1))																				//改为真实时间后直接判断传输上来的X时间轴
						{  	
							pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
							SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,pGraph->pGraphAttr->xAxisHead, pGraph->pGraphAttr->xAxisTail);										//设置  X  轴的范围
						}
						else if(pGraph->pGraphAttr->xAxisTail <= *((pGraph->pCurveArray +1)->pDotX-1))																		//改为真实时间后直接判断传输上来的X时间轴
						{  	
							pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
							SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,pGraph->pGraphAttr->xAxisHead, pGraph->pGraphAttr->xAxisTail);										//设置  X  轴的范围
						}
						//************************************************* X轴  **********************************************************// 	
				}else if((select_Addr2 == 0x02) && (rowIndex2 > 2))
				{
					//************************************************* Y轴  **********************************************************//
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																												//判断Y轴数据位 为正值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																									//判断Y轴数据为负值
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
					//************************************************* Y轴  **********************************************************//
	
					//************************************************* X轴  **********************************************************// 
					if(pGraph->pGraphAttr->xAxisTail <= *((pGraph->pCurveArray +1)->pDotX-1))																				//改为真实时间后直接判断传输上来的X时间轴
					{  	
						pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
						SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,pGraph->pGraphAttr->xAxisHead, pGraph->pGraphAttr->xAxisTail);											//设置  X  轴的范围
					}
					//************************************************* X轴  **********************************************************// 	
					
				}else if((select_Addr1 == 0x01) && (rowIndex > 2))
				{
					//************************************************* Y轴  **********************************************************//
						if(*(pGraph->pCurveArray->pDotY-1) > 0)																												//判断Y轴数据位 为正值
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
						else if(*(pGraph->pCurveArray->pDotY-1) < 0)																											//判断Y轴数据为负值
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 大于 Y轴 坐标 的 上限最大值
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//判断Y轴的数值是否 小于 Y轴 坐标 的 下限最大值
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//设置  Y  轴的范围
							}
						}
					//************************************************* Y轴  **********************************************************//
					
					//************************************************* X轴  **********************************************************//
			
						if(pGraph->pGraphAttr->xAxisTail <= *(pGraph->pCurveArray->pDotX-1))																				//真实时间修改后自动判断坐标轴扩展。
						{  	
							pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
							SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,pGraph->pGraphAttr->xAxisHead, pGraph->pGraphAttr->xAxisTail);										//设置  X  轴的范围
						}
					//************************************************* X轴 **********************************************************//
				}		
				break;
		}
	}
}
void SetGraphY_Axis_TEMP(int control,Graph_TypeDef* pGraph)
{
	if(pGraph->pCurveArray->numOfPlotDots > 0)
	{
		if(pGraph->pGraphAttr->yAxisTail <= *(pGraph->pCurveArray->pDotY-1))		//温度值
		{
			//pGraph->pGraphAttr->yAxisTail=pGraph->pGraphAttr->yAxisTail+30;
			pGraph->pGraphAttr->yAxisTail = (*(pGraph->pCurveArray->pDotY-1))*1.3;
			SetAxisScalingMode(graphDispPanel,control , VAL_LEFT_YAXIS  , VAL_MANUAL, 0, pGraph->pGraphAttr->yAxisTail);
		}
	}
}
