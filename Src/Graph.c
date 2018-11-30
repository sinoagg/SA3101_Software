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
	pGraph->graphIndex=graphIndex;		//������ͼ
	pGraph->numOfCurve=numOfCurve;		//����������
	Curve_TypeDef* pCurveArray = (Curve_TypeDef *)malloc(numOfCurve * sizeof(Curve_TypeDef)); 	//Curve Array Saving pointers to curve structure  �������б���ָ�뵽���߽ṹ��
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

	if(pGraph->pGraphAttr->xAxisTail <= pGraph->pCurveArray->numOfPlotDots * TestPara1.TotalDelay)												//�ѻ�����*Step ===��ʵ��x�᳤��
	{  	
		pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
		SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, pGraph->pGraphAttr->xAxisTail);										//����  X  ��ķ�Χ
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
				//IV����������Զ�������X�᲻�������Զ���������ʼ�������޸ġ�
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02) && (rowIndex > 2) && (rowIndex2 > 2))
				{
							if(*(pGraph->pCurveArray->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
								}
							}
							else if(*(pGraph->pCurveArray->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
								}
							}
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
				
				}else if((select_Addr2 == 0x02) && (rowIndex2 > 2))
				{
					//************************************************* Y��  **********************************************************//
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
				}else if((select_Addr1 == 0x01)&& (rowIndex > 2))
				{
					//************************************************* Y��  **********************************************************//
						if(*(pGraph->pCurveArray->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
						else if(*(pGraph->pCurveArray->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
				}
				break;
			case NO_SWEEP_IT:
			case NO_SWEEP_RT:
			case NO_SWEEP_VT: 
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02) && (rowIndex > 2) && (rowIndex2 > 2))
				{
							if(*(pGraph->pCurveArray->pDotY-1) > 0)																											//�ж�Y������λ Ϊ��ֵ
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//����  Y  ��ķ�Χ
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//����  Y  ��ķ�Χ
								}
							}
							else if(*(pGraph->pCurveArray->pDotY-1) < 0)																									//�ж�Y������Ϊ��ֵ
							{
								if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
								{  	
									pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//����  Y  ��ķ�Χ
								}
								if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
								{
									pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
									SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);	//����  Y  ��ķ�Χ
								}
							}
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																										//�ж�Y������λ Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																									//�ж�Y������Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
				
				
					//************************************************* Y��  **********************************************************//

						//************************************************* X��  **********************************************************//
						if(pGraph->pGraphAttr->xAxisTail <= *(pGraph->pCurveArray->pDotX-1))																				//��Ϊ��ʵʱ���ֱ���жϴ���������Xʱ����
						{  	
							pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
							SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, pGraph->pGraphAttr->xAxisTail);										//����  X  ��ķ�Χ
						}
						else if(pGraph->pGraphAttr->xAxisTail <= *((pGraph->pCurveArray +1)->pDotX-1))																		//��Ϊ��ʵʱ���ֱ���жϴ���������Xʱ����
						{  	
							pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
							SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, pGraph->pGraphAttr->xAxisTail);										//����  X  ��ķ�Χ
						}
						//************************************************* X��  **********************************************************// 	
				}else if((select_Addr2 == 0x02) && (rowIndex2 > 2))
				{
					//************************************************* Y��  **********************************************************//
						if(*((pGraph->pCurveArray + 1)->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
						else if(*((pGraph->pCurveArray + 1)->pDotY-1) < 0)																									//�ж�Y������Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *((pGraph->pCurveArray + 1)->pDotY-1) - *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *((pGraph->pCurveArray + 1)->pDotY-1)) 																		//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *((pGraph->pCurveArray + 1)->pDotY-1) + *((pGraph->pCurveArray + 1)->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
					//************************************************* Y��  **********************************************************//
	
					//************************************************* X��  **********************************************************// 
					if(pGraph->pGraphAttr->xAxisTail <= *((pGraph->pCurveArray +1)->pDotX-1))																				//��Ϊ��ʵʱ���ֱ���жϴ���������Xʱ����
					{  	
						pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
						SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, pGraph->pGraphAttr->xAxisTail);											//����  X  ��ķ�Χ
					}
					//************************************************* X��  **********************************************************// 	
					
				}else if((select_Addr1 == 0x01) && (rowIndex > 2))
				{
					//************************************************* Y��  **********************************************************//
						if(*(pGraph->pCurveArray->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
						else if(*(pGraph->pCurveArray->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
						{
							if(pGraph->pGraphAttr->yAxisTail < *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� ���� Y�� ���� �� �������ֵ
							{  	
								pGraph->pGraphAttr->yAxisTail = *(pGraph->pCurveArray->pDotY-1) - *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE;
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
							if(pGraph->pGraphAttr->yAxisHead > *(pGraph->pCurveArray->pDotY-1)) 																			//�ж�Y�����ֵ�Ƿ� С�� Y�� ���� �� �������ֵ
							{
								pGraph->pGraphAttr->yAxisHead = *(pGraph->pCurveArray->pDotY-1) + *(pGraph->pCurveArray->pDotY-1)*YAXISRANGE; 
								SetAxisScalingMode(graphDispPanel, control, VAL_LEFT_YAXIS, VAL_MANUAL,pGraph->pGraphAttr->yAxisHead, pGraph->pGraphAttr->yAxisTail);		//����  Y  ��ķ�Χ
							}
						}
					//************************************************* Y��  **********************************************************//
					
					//************************************************* X��  **********************************************************//
			
						if(pGraph->pGraphAttr->xAxisTail <= *(pGraph->pCurveArray->pDotX-1))																				//��ʵʱ���޸ĺ��Զ��ж���������չ��
						{  	
							pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
							SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, pGraph->pGraphAttr->xAxisTail);										//����  X  ��ķ�Χ
						}
					//************************************************* X��  **********************************************************//
				}		

				break;
		}
	}
}
void SetGraph_Temp_X_Axis(int control,Graph_TypeDef* pGraph)		   
{					 
	switch(TestPara1.testMode)
	{
		case NO_SWEEP_IV:
			break;
		case NO_SWEEP_VI:
			break;
		case NO_SWEEP_IT:
		case NO_SWEEP_RT:
			if(pGraph->pGraphAttr->xAxisTail <= pGraph->pCurveArray->numOfPlotDots * TestPara1.timeStep * 0.001) //�ѻ�����*Step ===��ʵ��x�᳤��
			{  	
				pGraph->pGraphAttr->xAxisTail = pGraph->pGraphAttr->xAxisTail*1.2;
				SetAxisScalingMode(graphDispPanel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL,0, Graph.pGraphAttr->xAxisTail);//���� X  ��ķ�Χ
				
			}
			break;
		case NO_SWEEP_VT:
			break;
	}

}
void SetGraphY_Axis_TEMP(int control,Graph_TypeDef* pGraph)
{
	if(pGraph->pGraphAttr->yAxisTail <= *(pGraph->pCurveArray->pDotY))		//�¶�ֵ
	{
		pGraph->pGraphAttr->yAxisTail=pGraph->pGraphAttr->yAxisTail+30;
		SetAxisScalingMode(graphDispPanel,control , VAL_LEFT_YAXIS  , VAL_MANUAL, 0, pGraph->pGraphAttr->yAxisTail);
	}
}
