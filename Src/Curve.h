//==============================================================================
//
// Title:		Curve.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/12 at 15:27:18 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Curve_H__
#define __Curve_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files
//==============================================================================
// Constants

//==============================================================================
// Types
typedef struct
{
	int pointStyle;			//����ʽ
	int plotStyle;			//��ͼ��ʽ
	int lineStyle;			//������ʽ
	int smu1lineColor;			//������ɫ
	int smu2lineColor;
	int tempColor; 
	int humColor;
	int pressColor;
}CurveAttrTypeDef;

typedef struct
{
	int curveIndex;									//��������
	int numOfTotalDots;								//���ۼ����ܵ���
	int numOfPlotDots;								//�ѻ�������
	int numOfDotsToPlot;							//��Ҫ��������
	int numofSmu1RxDots;							//������1�Ĵ��ڽ��ո���
	int numofSmu2RxDots;							//������2�Ĵ��ڽ��ո���
	float* pDotXHead;								//��X���������׵�ַ   
	float* pDotYHead;								//��Y���������׵�ַ  
	float* pDotX;									//��ǰ���յ�X�����ַ
	float* pDotY;									//��ǰ���յ�Y�����ַ
	float* pDotXPlot;								//��ǰ���ߵ�X����
	float* pDotYPlot;								//��ǰ���ߵ�Y����
}Curve_TypeDef;

typedef struct
{
	float x;										//x on X-Axis
	float y;										//y on Y-Axis
}Dot_TypeDef;
//==============================================================================
// External variables

//==============================================================================
// Global functions
extern CurveAttrTypeDef CurveAttr; //DLP   
int curveInit(int curveIndex, int numOfDots, Curve_TypeDef* pCurve);
int curveDeinit(Curve_TypeDef* pCurve);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Curve_H__ */