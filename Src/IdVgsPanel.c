#include "Id-Vds Configuration.h"
#include <userint.h>
//==============================================================================
//
// Title:		IdVgsPanel.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/11 at 13:36:34 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "IdVgsPanel.h"
#include "LoadPanel.h"
#include "Id-Vgs Configuration.h"
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
IdVgCfg_TypeDef IdVgCfg; 
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
//int GetIdVgCfg (int panelHandle)
//{
//	/*int temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VD_START, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVdstart=temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VD_STOP, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVdstop=temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VD_STEP, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVdstep=temp;*/
//	
//	
//		int temp;
//	/*if(GetCtrlVal(panelHandle, IDVGS_CFG_VG_BIAS, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVdstart=temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VG_COMPL_2, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVdstop=temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VG_BIAS_2, &temp)<0)
//		return -1;*/
//	IdVgCfg.cfgVdstep=temp;
//	
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VG_START, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVgstart=temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VG_STOP, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVgstop=temp;
//	if(GetCtrlVal(panelHandle, IDVGS_CFG_VG_STEP, &temp)<0)
//		return -1;
//	IdVgCfg.cfgVgstep=temp;
//	
//	return 0;
//}

static void IdVgSetGATEDisp(int panel, char focus)
{
	if(focus==FOCUS)
	{
		SetCtrlAttribute (panel, IDVGS_CFG_BG_GATE, ATTR_PICT_BGCOLOR, VAL_BG_Pressed);
	}
	else
	{
		SetCtrlAttribute (panel, IDVGS_CFG_BG_GATE, ATTR_PICT_BGCOLOR, VAL_BG); 
	}
}
static void IdVgSetDRAINDisp(int panel, char focus)
{
	if(focus==FOCUS)
	{
		SetCtrlAttribute (panel, IDVGS_CFG_BG_DRAIN, ATTR_PICT_BGCOLOR, VAL_BG_Pressed);
	}
	else
	{
		SetCtrlAttribute (panel, IDVGS_CFG_BG_DRAIN, ATTR_PICT_BGCOLOR, VAL_BG); 
	}
}

int CVICALLBACK IdVgGATEDecoCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
			IdVgSetGATEDisp(panel,FOCUS);
			IdVgSetDRAINDisp(panel,UNFOCUS); 
			break;	
		case EVENT_LEFT_CLICK_UP:
			IdVgSetGATEDisp(panel,FOCUS);
			IdVgSetDRAINDisp(panel,UNFOCUS); 
			break;
	}
	return 0;
}

int CVICALLBACK IdVgDRAINDecoCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
			IdVgSetGATEDisp(panel,UNFOCUS);
			IdVgSetDRAINDisp(panel,FOCUS); 

			break;
		case EVENT_LEFT_CLICK_UP:
			IdVgSetGATEDisp(panel,UNFOCUS);
			IdVgSetDRAINDisp(panel,FOCUS); 

			break;

	}
	return 0;
}

int CVICALLBACK IdVgDrainModeCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
			IdVgSetGATEDisp(panel,UNFOCUS);
			IdVgSetDRAINDisp(panel,FOCUS); 
			break;
		case EVENT_LEFT_CLICK_UP:
			IdVgSetGATEDisp(panel,UNFOCUS);
			IdVgSetDRAINDisp(panel,FOCUS); 
			break;
		case EVENT_VAL_CHANGED:
		    int val;
		    GetCtrlVal(IdVgPanel,IDVGS_CFG_DRAINMODE,&val);
			if(val==0)
			{
				DisplayImageFile (IdVgPanel, IDVGS_CFG_PIC_DRAIN, "Resource\\V_Step.ico");
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_STOP, ATTR_VISIBLE, 1);
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_VD_STOP, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_MV, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_STEP, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_VD_STEP, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_MV2, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_DECORATION_Drain, ATTR_HEIGHT, 250); 
			}
			else if(val==1)
			{
				DisplayImageFile (IdVgPanel, IDVGS_CFG_PIC_DRAIN, "Resource\\V_Bias.ico");
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_STOP, ATTR_VISIBLE, 0);
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_VD_STOP, ATTR_VISIBLE, 0); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_MV, ATTR_VISIBLE, 0); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_STEP, ATTR_VISIBLE, 0); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_VD_STEP, ATTR_VISIBLE, 0); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_TXT_MV2, ATTR_VISIBLE, 0); 
				SetCtrlAttribute (IdVgPanel, IDVGS_CFG_DECORATION_Drain, ATTR_HEIGHT, 160);
			}
	}
	return 0;
}


