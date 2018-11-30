//==============================================================================
//
// Title:		ProjectPanelCB.c
// Purpose:		A short description of the implementation.
//
// Created on:	2018/7/27 at 13:22:00 by 
// Copyright:    . All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <userint.h>
#include "ProjectPanel.h"
#include "LoadPanel.h"
#include "Result Menu.h"
#include "MainPanelCb.h"
#include "I_T_Panel.h"
//==============================================================================
// Constants
#define CHANGECOLOR 	0x94CEFF //浅蓝
#define COLOR  			0x065279 //深蓝
#define SEARCHCOLOR 	0xB2C9D5 //不可用颜色
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

int selectedPrjIndex=0;			//当前选中的项目序号 
int selectPanel;   	

static void DiscardAllPrjPanel(PrjHandleTypeDef *pSingleProject)
{
	int i=0;
	while((pSingleProject+i)->index != -1)
	{
		DiscardPanel((pSingleProject+i)->hSinglePrjPanel);
		i++;
	}
}

static int RecallAllPanelState(char* pConfigSavePath)
{
	RecallPanelState(IVPanel, pConfigSavePath, 1);
	RecallPanelState(VIPanel, pConfigSavePath, 2);
	RecallPanelState(I_T_Panel1.panelHandle, pConfigSavePath, 3); //IT面板的值  
	RecallPanelState(VTPanel, pConfigSavePath, 4);
	RecallPanelState(RTPanel, pConfigSavePath, 5);
	RecallPanelState(hBasicSamplePanel, pConfigSavePath, 10);	   //用户设置 配置值
	RecallPanelState(hAdvanceSamplePanel, pConfigSavePath, 11);  //高级设置面板值
	RecallPanelState(hEnvCfgPanel, pConfigSavePath, 14);
	RecallPanelState(ENVTPanel, pConfigSavePath, 15);
	RecallPanelState(hSettingsGraphPanel, pConfigSavePath, 16);
	
	return 0;
}
int CVICALLBACK TXT_OpenPrjCallback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			RecallAllPanelState(pFileLable[selectedPrjIndex]->pFileName);//load all panel and other parameters
			
			DiscardAllPrjPanel(SingleProject);
			RemovePopup (proPanel);
			break;
	}
	return 0;

}
static void SelectProject(int panel, int select)
{
	selectPanel=GetActivePanel();
	if(select)
	{
		SetPanelAttribute (panel, ATTR_BACKCOLOR, CHANGECOLOR);	 //选中某一工程面板
		SetCtrlAttribute (panel, DEFPANEL_CANVAS, ATTR_PICT_BGCOLOR, COLOR);	
	}
	else
	{
		SetPanelAttribute (panel, ATTR_BACKCOLOR, VAL_WHITE);	 //取消选中
		SetCtrlAttribute (panel, DEFPANEL_CANVAS, ATTR_PICT_BGCOLOR, VAL_WHITE);
	}
}

static char GetPanelIndex(int panel)
{
	int top;
	GetPanelAttribute(panel, ATTR_TOP, &top);
	return (char)((top)/117);				//减去偏置，除以单个高度
}

int CVICALLBACK PrjSelectCallback (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			//SetPanelAttribute (panel, ATTR_BACKCOLOR, CHANGECOLOR);
			//SetCtrlAttribute (panel, DEFPANEL_CANVAS, ATTR_PICT_BGCOLOR, COLOR);
			SetCtrlAttribute (proPanel, PROPANEL_PIC_OPENPRJ, ATTR_DIMMED, 0);   //启用openproject
			SetCtrlAttribute (proPanel, PROPANEL_TXT_OPENPRJ, ATTR_DIMMED, 0);
			
			SelectProject(panel, 1);
			selectedPrjIndex=GetPanelIndex(panel);
			int i=0;
			while(SingleProject[i].index!=-1)
			{
				if(i!=selectedPrjIndex)
					SelectProject(SingleProject[i].hSinglePrjPanel, 0);
				i++;
			}
			break;
	}
	return 0;
}


int CVICALLBACK SearchCallback (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:    
			if(selectPanel)	 
			{
				SelectProject(selectPanel,0);											//取消当前选中状态  
				SetCtrlAttribute (proPanel,PROPANEL_PIC_OPENPRJ , ATTR_DIMMED, 1);	    //禁用openproject
				SetCtrlAttribute (proPanel,PROPANEL_TXT_OPENPRJ , ATTR_DIMMED, 1); 
				selectPanel=0;
			}
			break;
	}
	return 0;
}


int CVICALLBACK PIC_ExitPrjCallback2 (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	if(event==EVENT_LEFT_CLICK)
	{
		DiscardAllPrjPanel(SingleProject);
		RemovePopup (proPanel);  
	}
	return 0;
}

int CVICALLBACK PIC_ExitPrjCallback (int panel, int event, void *callbackData,
									 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:

				DiscardAllPrjPanel(SingleProject);
				RemovePopup (proPanel);  
			
			break;
			
	}
	return 0;
}
