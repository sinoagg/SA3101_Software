//==============================================================================
//
// Title:		SampleCfgPanel.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/10 at 20:38:31 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <userint.h> 
#include "SampleCfgPanel.h"
#include "Sample Configuration.h"
#include "LoadPanel.h" 

int hBasicSamplePanelHandle; 
//==============================================================================
// Constants

//==============================================================================
// Types



#define VERY_SLOW		0		//最低速
#define SLOW			1		//低速挡
#define MID				2		//中速档
#define FAST			3		//高速
#define VERY_FAST		4		//最高速
#define CUSTOMIZE		15		//自定义

		  
//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//SampleCfg_TypeDef SampleCfg;
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
//int GetSampleCfg (int panelHandle)
//{
//	int temp;
//	if(GetCtrlVal(panelHandle, SAMPLE_CFG_SRT_QUIET, &temp)<0)
//		return -1;
//	SampleCfg.cfgQuiteTime=temp;
//	if(GetCtrlVal(panelHandle, SAMPLE_CFG_SRT_INTERVAL, &temp)<0)
//		return -1;
//	SampleCfg.cfgTimeInterval=temp;
//	if(GetCtrlVal(panelHandle, SAMPLE_CFG_STR_RUNTIME, &temp)<0)
//		return -1;
//	SampleCfg.cfgRunTime=temp;
//	if(GetCtrlVal(panelHandle, SAMPLE_CFG_STR_RATE, &temp)<0)
//		return -1;
//	SampleCfg.cfgSampleRate=temp;
//	if(GetCtrlVal(panelHandle, SAMPLE_CFG_STR_NUMBER, &temp)<0)
//		return -1;
//	SampleCfg.cfgSampleNum=temp;
//	if(GetCtrlVal(panelHandle, SAMPLE_CFG_STR_SETTING, &temp)<0)
//		return -1;
//	SampleCfg.cfgRangeSetting=temp;
//	
//	return 0;
//	
//}
int CVICALLBACK AdvancSetCallback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch(event){
	
		case EVENT_LEFT_CLICK_UP:
			SetPanelPos(hAdvanceSamplePanel, 105, 1600);
			DisplayPanel(hAdvanceSamplePanel);
			break;
	}
	return 0;
}

int CVICALLBACK BasicSetCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch(event){
		case EVENT_LEFT_CLICK_UP:
			    SetPanelPos(hBasicSamplePanel, 105, 1600);
				DisplayPanel(hBasicSamplePanel);
			break;
	}
	return 0;
}

int CVICALLBACK RampleRateCallback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
    
	switch (event)
	{
		case  EVENT_VAL_CHANGED:                            
			int sampleRate;  
			GetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_SWEEOSPEED, ATTR_CTRL_VAL,&sampleRate); 
			if(sampleRate == VERY_FAST)
			{
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_VAL,2000 );
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_VAL,80 );  
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_MODE, VAL_INDICATOR);
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_MODE, VAL_INDICATOR);  
			}else if(sampleRate == FAST)
			{
				SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE,2000);
			    SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER,200); 
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_MODE, VAL_INDICATOR);
			   SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_MODE, VAL_INDICATOR);  
			}
			else if(sampleRate == MID) 
			{
				SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE,2000);
			    SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER,400); 
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_MODE, VAL_INDICATOR);
			   SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_MODE, VAL_INDICATOR);  
	
			}
			else if(sampleRate == SLOW)
			{
				SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE,2000);
			    SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER,1000);
				SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_MODE, VAL_INDICATOR);
			   SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_MODE, VAL_INDICATOR);  
			} 
			else if(sampleRate == VERY_SLOW)
			{
				 SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE,2000);
			     SetCtrlVal(hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER,2000); 
				 SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_MODE, VAL_INDICATOR);
			   SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_MODE, VAL_INDICATOR);  
			}else if( sampleRate == CUSTOMIZE)
			{
			   SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLERATE, ATTR_CTRL_MODE, VAL_HOT);
			   SetCtrlAttribute (hAdvanceSamplePanel,SAMPLE_ADV_SAMPLENUMBER, ATTR_CTRL_MODE, VAL_HOT);   
			}
			break;
	}
	return 0;
}



