//==============================================================================
//
// Title:		I_T_Panel.c
// Purpose:		A short description of the implementation.
//
// Created on:	2018/7/14 at 10:17:00 by.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include "I-T Configuration.h"
#include <userint.h>
#include "I_T_Panel.h"
#include "ExpListPanel.h"
#include "LoadPanel.h"
//==============================================================================
// Constants
#define VAL_BG                        0xA9A9A9L    //未被选中时的背景色
#define VAL_BG_Pressed                0xA5B8D2L    //被选中时的背景色
#define FOCUS	0
#define UNFOCUS 1
//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
////ITCfg_TypeDef ITCfg1; 
//ITCfg_TypeDef ITCfg2;
ExpPanelTypeDef I_T_Panel1={0, PANEL_I_T_START1, PANEL_I_T_START1, PANEL_I_T_START1, PANEL_I_T_START1, PANEL_I_T_START1, PANEL_I_T_START1};
ExpPanelTypeDef I_T_Panel2={0, PANEL_I_T_START2, PANEL_I_T_START2, PANEL_I_T_START2, PANEL_I_T_START2, PANEL_I_T_START2, PANEL_I_T_START2};
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
static void ITSetSMU1Disp(int panel, char focus)
{
	if(focus==FOCUS)
	{
		SetCtrlAttribute (panel, PANEL_I_T_BG_SMU1, ATTR_PICT_BGCOLOR, VAL_BG_Pressed);
	}
	else
	{
		SetCtrlAttribute (panel, PANEL_I_T_BG_SMU1, ATTR_PICT_BGCOLOR, VAL_BG);  
	}
}

static void ITSetSMU2Disp(int panel, char focus)
{
	if(focus==FOCUS)
	{
		SetCtrlAttribute (panel, PANEL_I_T_BG_SMU2, ATTR_PICT_BGCOLOR, VAL_BG_Pressed);		
	}
	else
	{
		SetCtrlAttribute (panel, PANEL_I_T_BG_SMU2, ATTR_PICT_BGCOLOR, VAL_BG);
	}
}
int CVICALLBACK ITSMU1DecoCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
			 ITSetSMU1Disp(ITPanel, FOCUS);
			 ITSetSMU2Disp(ITPanel, UNFOCUS);
			break;
		case EVENT_LEFT_CLICK_UP:
			 ITSetSMU1Disp(ITPanel, FOCUS);
			 ITSetSMU2Disp(ITPanel, UNFOCUS);
			break;
	}
	return 0;
}

int CVICALLBACK ITSMU2DecoCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
			 ITSetSMU1Disp(ITPanel, UNFOCUS);
			 ITSetSMU2Disp(ITPanel, FOCUS);
			break;		
		case EVENT_LEFT_CLICK_UP:
			 ITSetSMU1Disp(ITPanel, UNFOCUS);
			 ITSetSMU2Disp(ITPanel, FOCUS);
			break;
	}
	return 0;
}

static void GetTestPara(ExpPanelTypeDef* pExpPanel, TestParaTypeDef* pTestPara) //得到面板中用户设置的参数
{
	GetCtrlVal(pExpPanel->panelHandle, pExpPanel->VdStartID,&(pTestPara->Voltage_Start));
	GetCtrlVal(pExpPanel->panelHandle, pExpPanel->VdStopID, &(pTestPara->Voltage_Stop));
	GetCtrlVal(pExpPanel->panelHandle, pExpPanel->VdStepID, &(pTestPara->Voltage_Step));
	
	GetCtrlVal(pExpPanel->panelHandle, pExpPanel->VgStartID, &(pTestPara->Current_Start));
	GetCtrlVal(pExpPanel->panelHandle, pExpPanel->VgStopID, &(pTestPara->Current_Stop));
	GetCtrlVal(pExpPanel->panelHandle, pExpPanel->VgStepID, &(pTestPara->Current_Step));

	GetCtrlVal(hBasicSamplePanel, SAMPLE_CFG_QUIETTIME, &(pTestPara->quietTime));   //所有采样配置都是兼容的
	GetCtrlVal(hBasicSamplePanel, SAMPLE_CFG_TIMESTEP, &(pTestPara->timeStep));
	GetCtrlVal(hBasicSamplePanel, SAMPLE_CFG_RUNTIME, &(pTestPara->runTime));
	
	GetCtrlVal(hBasicSamplePanel, SAMPLE_CFG_SWEEOSPEED, &(pTestPara->sweepSpeed));			//采样速度
	
	GetCtrlVal(hAdvanceSamplePanel, SAMPLE_ADV_SAMPLERATE, &(pTestPara->sampleRate));
	GetCtrlVal(hAdvanceSamplePanel, SAMPLE_ADV_SAMPLENUMBER, &(pTestPara->sampleNumber));
	
	
	GetCtrlVal(hBasicSamplePanel, SAMPLE_CFG_RANGESETTING, &(pTestPara->rangeMode));		
	GetCtrlVal(hBasicSamplePanel, SAMPLE_CFG_SAMPLE_DELAY, &(pTestPara->SampleDelay));		//采样延时
	
	GetCtrlVal(hAdvanceSamplePanel, SAMPLE_ADV_OutputRelay,&(pTestPara->outputRelay));
	GetCtrlVal(hAdvanceSamplePanel, SAMPLE_ADV_MAXRANGE, &(pTestPara->maxRange));
	GetCtrlVal(hAdvanceSamplePanel, SAMPLE_ADV_MINRANGE, &(pTestPara->minRange));

}

/////*


////	功能：得到 V_T V_I  模式下用户在面板的设置值

////*/
////int Get_V_I_Cfg (int panelHandle)
////{
////	int temp;
////	
////	//得到源表 1 的 电流   开始  结束 间隔参数	  
////	if(GetCtrlVal(panelHandle, PANEL_I_T_START1, &temp)<0)
////		return -1;
////	ITCfg1.cfgIT_Current_start=temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_STOP1, &temp)<0)
////		return -1;
////	ITCfg1.cfgIT_Current_stop=temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_GAP1, &temp)<0)
////		return -1;
////	ITCfg1.cfgIT_Current_step = temp;
////	
////	
////	//得到源表 2 的 电流   开始  结束 间隔参数 	
////	if(GetCtrlVal(panelHandle, PANEL_I_T_START2, &temp)<0)
////		return -1;
////	ITCfg2.cfgIT_Current_start = temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_STOP2, &temp)<0)
////		return -1;
////	ITCfg2.cfgIT_Current_stop = temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_GAP2, &temp)<0)
////		return -1;
////	ITCfg2.cfgIT_Current_step = temp;
////	
////	return 0;
////}
/////*


////	功能：得到 I_T  I_V  模式下用户在面板的设置值

////*/
////int Get_IT_Cfg (int panelHandle)
////{
////	int temp;
////	
////	//得到源表 1 的 电压   开始  结束 间隔参数	  
////	if(GetCtrlVal(panelHandle, PANEL_I_T_START1, &temp)<0)		  
////		return -1;
////	ITCfg1.cfgIT_Voltage_start=temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_STOP1, &temp)<0)
////		return -1;
////	ITCfg1.cfgIT_Voltage_stop=temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_GAP1, &temp)<0)
////		return -1;
////	ITCfg1.cfgIT_Voltage_step = temp;
////	
////	//得到源表 2 的 电压   开始  结束 间隔参数 	
////	if(GetCtrlVal(panelHandle, PANEL_I_T_START2, &temp)<0)
////		return -1;
////	ITCfg2.cfgIT_Voltage_start = temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_STOP2, &temp)<0)
////		return -1;
////	ITCfg2.cfgIT_Voltage_stop = temp;
////	if(GetCtrlVal(panelHandle, PANEL_I_T_GAP2, &temp)<0)
////		return -1;
////	ITCfg2.cfgIT_Voltage_step = temp;
////	
////	return 0;
////}

