//==============================================================================
//
//==============================================================================

//==============================================================================
// Include files

//#include "SetPanelCB.h"
#include <userint.h>
#include "MainPanel.h"
#include "LoadPanel.h"
#include "SetPanelCB.h"
#include <ansi_c.h>
#include "V-I Configuration.h"
#include "SetPanel.h"
//==============================================================================
// Constants

//==============================================================================
// Types

#define VAL_TEXTBG                        0xF0F0F0L    //未被选中背景色
#define VAL_TEXTBG_PRESSED                0x065279L    //被选中背景色

//==============================================================================
// Static global variables
int graph2Color1Old;
int graph2Color2Old;
int graph2Color3Old;
int smu1ClrOld;
int smu2ClrOld;
//==============================================================================
// Static functions

//==============================================================================
// Global variables
char ProjectSavePath[512]="C:\\Sinoagg\\SA3102\\Users\\";	   //本地工作目录Dir 
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?

int CVICALLBACK SettingsCB (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			RemovePopup(setPanel);
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU1CLR, smu1ClrOld);
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU2CLR, smu2ClrOld);
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR1, graph2Color1Old); 
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR2, graph2Color2Old); 
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR3, graph2Color3Old);
			break;
	}
	return 0;
}

int CVICALLBACK PrjBtnCallback (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			SetPanelPos(ENVTPanel, 5 , 160);
			DisplayPanel(ENVTPanel); 
			
			HidePanel (hSettingsGraphPanel);
			HidePanel (aboutPanel); 

			SetCtrlAttribute (panel, SETTINGS_PRJBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG_PRESSED);   //project背景色
	     	SetCtrlAttribute (panel, SETTINGS_PRJBTN, ATTR_TEXT_COLOR, VAL_WHITE);              //project文本颜色
			
			SetCtrlAttribute (panel, SETTINGS_GRAPHBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);         //graph背景色
	     	SetCtrlAttribute (panel, SETTINGS_GRAPHBTN, ATTR_TEXT_COLOR, VAL_BLACK);            //graph文本颜色
			
			SetCtrlAttribute (panel, SETTINGS_ABOUTBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);         //about背景色
	     	SetCtrlAttribute (panel, SETTINGS_ABOUTBTN, ATTR_TEXT_COLOR, VAL_BLACK);            //about文本颜色
																								 
			break;

	}
	return 0;
}

int CVICALLBACK GraphBtnCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			SetPanelPos(hSettingsGraphPanel, 5, 160);
			DisplayPanel(hSettingsGraphPanel);
			
			HidePanel (ENVTPanel); 
			HidePanel (aboutPanel);

			SetCtrlAttribute (panel, SETTINGS_GRAPHBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG_PRESSED);   //graph背景色
	     	SetCtrlAttribute (panel, SETTINGS_GRAPHBTN, ATTR_TEXT_COLOR, VAL_WHITE);              //graph文本颜色
			
			SetCtrlAttribute (panel, SETTINGS_PRJBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);             //project背景色
	     	SetCtrlAttribute (panel, SETTINGS_PRJBTN, ATTR_TEXT_COLOR, VAL_BLACK);                //project文本颜色
			
			SetCtrlAttribute (panel, SETTINGS_ABOUTBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);           //about背景色
	     	SetCtrlAttribute (panel, SETTINGS_ABOUTBTN, ATTR_TEXT_COLOR, VAL_BLACK);              //about文本颜色
			
			break;

	}
	return 0;
}



int CVICALLBACK AboutBtnCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			 
			SetPanelPos(aboutPanel, 5, 160);
			DisplayPanel(aboutPanel);
			
		    HidePanel (ENVTPanel); 
			HidePanel (hSettingsGraphPanel);

			SetCtrlAttribute (panel, SETTINGS_ABOUTBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG_PRESSED);   //about背景色
	     	SetCtrlAttribute (panel, SETTINGS_ABOUTBTN, ATTR_TEXT_COLOR, VAL_WHITE);              //about文本颜色
			
			SetCtrlAttribute (panel, SETTINGS_GRAPHBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);           //graph背景色
	     	SetCtrlAttribute (panel, SETTINGS_GRAPHBTN, ATTR_TEXT_COLOR, VAL_BLACK);              //graph文本颜色
			
			SetCtrlAttribute (panel, SETTINGS_PRJBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);             //project背景色
	     	SetCtrlAttribute (panel, SETTINGS_PRJBTN, ATTR_TEXT_COLOR, VAL_BLACK);                //project文本颜色
			break;

	}
	return 0;
}
int CVICALLBACK ProCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			if(DirSelectPopup ("E:\\GitWorkplace\\Sinoagg\\Software\\", "Select Directory", 1, 1, ProjectSavePath)>0)     
			{
			 SetCtrlVal(panel, ENVT_PROPATH, ProjectSavePath); 
			}
			else 
			{
				return -1;
			}
			break;
	}
	return 0;
}



int CVICALLBACK CancelCallback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			RemovePopup(setPanel); 
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU1CLR, smu1ClrOld);
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU2CLR, smu2ClrOld);
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR1, graph2Color1Old); 
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR2, graph2Color2Old); 
			SetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR3, graph2Color3Old);
			break;

	}
	return 0;
}

int CVICALLBACK OkCallback (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			RemovePopup(setPanel); 
			break;
	}
	return 0;
}
