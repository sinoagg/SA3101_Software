//==============================================================================
//
// Title:		Curve.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/12 at 15:27:18 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <ansi_c.h>
#include "Curve.h"


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

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int curveInit(int curveIndex, int numOfTotalDots, Curve_TypeDef* pCurve)
{
	pCurve->pDotXHead = (float *)malloc(numOfTotalDots * sizeof(float));
	if(pCurve->pDotXHead == NULL) return -1;
	pCurve->pDotYHead = (float *)malloc(numOfTotalDots * sizeof(float));
	if(pCurve->pDotYHead == NULL) return -1;
	pCurve->curveIndex=curveIndex;
	pCurve->numOfTotalDots=numOfTotalDots;
	pCurve->pDotX=pCurve->pDotXHead;
	pCurve->pDotY=pCurve->pDotYHead;
	pCurve->numOfPlotDots=0;		
	pCurve->numOfDotsToPlot=0;
	pCurve->pDotXPlot=pCurve->pDotX;				//画图的指针指向数据的指针
	pCurve->pDotYPlot=pCurve->pDotY;				//画图的指针指向数据的指针 	
	
	
	
	return 0;
}

int curveDeinit(Curve_TypeDef* pCurve)
{
	free(pCurve->pDotXHead);
	free(pCurve->pDotYHead);
	
	return 0;
}

//int GraphDeinit(Graph_TypeDef* pGraph)
//{
//	int i=0;
//	for(i=0;i<(pGraph->numOfCurve);i++)
//	{
//		curveDeinit(pGraph->pCurveArray++); 	
//	}
//	return 0;
//}
