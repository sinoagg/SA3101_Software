//==============================================================================
//
// Title:		ExperimentList.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/11 at 9:55:13 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <ansi_c.h>
#include <userint.h>
#include "Experiment List.h"
#include "ExpListPanel.h"
#include "Id-Vds Configuration.h"
#include "Id-Vgs Configuration.h"
#include "I-T Configuration.h"
#include "I_T_Panel.h"
#include "MainPanel.h"
#include "MainPanelCb.h" 
#include "EnvironmentDisp.h"
#include "Sample Configuration.h"
#include "GraphDisp.h"  
#include "LoadPanel.h" 
//==============================================================================
// Constants
#define TWO_TERMINAL 0
#define EXP_I_V 1
#define EXP_V_I 2
#define EXP_I_T 3
#define EXP_V_T 4
#define EXP_R_T 5
#define THREE_TERMINAL 12 
#define FOUR_TERMINAL 16
#define EXP_ID_VDS 17
#define EXP_ID_VGS 18
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
static void DispRuntime(int display)
{ 
	SetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_TXT_RUNTIME, ATTR_VISIBLE, display);
	SetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_RUNTIME, ATTR_VISIBLE, display); 
	SetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_TXT_S, ATTR_VISIBLE, display);
}
static void ExpList(int display)
{
	SetCtrlAttribute (mainPanel, MAIN_PANEL_ANALYZE, ATTR_DIMMED,display);	   //禁用analyze
	
	SetPanelPos(hBasicSamplePanel, 105, 1600);
	SetPanelAttribute (hBasicSamplePanel, ATTR_VISIBLE, !(display));
	
	SetPanelPos(hEnvCfgPanel, 556, 1600);
	SetPanelAttribute (hEnvCfgPanel, ATTR_VISIBLE, !(display));
	
	HidePanel(hEnvResultPanel);						//隐藏analyze下的面板
	HidePanel(hResultDispPanel);
	HidePanel(hAdvanceSamplePanel);  
}

int CVICALLBACK ExpListCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int index; 
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			DisplayImageFile (mainPanel, MAIN_PANEL_SELECT, "Resource\\Select.ico");
			DisplayImageFile (mainPanel, MAIN_PANEL_CONFIGURE, "Resource\\Configure_pressed.ico"); 
			DisplayImageFile (mainPanel, MAIN_PANEL_ANALYZE, "Resource\\Analyze.ico");
			
			GetCtrlIndex(expListPanel, EXP_LIST_EXPLIST, &index);
			if(index==TWO_TERMINAL)
			{
				SetPanelPos(TwoTerminalPanel, 105, 305);
				DisplayPanel(TwoTerminalPanel);
				ExpList(1);
			}
			else if(index==EXP_I_V)
			{
				SetPanelPos(IVPanel, 105, 305);
				DisplayPanel(IVPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 1);
				ExpList(0);
				DispRuntime(0);
			}
			else if(index==EXP_V_I)
			{
				SetPanelPos(VIPanel, 105, 305);
				DisplayPanel(VIPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 0); 
				ExpList(0);
				DispRuntime(0);
			}
			else if(index==EXP_I_T)
			{
				SetPanelPos(ITPanel, 105, 305);
				DisplayPanel(ITPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 1);  
				ExpList(0); 
				DispRuntime(1);
			}
			else if(index==EXP_V_T)
			{
				SetPanelPos(VTPanel, 105, 305);
				DisplayPanel(VTPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 0); 
				ExpList(0);
				DispRuntime(1);
			}
			else if(index==EXP_R_T)
			{
				SetPanelPos(RTPanel, 105, 305);
				DisplayPanel(RTPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 0); 
				ExpList(0);
				DispRuntime(1);
			}
			/*else if(index==THREE_TERMINAL)
			{
				SetPanelPos(ThreeTerminalPanel, 105, 305);
				DisplayPanel(ThreeTerminalPanel);
				ExpList(1);
			} 
			else if(index==EXP_ID_VDS)
			{
				SetPanelPos(IdVdPanel, 105, 305);
				DisplayPanel(IdVdPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 0); 
				ExpList(0);
				DispRuntime(1);
			}
			else if(index==EXP_ID_VGS)
			{
				SetPanelPos(IdVgPanel, 105, 305);
				DisplayPanel(IdVgPanel);
				SetCtrlAttribute (hBasicSamplePanel, SAMPLE_ADV_CURRENTMODE, ATTR_VISIBLE, 0); 
				ExpList(0);
				DispRuntime(1);
			}
			else if(index==FOUR_TERMINAL)
			{
				SetPanelPos(FourTerminalPanel, 105, 305);
				DisplayPanel(FourTerminalPanel);
				ExpList(1); 
			}
			else
			{
				ExpList(0);
				DispRuntime(1);
			}
			
			Dispgraph();				//不同模式显示不同的名称和单位*/ 
			
			break;
	}
	return 0;
}
