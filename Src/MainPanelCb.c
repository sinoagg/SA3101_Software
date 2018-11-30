//==============================================================================
//
// Title:		MainPanel.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/16 at 15:58:31 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <windows.h>
#include <cvintwrk.h>
#include <utility.h>
#include <cvixml.h>
#include "asynctmr.h"
#include "MainPanelCb.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "LoadPanel.h"
#include "Protocol.h"
#include "Timer.h"
#include "I_T_Panel.h"
#include "SampleCfgPanel.h"
#include "ResultMenuPanel.h"
#include "table.h"
#include "SetPanelCB.h"
#include "File.h"
#include <rs232.h>
#include "MainPanel.h"
#include "main.h"
#include "Tools.h"
#include "SetPanel.h"
#include "Uart.h"
#include "AbnormalDataCache.h"
#include "System_ini.h"
#include "Environment.h"
#include "curve.h" 
#include "Plot.h"

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
#define VAL_TEXTBG               0xF0F0F0L    //未被选中背景色
#define VAL_TEXTBG_PRESSED       0x065279L    //被选中背景
#define VAL_BG_ENABLE            0x065279L 
#define VAL_BG_DISABLE           0xB2C9D5L 
#define MAX_SIZE   1024 
//==============================================================================
// Types

//==============================================================================
// Static global variables
CurveAttrTypeDef CurveAttr;  //DLP
void GetCurveAttr(CurveAttrTypeDef* pCurveAttr); 
//==============================================================================
// Static functions

//==============================================================================
// Global variables
int threadFlag = 0;
CmtThreadFunctionID abnmDCThreadId;  //异常发生时数据缓存函数的线程Id 
int TimerID;
unsigned char measureComPort;						//Serial Com Number
unsigned char controlComPort;
char configSavePath[512]={0};
FileLableTypeDef *pFileLable[64];									//存所有FileLable的指针，最多只能加载一个文件夹下的64个文件
PrjHandleTypeDef SingleProject[64];									
Graph_TypeDef Graph;
Graph_TypeDef Graph_Temp;
char ABC1[11][20] ={"A","B","C","D","E","F","G","H","I","J","K"};
char Table_title_IV[11][20] ={"V1(mV)"	,"I1(A)" ,"V2(mV)"	,"I2(A)" ,"T(℃)","H(%RH)","p(Pa)"};
char Table_title_IT[11][20] ={"t(s)"	,"I1(A)" ,"t(s)"	,"I2(A)" ,"T(℃)","H(%RH)","p(Pa)"};
char Table_title_RT[11][20] ={"t1(s)"	,"R1(Ω)","t2(s)"	,"R2(Ω)","T(℃)","H(%RH)","p(Pa)"};
char Table_title_VT[11][20] ={"t1(s)"	,"V1(mV)","t2(s)"	,"V2(mV)","T(℃)","H(%RH)","p(Pa)"};

char Table_title_VI_A[11][20] ={"I1(A)"     ,"V1(mV)","I2(A)"	,"V2(mV)" ,"T(℃)","H(%RH)","p(Pa)"}; 
char Table_title_VI_mA[11][20] ={"I1(mA)"	,"V1(mV)","I2(mA)"	,"V2(mV)" ,"T(℃)","H(%RH)","p(Pa)"};  
char Table_title_VI_uA[11][20] ={"I1(uA)"	,"V1(mV)","I2(uA)"	,"V2(mV)" ,"T(℃)","H(%RH)","p(Pa)"};  
char Table_title_VI_nA[11][20] ={"I1(nA)"	,"V1(mV)","I2(nA)"	,"V2(mV)" ,"T(℃)","H(%RH)","p(Pa)"};  
char Table_title_VI_pA[11][20] ={"I1(pA)"	,"V1(mV)","I2(pA)"	,"V2(mV)" ,"T(℃)","H(%RH)","p(Pa)"};

static int ledFlag = 0;           //灯的标志位；小马哥写
static int ftp_handle = -1;
static int error; 
static char attrName[MAX_SIZE];
static char getProjectDir[MAX_PATHNAME_LEN];
static char getProjectDirs[MAX_PATHNAME_LEN]; 
static char updateInfoPath[MAX_PATHNAME_LEN];
static char localInfoPath[MAX_PATHNAME_LEN]; 
static char updateSoftFilePath[MAX_PATHNAME_LEN]; 
static char updateHardFilePath[MAX_PATHNAME_LEN]; 

static char *serversSoftVersion = NULL;
static char *currentSoftVersion = NULL;
static char *serversUpdateVersion = NULL;
static char *currentUpdateVersion = NULL;
static char *serversHardVersion = NULL;
static char *currentHardVersion = NULL;

const char defaultUpdateVersion[] = "0.0.0";
static CVIXMLDocument	 hLoDocument = 0;
static CVIXMLDocument    hUpDocument = 0;
static CVIXMLElement	 hRootElem = 0;
static CVIXMLElement    hChildElem = 0;
static CVIXMLAttribute  hSoftwareCurrAttr = 0;
static CVIXMLAttribute  hUpdateCurrAttr = 0; 
static CVIXMLAttribute  hHardCurrAttr = 0; 

//==============================================================================
// Global functions
static void DispRuntime(int display)
{ 
	SetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_TXT_RUNTIME, ATTR_VISIBLE, display);
	SetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_RUNTIME, ATTR_VISIBLE, display); 
	SetCtrlAttribute (hBasicSamplePanel, SAMPLE_CFG_TXT_S, ATTR_VISIBLE, display);
}
//===================================================
//   MAIN_PANEL_Callback
int CVICALLBACK MAIN_PANEL_Callback (int panel, int event, void *callbackData,
									 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if(controlComPort>0)
			CloseCom(controlComPort);
			if(measureComPort>0)
			CloseCom(measureComPort);
//==============================================小马哥写==============================================================================     			
			SaveSystemClose(0);    //软件正常关闭状态为0
//==============================================小马哥写==============================================================================     
			QuitUserInterface(0); 
		break;
	}
	return 0;
}
static void InitSingleProject(PrjHandleTypeDef *pSingleProject)
{
	int i;
	for(i=0;i<64;i++)
	{
		(pSingleProject+i)->index=-1;	
	}
}
void Dispgraph()
{
	int index;
	GetCtrlIndex(expListPanel, EXP_LIST_EXPLIST, &index);
	if(index==EXP_I_V)
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "V(mV)");		  //graph1坐标轴
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_YNAME, "I(A)");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1X, "V");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1Y, "I");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT, "A");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2X, "V");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2Y, "I");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT, "A");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE, 	"Voltage");
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE_2, 	"Voltage");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT_VOL, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT_VOL, "mV");
		
	}
	else if(index==EXP_V_I)
	{
		int val;  
		GetCtrlVal (VIPanel, PANEL_V_I_START1UNIT, &val);
		if(val==0)
		{
			SetCtrlVal (VIPanel, PANEL_V_I_STOP1UNIT, "A");		  //SMU1单位
			SetCtrlVal (VIPanel, PANEL_V_I_GAP1UNIT,  "A");
			SetCtrlVal (VIPanel, PANEL_V_I_STOP2UNIT, "A");		  //SMU2单位
			SetCtrlVal (VIPanel, PANEL_V_I_GAP2UNIT,  "A");
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "I(A)");	//X轴单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "A");	  //实时显示SMU1X单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "A");	  //实时显示SMU2X单位
			
			
		}
		else if(val==1)
		{
			SetCtrlVal (VIPanel, PANEL_V_I_STOP1UNIT, "mA");		//SMU1单位
			SetCtrlVal (VIPanel, PANEL_V_I_GAP1UNIT,  "mA");
			SetCtrlVal (VIPanel, PANEL_V_I_STOP2UNIT, "mA");		//SMU2单位 
			SetCtrlVal (VIPanel, PANEL_V_I_GAP2UNIT,  "mA");
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "I(mA)");	//X轴单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "mA");	  //实时显示SMU1X单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "mA");	  //实时显示SMU2X单位
		}
		else if(val==2)
		{
			SetCtrlVal (VIPanel, PANEL_V_I_STOP1UNIT, "uA");		 //SMU1单位 
			SetCtrlVal (VIPanel, PANEL_V_I_GAP1UNIT,  "uA");
			SetCtrlVal (VIPanel, PANEL_V_I_STOP2UNIT, "uA");		 //SMU2单位
			SetCtrlVal (VIPanel, PANEL_V_I_GAP2UNIT,  "uA");
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "I(uA)");	//X轴单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "uA");	  //实时显示SMU1X单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "uA");	  //实时显示SMU2X单位
		}
		else if(val==3)
		{
			SetCtrlVal (VIPanel, PANEL_V_I_STOP1UNIT, "nA");		 //SMU1单位 
			SetCtrlVal (VIPanel, PANEL_V_I_GAP1UNIT,  "nA");
			SetCtrlVal (VIPanel, PANEL_V_I_STOP2UNIT, "nA");		 //SMU2单位  
			SetCtrlVal (VIPanel, PANEL_V_I_GAP2UNIT,  "nA");
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "I(nA)");	//X轴单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "nA");	  //实时显示SMU1X单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "nA");	  //实时显示SMU2X单位
		}
		else if(val==4)
		{   
			SetCtrlVal (VIPanel, PANEL_V_I_STOP1UNIT, "pA");
			SetCtrlVal (VIPanel, PANEL_V_I_GAP1UNIT,  "pA");
			SetCtrlVal (VIPanel, PANEL_V_I_STOP2UNIT, "pA");
			SetCtrlVal (VIPanel, PANEL_V_I_GAP2UNIT,  "pA");
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "I(pA)");	//X轴单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "pA");	  //实时显示SMU1X单位
			SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "pA");	  //实时显示SMU2X单位
		}
		
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_YNAME, "V(mV)");		//graph1坐标轴
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1X, "I");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1Y, "V");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2X, "I");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2Y, "V");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT, "mV");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE, 	"Current");   //Current
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE_2, 	"Current");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT_VOL, "A");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT_VOL, "A");
		
		
	}
	else if(index==EXP_I_T)
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "t(s)");
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_YNAME, "I(A)");		   //graph1坐标轴
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1X, "t");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "s");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1Y, "I");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT, "A");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2X, "t");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "s");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2Y, "I");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT, "A");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE, 	"Voltage");   //current
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE_2, 	"Voltage");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT_VOL, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT_VOL, "mV");
		
		
	}
	else if(index==EXP_V_T)
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "t(s)");
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_YNAME, "V(mV)");		  //graph1坐标轴
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1X, "t");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "s");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1Y, "V");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2X, "t");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "s");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2Y, "V");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT, "mV");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE, 	"Current");   //Current
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE_2, 	"Current");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT_VOL, "A");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT_VOL, "A");
		
		
	}
	else if(index==EXP_R_T)
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_XNAME, "t(s)");
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1, ATTR_YNAME, "R(Ω)");		   //graph1坐标轴
		
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1X, "t");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1XUNIT, "s");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1Y, "R");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT, "Ω");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2X, "t");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2XUNIT, "s");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2Y, "R");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT, "Ω");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE, 	"Voltage");
		SetCtrlVal (hResultDispPanel, SAMPLE_RX_VOLTAGE_2, 	"Voltage");
		
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU1YUNIT_VOL, "mV");
		SetCtrlVal (hResultDispPanel, SAMPLE_SMU2YUNIT_VOL, "mV");
	}
	else
	{
	}
}
void RunKeyAction()														 //运行按钮按下后产生的一系列动作
{
	SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP, ATTR_DIMMED, 0);       //恢复 停止按钮
	SetCtrlAttribute (mainPanel, MAIN_PANEL_RUN, ATTR_DIMMED,1);         //禁用 开始按钮      
    SetCtrlAttribute (mainPanel, MAIN_PANEL_SAVE, ATTR_DIMMED,1);        //禁用 保存按钮
	SetCtrlAttribute (mainPanel, MAIN_PANEL_SETTINGS, ATTR_DIMMED,1);    //禁用 设置按钮 
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_MEASURE, ATTR_DIMMED, 1);		  //禁用测量按钮 dlp
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		  //dlp
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);		  //dlp
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);		  //dlp
	
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX, ATTR_DIMMED, !(tempVal));		//前面板是否勾选决定后面板是否禁用
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX_2, ATTR_DIMMED, !(humidityVal));
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX_3, ATTR_DIMMED, !(pressureVal));  //dlp

	SetPanelPos(resultPanel, 105, 305);  
	DisplayPanel(resultPanel);  

		
	SetPanelPos(hResultDispPanel, 105, 1600);
	DisplayPanel(hResultDispPanel);
		
	SetPanelPos(hEnvResultPanel, 556, 1600);
	DisplayPanel(hEnvResultPanel);
	
	DisplayImageFile (mainPanel, MAIN_PANEL_SELECT, "Resource\\Select.ico");
	DisplayImageFile (mainPanel, MAIN_PANEL_CONFIGURE, "Resource\\Configure.ico"); 
	DisplayImageFile (mainPanel, MAIN_PANEL_ANALYZE, "Resource\\Analyze_pressed.ico");
	DispResultTableGraph();  			//根据当前状态显示相应界面dlp
	
	SetCtrlAttribute(graphDispPanel, GRAPHDISP_GRAPH1, ATTR_ENABLE_ZOOM_AND_PAN, 1 );			//设置曲线图可以通过鼠标键盘放大与缩小
	SetCtrlAttribute(graphDispPanel, GRAPHDISP_GRAPH2, ATTR_ENABLE_ZOOM_AND_PAN, 1 );			//设置曲线图可以通过鼠标键盘放大与缩小
	
	DeleteGraphPlot (graphDispPanel, GRAPHDISP_GRAPH1,-1 , VAL_IMMEDIATE_DRAW); 				//清空图表1图上的所有曲线
	DeleteGraphPlot (graphDispPanel, GRAPHDISP_GRAPH2,-1 , VAL_IMMEDIATE_DRAW); 				//清空图表2图上的所有曲线 
	DeleteTableRows (tablePanel, TABLE_TABLE1, 1, -1);											//清除表格 
	DeleteTableColumns (tablePanel, TABLE_TABLE1, 1, -1);
	
	GetCtrlVal (hAdvanceSamplePanel, SAMPLE_ADV_CURRENTMODE, &logFlag);		//是否显示为log
	GetCurveAttr(&CurveAttr);		//获取曲线属性	  
}
void RunKeyAction_Again()																		//运行按钮按下后产生的一系列动作
{
	SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP, ATTR_DIMMED, 0);       						//恢复 停止按钮
	SetCtrlAttribute (mainPanel, MAIN_PANEL_RUN, ATTR_DIMMED,1);         						//禁用 开始按钮      
    SetCtrlAttribute (mainPanel, MAIN_PANEL_SAVE, ATTR_DIMMED,1);        						//禁用 保存按钮
	SetCtrlAttribute (mainPanel, MAIN_PANEL_SETTINGS, ATTR_DIMMED,1);    						//禁用 设置按钮 
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_MEASURE, ATTR_DIMMED, 1);		  				//禁用测量按钮 dlp
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		  				//dlp
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);		  				//dlp
	SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);		  				//dlp
	
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX, ATTR_DIMMED, !(tempVal));			//前面板是否勾选决定后面板是否禁用
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX_2, ATTR_DIMMED, !(humidityVal));
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX_3, ATTR_DIMMED, !(pressureVal));  	//dlp

	SetPanelPos(resultPanel, 105, 305);  
	DisplayPanel(resultPanel);  

		
	SetPanelPos(hResultDispPanel, 105, 1600);
	DisplayPanel(hResultDispPanel);
		
	SetPanelPos(hEnvResultPanel, 556, 1600);
	DisplayPanel(hEnvResultPanel);
	
	DisplayImageFile (mainPanel, MAIN_PANEL_SELECT, "Resource\\Select.ico");
	DisplayImageFile (mainPanel, MAIN_PANEL_CONFIGURE, "Resource\\Configure.ico"); 
	DisplayImageFile (mainPanel, MAIN_PANEL_ANALYZE, "Resource\\Analyze_pressed.ico");
	DispResultTableGraph();  																	//根据当前状态显示相应界面dlp
	
	//SetCtrlAttribute(graphDispPanel, GRAPHDISP_GRAPH1, ATTR_ENABLE_ZOOM_AND_PAN, 1 );			//设置曲线图可以通过鼠标键盘放大与缩小
	//SetCtrlAttribute(graphDispPanel, GRAPHDISP_GRAPH2, ATTR_ENABLE_ZOOM_AND_PAN, 1 );			//设置曲线图可以通过鼠标键盘放大与缩小
	
	//DeleteGraphPlot (graphDispPanel, GRAPHDISP_GRAPH1,-1 , VAL_IMMEDIATE_DRAW); 				//清空图表1图上的所有曲线
	//DeleteGraphPlot (graphDispPanel, GRAPHDISP_GRAPH2,-1 , VAL_IMMEDIATE_DRAW); 				//清空图表2图上的所有曲线 
	//DeleteTableRows (tablePanel, TABLE_TABLE1, 1, -1);											//清除表格 
	//DeleteTableColumns (tablePanel, TABLE_TABLE1, 1, -1);
	
	GetCtrlVal (hAdvanceSamplePanel, SAMPLE_ADV_CURRENTMODE, &logFlag);		//是否显示为log
	//GetCurveAttr(&CurveAttr);		//获取曲线属性
	
	CurveAttr.smu1lineColor = CurveAttr.smu1lineColor + 100000;
	CurveAttr.smu2lineColor = CurveAttr.smu2lineColor + 100000;
}
void GetCurveAttr(CurveAttrTypeDef* pCurveAttr)		//获取曲线属性	
{
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_PLOT_STYLE, &(pCurveAttr->plotStyle));
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_POINT_STYLE, &(pCurveAttr->pointStyle));
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_LINE_STYLE, &(pCurveAttr->lineStyle));

	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU1CLR, &(pCurveAttr->smu1lineColor));	//得到SMU1和SMU2的曲线颜色DLP
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU2CLR, &(pCurveAttr->smu2lineColor));
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR1, &(pCurveAttr->tempColor));		//得到温度湿度压力三条曲线的颜色 DLP
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR2, &(pCurveAttr->humColor));
	GetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR3, &(pCurveAttr->pressColor));
}
void StopKeyAction(unsigned char select_Addr1,unsigned char select_Addr2)						//停止按钮按下后产生的一系列动作
{
	DiscardAsyncTimer(TimerID);									 							 	//关闭异步定时器  停止曲线显示 
	Graph.pCurveArray->numOfPlotDots=0;
	SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP, ATTR_DIMMED,1);      							//禁用 停止按钮      
	SetCtrlAttribute (mainPanel, MAIN_PANEL_RUN, ATTR_DIMMED, 0);      							//恢复 开始按钮
	SetCtrlAttribute (mainPanel, MAIN_PANEL_SAVE, ATTR_DIMMED, 0);     							//恢复 保存按钮
	SetCtrlAttribute (mainPanel, MAIN_PANEL_SETTINGS, ATTR_DIMMED,0); 							//禁用 设置按钮 
	
	if(controlComPort==0)

	{
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_MEASURE, ATTR_DIMMED, 1);		  //恢复测量按钮 dlp 
	}
	else
	{
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_MEASURE, ATTR_DIMMED, 0);		  //恢复测量按钮 dlp  
	}
	
	GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_MEASURE, &measureval); 
	
	if(measureval==0)
	{
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		//禁用
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);
	}
	else
	{
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 0);		  //前面板dlp
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 0);		  //前面板dlp
		SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 0);		  //前面板dlp
	}

	
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX, ATTR_DIMMED, 1);		//禁用后面板可选状态
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX_2, ATTR_DIMMED, 1);		 //禁用后面板dlp
	SetCtrlAttribute (hEnvResultPanel,  ENVIRPANEL_CHECKBOX_3, ATTR_DIMMED, 1);		 //禁用后面板dlp
	
	if((select_Addr1 == 0X01) && (select_Addr2 == 0X02))
	{
		ProtocolStop(measureComPort, 0x01, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x01, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x01, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x01, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令 
		ProtocolStop(measureComPort, 0x01, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令 
		ProtocolStop(measureComPort, 0x01, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
	}
	else if(select_Addr1 == 0x01)
	{
		ProtocolStop(measureComPort, 0x01, 0x00, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x01, 0x00, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x01, 0x00, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令 
		ProtocolStop(measureComPort, 0x01, 0x00, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令 
		ProtocolStop(measureComPort, 0x01, 0x00, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令 
		ProtocolStop(measureComPort, 0x01, 0x00, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令 
		
	}
	else if(select_Addr2 == 0x02)
	{
		ProtocolStop(measureComPort, 0x00, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x00, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x00, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x00, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x00, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
		ProtocolStop(measureComPort, 0x00, 0x02, measUartTxBuf1, measUartTxBuf2);  				//发送停止指令
	}
	if(measureComPort > 0)
	{
		FlushInQ(measureComPort);	   																	//Clear input and output buffer
		FlushOutQ(measureComPort);
	}
	if(controlComPort > 0)
	{
		FlushInQ(controlComPort);	   																	//Clear input and output buffer
		FlushOutQ(controlComPort);
	}
	
	
	
}
void ProtocolCfg(unsigned char measureComPort, unsigned char devAddr1, unsigned char devAddr2,unsigned char expType, unsigned char* pmeasUartTxBuf1,unsigned char* pmeasUartTxBuf2)
{
	int graphIndex = 1;
	int numOfCurve = 2;
	int numOfDots  = 100;
	int temp       = 0;
	int numOfDots1,numOfDots2;
	Table_TypeDef Table_ATTR;
	switch((enum ExpType)expType)  																				//用户输入鼠标选择的模式标号
	{
		case NO_SWEEP_IV:
			GetTestPara(&I_V_Panel1, &TestPara1);																//得到源表 1 用户设置参数
			GetTestPara(&I_V_Panel2, &TestPara2);																//得到源表 2 用户设置参数
			numOfDots1 = abs(TestPara1.Current_Start - TestPara1.Current_Stop)/TestPara1.Current_Step +1;
			numOfDots2 = abs(TestPara2.Current_Start - TestPara2.Current_Stop)/TestPara2.Current_Step +1;
			
			numOfDots =100 + (numOfDots1 >= numOfDots2 ? numOfDots1:numOfDots2);
			
			graphInit(graphIndex, numOfCurve, numOfDots, &Graph);
			graphInit(graphIndex, 3, numOfDots + 5, &Graph_Temp); 
			Graph.pCurveArray->numOfTotalDots = numOfDots;														//理论计算总点数
			
			Table_ATTR.column = 7;   				//列数
			Table_ATTR.column_width = 150;  		//列宽
			Table_ATTR.row=numOfDots + 10;
			
			Graph.pGraphAttr->xAxisHead = 0;
			Graph.pGraphAttr->xAxisTail = 100;
			Graph.pGraphAttr->yAxisHead = 1.008e-13;
	   		Graph.pGraphAttr->yAxisTail = 1.0134e-13; 
			
			Graph_Temp.pGraphAttr->xAxisHead = 0;
			Graph_Temp.pGraphAttr->xAxisTail = 100;
			Graph_Temp.pGraphAttr->yAxisHead = 0;
	   		Graph_Temp.pGraphAttr->yAxisTail = 100;
			
		
			Table_init(Table_title_IV, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			Graph.pGraphAttr->xAxisHead = TestPara1.Current_Start <= TestPara2.Current_Start ? TestPara1.Current_Start:TestPara2.Current_Start;
			Graph.pGraphAttr->xAxisTail = TestPara1.Current_Stop >= TestPara2.Current_Stop ? TestPara1.Current_Stop:TestPara2.Current_Stop;
			Graph_Temp.X_Axis1 = Graph.pGraphAttr->xAxisHead; 
			
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH1,VAL_BOTTOM_XAXIS,VAL_MANUAL,Graph.pGraphAttr->xAxisHead,Graph.pGraphAttr->xAxisTail);
			//SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);					//设置电学Y轴的初始化坐标范围
		    
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph.pGraphAttr->xAxisHead, Graph.pGraphAttr->xAxisTail);				//设置环境X轴的初始化坐标范围
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH2,VAL_LEFT_YAXIS,VAL_MANUAL,Graph_Temp.pGraphAttr->yAxisHead ,Graph_Temp.pGraphAttr->yAxisTail);			//环境Y轴
			
			break;
		case NO_SWEEP_VI:
			int val;
			GetCtrlVal (VIPanel, PANEL_V_I_START1UNIT, &val); 
			GetTestPara(&V_I_Panel1, &TestPara1);  																//得到源表 1 用户设置参数
			GetTestPara(&V_I_Panel2, &TestPara2);  																//得到源表 2 用户设置参数
			numOfDots1 = abs(TestPara1.Voltage_Start - TestPara1.Voltage_Stop)/TestPara1.Voltage_Step + 1;
			numOfDots2 = abs(TestPara2.Voltage_Start - TestPara2.Voltage_Stop)/TestPara2.Voltage_Step + 1; 
			
			numOfDots =100 + (numOfDots1 >= numOfDots2 ? numOfDots1:numOfDots2);
			
			graphInit(graphIndex, numOfCurve, numOfDots, &Graph);
			graphInit(graphIndex, 3, numOfDots + 5, &Graph_Temp); 
			
			
			
			Table_ATTR.column = 7;   																			//列数
			Table_ATTR.column_width = 150;  																	//列宽
			Table_ATTR.row=numOfDots + 10; 
			if(val==0)
				Table_init(Table_title_VI_A, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			else if(val==1)
				Table_init(Table_title_VI_mA, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			else if(val==2)
				Table_init(Table_title_VI_uA, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			else if(val==3)
				Table_init(Table_title_VI_nA, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			else if(val==4)
				Table_init(Table_title_VI_pA, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			
			Graph.pGraphAttr->xAxisHead = 0;
			Graph.pGraphAttr->xAxisTail = 100;
			Graph.pGraphAttr->yAxisHead = 1.008e-13;
	   		Graph.pGraphAttr->yAxisTail = 1.0134e-13; 
			
			
			Graph_Temp.pGraphAttr->xAxisHead = 0;
			Graph_Temp.pGraphAttr->xAxisTail = 100;
			Graph_Temp.pGraphAttr->yAxisHead = 0;
	   		Graph_Temp.pGraphAttr->yAxisTail = 100;
			
			
			Graph.pGraphAttr->xAxisHead = TestPara1.Voltage_Start;   
			Graph.pGraphAttr->xAxisTail = TestPara1.Voltage_Stop;
			
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH1,VAL_BOTTOM_XAXIS,VAL_MANUAL,Graph.pGraphAttr->xAxisHead,Graph.pGraphAttr->xAxisTail);
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph.pGraphAttr->xAxisHead, Graph.pGraphAttr->xAxisTail);				//设置环境X轴的初始化坐标范围  
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH2,VAL_LEFT_YAXIS,VAL_MANUAL,Graph_Temp.pGraphAttr->yAxisHead ,Graph_Temp.pGraphAttr->yAxisTail);   //环境Y轴        
			//*********************************************************得到用户选择的电流单位,只有在设置电流时使用 V-I V-T**************************************************************************//
			GetCtrlVal(VIPanel,PANEL_V_I_START1UNIT,&(temp));									//得到电路板1用户选择的电流单位
			TestPara1.rangeMode = (temp<<4)|TestPara1.rangeMode; 
			
			GetCtrlVal(VIPanel,PANEL_V_I_START2UNIT,&(temp));										//得到电路板2用户选择的电流单位
			TestPara2.rangeMode = (temp<<4)|TestPara2.rangeMode; 
			//*********************************************************得到用户选择的电流单位**************************************************************************//
			
			
			break;
		case NO_SWEEP_IT:
			GetTestPara(&I_T_Panel1, &TestPara1);														//得到源表 1 用户设置参数
			GetTestPara(&I_T_Panel2, &TestPara2);														//得到源表 2 用户设置参数
			
			//numOfDots1 =(TestPara1.runTime*1000)/TestPara1.timeStep + 1;
			//numOfDots2 =(TestPara2.runTime*1000)/TestPara2.timeStep + 1;
			TestPara1.TotalDelay = TestPara1.SampleDelay*0.001+(TestPara1.sampleNumber/(TestPara1.sampleRate*1.0));
			TestPara2.TotalDelay = TestPara2.SampleDelay*0.001+(TestPara2.sampleNumber/(TestPara2.sampleRate*1.0));
			numOfDots1 =TestPara1.runTime/TestPara1.TotalDelay;
			numOfDots2 =TestPara2.runTime/TestPara2.TotalDelay;
			
			numOfDots =200 + (numOfDots1 >= numOfDots2 ? numOfDots1:numOfDots2);
			
			graphInit(graphIndex, numOfCurve, numOfDots, &Graph);
			graphInit(graphIndex, 3, numOfDots + 5, &Graph_Temp);
			Graph.pCurveArray->numOfTotalDots = numOfDots;												//理论计算总点数
			
			Table_ATTR.column = 7;   				//列数
			Table_ATTR.column_width = 150;  		//列宽
			Table_ATTR.row=numOfDots + 10;
			
			Graph.pGraphAttr->xAxisHead = 0;
			Graph.pGraphAttr->xAxisTail = 100;
			Graph.pGraphAttr->yAxisHead = 1.008e-13;
	   		Graph.pGraphAttr->yAxisTail = 1.0134e-13; 
			
			Graph_Temp.pGraphAttr->xAxisHead = 0;
			Graph_Temp.pGraphAttr->xAxisTail = 100;
			Graph_Temp.pGraphAttr->yAxisHead = 0;
	   		Graph_Temp.pGraphAttr->yAxisTail = 100;

			Table_init(Table_title_IT, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);																				//初始化表格
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph.pGraphAttr->xAxisHead, Graph.pGraphAttr->xAxisTail); 						//设置电学X轴的初始化坐标范围
			//SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);						//设置电学Y轴的初始化坐标范围
			
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph_Temp.pGraphAttr->xAxisHead, Graph_Temp.pGraphAttr->xAxisTail);				//设置环境X轴的初始化坐标范围
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH2,VAL_LEFT_YAXIS,VAL_MANUAL,Graph_Temp.pGraphAttr->yAxisHead ,Graph_Temp.pGraphAttr->yAxisTail);   //环境Y轴  
			break;
		case NO_SWEEP_RT:
			expType = 3;																				//RT与IT模式相同
			GetTestPara(&R_T_Panel1, &TestPara1);   													//得到源表 1 用户设置参数
			GetTestPara(&R_T_Panel2, &TestPara2);   													//得到源表 2 用户设置参数
			//TestPara1.Current_Step = 1;
			//numOfDots1 =(TestPara1.runTime*1000)/TestPara1.timeStep + 1;
			//numOfDots2 =(TestPara2.runTime*1000)/TestPara2.timeStep + 1;
			TestPara1.TotalDelay = TestPara1.SampleDelay*0.001+(TestPara1.sampleNumber/(TestPara1.sampleRate*1.0));
			TestPara2.TotalDelay = TestPara2.SampleDelay*0.001+(TestPara2.sampleNumber/(TestPara2.sampleRate*1.0));
			numOfDots1 =TestPara1.runTime/TestPara1.TotalDelay;
			numOfDots2 =TestPara2.runTime/TestPara2.TotalDelay;
			
			
			numOfDots =200 + (numOfDots1 >= numOfDots2 ? numOfDots1:numOfDots2);
			graphInit(graphIndex, numOfCurve, numOfDots, &Graph);
			graphInit(graphIndex, 3, numOfDots + 5, &Graph_Temp);
			Graph.pCurveArray->numOfTotalDots = numOfDots;												//理论计算总点数
			
			Table_ATTR.column = 7;   				//列数
			Table_ATTR.column_width = 150;  		//列宽
			Table_ATTR.row=numOfDots + 10;
			
			Graph.pGraphAttr->xAxisHead = 0;
			Graph.pGraphAttr->xAxisTail = 100;
			Graph.pGraphAttr->yAxisHead = 1.008e-13;
	   		Graph.pGraphAttr->yAxisTail = 1.0134e-13; 
			
			Graph_Temp.pGraphAttr->xAxisHead = 0;
			Graph_Temp.pGraphAttr->xAxisTail = 100;
			Graph_Temp.pGraphAttr->yAxisHead = 0;
	   		Graph_Temp.pGraphAttr->yAxisTail = 100;
			
			Table_init(Table_title_RT, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph.pGraphAttr->xAxisHead, Graph.pGraphAttr->xAxisTail); 						//设置电学X轴的初始化坐标范围
			//SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);						//设置电学Y轴的初始化坐标范围
			
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph_Temp.pGraphAttr->xAxisHead, Graph_Temp.pGraphAttr->xAxisTail);				//设置环境X轴的初始化坐标范围
			//SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_LEFT_YAXIS  , VAL_MANUAL, Graph_Temp.pGraphAttr->yAxisHead, Graph_Temp.pGraphAttr->yAxisTail);			//设置环境Y轴的初始化坐标范围  建议使用控件自动调整Y轴坐标的位置
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH2,VAL_LEFT_YAXIS,VAL_MANUAL,Graph_Temp.pGraphAttr->yAxisHead ,Graph_Temp.pGraphAttr->yAxisTail);   //环境Y轴  
			break;
		case NO_SWEEP_VT:
			
			expType = 4;
			GetTestPara(&V_T_Panel1, &TestPara1);  //得到源表 1 用户设置参数
			GetTestPara(&V_T_Panel2, &TestPara2);  //得到源表 2 用户设置参数
			
			//numOfDots1 =(TestPara1.runTime*1000)/TestPara1.timeStep + 1;
			//numOfDots2 =(TestPara2.runTime*1000)/TestPara2.timeStep + 1;
			TestPara1.TotalDelay = TestPara1.SampleDelay*0.001+(TestPara1.sampleNumber/(TestPara1.sampleRate*1.0));
			TestPara2.TotalDelay = TestPara2.SampleDelay*0.001+(TestPara1.sampleNumber/(TestPara1.sampleRate*1.0));
			numOfDots1 =TestPara1.runTime/TestPara1.TotalDelay;
			numOfDots2 =TestPara2.runTime/TestPara2.TotalDelay;
			
			
			numOfDots =200 + (numOfDots1 >= numOfDots2 ? numOfDots1:numOfDots2);
			
			
			graphInit(graphIndex, numOfCurve, numOfDots, &Graph);
			graphInit(graphIndex, 3, numOfDots + 5, &Graph_Temp); 
			Graph.pCurveArray->numOfTotalDots = numOfDots;												//理论计算总点数
			
			
			Table_ATTR.column = 7;   				//列数
			Table_ATTR.column_width = 150;  		//列宽
			Table_ATTR.row=numOfDots + 10;
			
			Graph.pGraphAttr->xAxisHead = 0;
			Graph.pGraphAttr->xAxisTail = 100;
			Graph.pGraphAttr->yAxisHead = 1.008e-13;
	   		Graph.pGraphAttr->yAxisTail = 1.0134e-13; 
			
			Graph_Temp.pGraphAttr->xAxisHead = 0;
			Graph_Temp.pGraphAttr->xAxisTail = 100;
			Graph_Temp.pGraphAttr->yAxisHead = 0;
	   		Graph_Temp.pGraphAttr->yAxisTail = 100;
			
			
			Table_init(Table_title_VT, Table_ATTR.column, Table_ATTR.column_width,Table_ATTR.row);																				//初始化表格
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph.pGraphAttr->xAxisHead, Graph.pGraphAttr->xAxisTail); 						//设置电学X轴的初始化坐标范围
			//SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);						//设置电学Y轴的初始化坐标范围
			
			SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_BOTTOM_XAXIS, VAL_MANUAL, Graph_Temp.pGraphAttr->xAxisHead, Graph_Temp.pGraphAttr->xAxisTail);				//设置环境X轴的初始化坐标范围
			//SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH2, VAL_LEFT_YAXIS  , VAL_MANUAL, Graph_Temp.pGraphAttr->yAxisHead, Graph_Temp.pGraphAttr->yAxisTail);			//设置环境Y轴的初始化坐标范围  建议使用控件自动调整Y轴坐标的位置
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH2,VAL_LEFT_YAXIS,VAL_MANUAL,Graph_Temp.pGraphAttr->yAxisHead ,Graph_Temp.pGraphAttr->yAxisTail);   //环境Y轴  
			//*********************************************************得到用户选择的电流单位,只有在设置电流时使用 V-I V-T**************************************************************************//
			GetCtrlVal(VTPanel,PANEL_V_T_VTSTART1UNIT,&(temp));									//得到电路板1用户选择的电流单位
			TestPara1.rangeMode = (temp<<4)|TestPara1.rangeMode; 
			
			GetCtrlVal(VTPanel,PANEL_V_T_VTSTART2UNIT,&(temp));								//得到电路板2用户选择的电流单位
			TestPara2.rangeMode = (temp<<4)|TestPara2.rangeMode; 
			//*********************************************************得到用户选择的电流单位**************************************************************************//
			
			
			break;
	}
	Graph.pCurveArray->numofSmu1RxDots = 0;
	(Graph.pCurveArray+1)->numofSmu2RxDots = 0;
	
	PrepareCfgTxData(&TestPara1, &TestPara2, devAddr1, devAddr2, expType, pmeasUartTxBuf1,pmeasUartTxBuf2); 			//分别向  源表1  源表2 变量中 放入用户输入的 设置命令 
	
	if(devAddr1 == 0x01)																								//判断是否为源表 1 地址，为真则发送 源表 1 设置命令
	ComWrt(measureComPort, (const char*)pmeasUartTxBuf1, SA31_UART_TX_LEN);
	Delay(0.05);
	if(devAddr2 == 0x02)																								//判断是否为源表 2 地址，为真则发送 源表 2 设置命令  
	ComWrt(measureComPort, (const char*)pmeasUartTxBuf2, SA31_UART_TX_LEN);
	Delay(0.05);
}

void SelectSMU(unsigned char* select_Addr1,unsigned char* select_Addr2)
{
	int status; 
	GetCtrlVal(mainPanel, MAIN_PANEL_SMU1, &status);																	//判断是否选中SMU1板子测试
	if(status>0)
		*select_Addr1=0x01;
	else
		*select_Addr1=0x00;
	GetCtrlVal(mainPanel, MAIN_PANEL_SMU2, &status);																	//判断是否选中SMU2板子测试
	if(status>0)
		*select_Addr2=0x02;
	else
		*select_Addr2=0x00;
}
//===========================================小马哥写=================================================================================  
int CVICALLBACK AbnmDCThreadFunction (void *functionData)
{ 
    int n = 1;   	 //每十分之n更新一次缓存去的数据   
	while(threadFlag == 1)
	{
		if(Graph.pCurveArray->numOfPlotDots > 0)  //每十分之一的总点数时更新一次缓存区
		{
		    WriteAndSaveExcel(tablePanel, TABLE_TABLE1);
			n +=1;
			if(n > 10)
			{
				n = 1;
			}
		}
	}
	return 0;
}
//============================================================================================================================  
//   RunCallback
int CVICALLBACK RunCallback (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int expType;
	int index;
	
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
				GetCtrlIndex(expListPanel, EXP_LIST_EXPLIST, &index);									//得到用户选择的测试模式，目前板子1和板子2是使用的同一种模式。后期可以分开设置
				if(index==TWO_TERMINAL||index==FOUR_TERMINAL||index==THREE_TERMINAL || index>=6)		//只识别有效的测试类型，目前只支持5种测试类型
				{
					MessagePopup ("", "Please select a experiment!");
				}
				else
				{  
					/********************************************以下后期需做修改**************************************************************/
					rowIndex  = 2;																		//源表1表格行
					rowIndex2 = 2;																		//源表2表格行
					rowIndex3 = 2;																		//温湿度数据在表格中的行
					Graph.X_Axis1 = 0;																	//源表1 时间轴 X数据
					Graph.X_Axis2 = 0;																	//源表2 时间轴 X数据
					Graph_Temp.X_Axis1 = 0;																//环境参数 时间轴 X数据
					/**************************************************************************************************************************/
					GraphDeinit(&Graph);																//内存释放在画图之后
					GraphDeinit(&Graph_Temp);
					//得到温湿度相关显示标志位的初始化值。
					GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX,&temp_flag);							//然后读取用户要选中的曲线
					GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_2,&humidity_flag); 
					GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_3,&pressure_flag);
					GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_TEMPTER, &tempVal);         					//读取前面板状态dlp
					GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_HUMIDITY, &humidityVal);	  					//dlp
					GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_PRESSURE, &pressureVal);	  					//dlp
//========================================小马哥写==============================================================================================
					//threadFlag = 1;
					//CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, AbnmDCThreadFunction, NULL, &abnmDCThreadId);//开辟新的线程缓存实验数据
//======================================================================================================================================
					
					int temp1;
					GetCtrlVal (hEnvCfgPanel,  ENVIRONMEN_MEASURE,&temp1);															//判断环境选择按按钮是否按下。
					if(temp1 == 1)
					{
					  control_Flag = 1;
					}else
					{
					  control_Flag = 0;
					}
					
					
					
					SelectSMU(&select_Addr1,&select_Addr2);																			//判断用户选择了那个板卡测量 
					if(GetCtrlVal(expListPanel, EXP_LIST_EXPLIST, &expType)<0)  													//每次开始之前判断一下用户选择的 测试模式 
						return -1;
					TestPara1.testMode = expType; 																					//源表 1 测试类型
					TestPara2.testMode = expType; 																					//源表 2 测试类型
					RunKeyAction();  			  																					//运行按钮按下后产生的一系列动作 
					ProtocolCfg(measureComPort, select_Addr1, select_Addr2,(unsigned char)expType, measUartTxBuf1,measUartTxBuf2);	//得到用户的设置参数  并发送
					Delay(1);																								
					ProtocolRun(measureComPort, select_Addr1, select_Addr2, measUartTxBuf1, measUartTxBuf2);						//send RUN command to instrument via UART 
					Delay(0.01);
					
					double temp=(double)TestPara1.timeStep * 0.001;
					temp = 0.01;																									//如果查询时间过快，会造成数据混乱，下位机响应中断过多
					TimerID = NewAsyncTimer(temp,-1, 1, TimerCallback, 0);
				}
			break;
	}
	return 0;
}

int CVICALLBACK RunAgainCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int expType;
	int index;
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
				GetCtrlIndex(expListPanel, EXP_LIST_EXPLIST, &index);								//得到用户选择的测试模式，目前板子1和板子2是使用的同一种模式。后期可以分开设置
				if(index==TWO_TERMINAL||index==FOUR_TERMINAL||index==THREE_TERMINAL || index>=6)	//只识别有效的测试类型，目前只支持5种测试类型
				{
					MessagePopup ("", "Please select a experiment!");
				}
				else
				{  
					/********************************************以下后期需做修改**************************************************************/
					rowIndex  = 2;																		//源表1表格行
					rowIndex2 = 2;																		//源表2表格行
					rowIndex3 = 2;																		//温湿度数据在表格中的行
					Graph.X_Axis1 = 0;																	//源表1 时间轴 X数据
					Graph.X_Axis2 = 0;																	//源表2 时间轴 X数据
					Graph_Temp.X_Axis1 = 0;																//环境参数 时间轴 X数据
					/**************************************************************************************************************************/
					//FlushInQ(measureComPort);	   														//Clear input and output buffer
					//FlushOutQ(measureComPort);
				/*	GraphDeinit(&Graph);																//内存释放在画图之后
					GraphDeinit(&Graph_Temp);*/
					//得到温湿度相关显示标志位的初始化值。
					GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX,&temp_flag);							//然后读取用户要选中的曲线
					GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_2,&humidity_flag); 
					GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_3,&pressure_flag);
					GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_TEMPTER, &tempVal);         					//读取前面板状态dlp
					GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_HUMIDITY, &humidityVal);	  					//dlp
					GetCtrlVal (hEnvCfgPanel, ENVIRONMEN_PRESSURE, &pressureVal);	  					//dlp

//========================================小马哥写==============================================================================================
					//threadFlag = 1;
					//CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, AbnmDCThreadFunction, NULL, &abnmDCThreadId);//开辟新的线程缓存实验数据
//======================================================================================================================================
					
					SelectSMU(&select_Addr1,&select_Addr2);																			//判断用户选择了那个板卡测量 
					if(GetCtrlVal(expListPanel, EXP_LIST_EXPLIST, &expType)<0)  													//每次开始之前判断一下用户选择的 测试模式 
						return -1;
					TestPara1.testMode = expType; 																					//源表 1 测试类型
					TestPara2.testMode = expType; 																					//源表 2 测试类型
					RunKeyAction_Again();  			  																					//运行按钮按下后产生的一系列动作 
					
					ProtocolCfg(measureComPort, select_Addr1, select_Addr2,(unsigned char)expType, measUartTxBuf1,measUartTxBuf2);	//得到用户的设置参数  并发送
					Delay(1);																								
					ProtocolRun(measureComPort, select_Addr1, select_Addr2, measUartTxBuf1, measUartTxBuf2);						//send RUN command to instrument via UART 
					Delay(0.05);
					
					double temp=(double)TestPara1.timeStep * 0.001;
					temp = 0.1;																									//如果查询时间过快，会造成数据混乱，下位机响应中断过多
					TimerID = NewAsyncTimer(temp,-1, 1, TimerCallback, 0); 
				}
			break;
	}
	return 0;
}

//===================================================
//   StopCallback

int CVICALLBACK StopCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
//==============================================小马哥写==============================================================================     			
			threadFlag = 0;
//==============================================小马哥写==============================================================================     			
			StopKeyAction(select_Addr1,select_Addr2);							//停止按钮按下后产生的一系列动作

			break;
	}
	return 0;
}

//===================================================
//   SaveCallback

int CVICALLBACK SaveCallback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK:			    //当Save被鼠标左键点击时 
			DisplayImageFile (mainPanel, MAIN_PANEL_SAVE, "Resource\\Save_pressed.ico");
			break;
		case EVENT_LEFT_CLICK_UP:		    //当鼠标释放时  
			DisplayImageFile (mainPanel, MAIN_PANEL_SAVE, "Resource\\Save.ico");
			if(FileSelectPopupEx("C:\\SINOAGG\\SA3102\\", ".sac", "*.sac", "Select Path", VAL_OK_BUTTON, 0, 1,  configSavePath)>0)
			SaveConfigToFile(configSavePath);
			break;
	}
	return 0;
}
static int SaveAllPanelState(char* pConfigSavePath)
{
	SavePanelState(IVPanel, pConfigSavePath, 1);
	SavePanelState(VIPanel, pConfigSavePath, 2);
	SavePanelState(I_T_Panel1.panelHandle, pConfigSavePath, 3); //IT面板的值  
	SavePanelState(VTPanel, pConfigSavePath, 4);
	SavePanelState(RTPanel, pConfigSavePath, 5);
	SavePanelState(hBasicSamplePanel, pConfigSavePath, 10);	   //用户设置 配置值
	SavePanelState(hAdvanceSamplePanel, pConfigSavePath, 11);  //高级设置面板值
	SavePanelState(hEnvCfgPanel, pConfigSavePath, 14);
	SavePanelState(ENVTPanel, pConfigSavePath, 15);
	SavePanelState(hSettingsGraphPanel, pConfigSavePath, 16);
	return 0;
}
static int SaveConfigToFile(char* pConfigSavePath)
{
	FILE * fp = NULL;							//表示打开的文件
	fp = fopen(pConfigSavePath, "w");
	if(fp == NULL)
	{
		MessagePopup ("Error", "Invalid Path, please select path to save configurations.");
		if(FileSelectPopup ("C:\\SINOAGG\\SA3102\\", ".sac", "*.sac", "Select Path", VAL_OK_BUTTON, 0, 1, 1, 1, pConfigSavePath)<0)
			return -1;
	}
	else
	{
		int maxCommentSize=255;
		char description[maxCommentSize];
		PromptPopup("Message", "Please enter description for this configuration:", description, maxCommentSize-1);
		SaveAllPanelState(pConfigSavePath);		//保存所有面板的数据
		fp = fopen(pConfigSavePath, "a+");
		fprintf(fp, "\r\n%-61s\r\n", description);		//左对齐打满64字符
		fclose(fp);//关闭文件
	}
	return 0;
}
//===================================================
//   SelectCallback

int CVICALLBACK SelectCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:			    //当Select被鼠标左键点击时,Select图标改变，其它两个正常状态 
			DisplayImageFile (mainPanel, MAIN_PANEL_SELECT, "Resource\\Select_pressed.ico");
			DisplayImageFile (mainPanel, MAIN_PANEL_CONFIGURE, "Resource\\Configure.ico"); 
			DisplayImageFile (mainPanel, MAIN_PANEL_ANALYZE, "Resource\\Analyze.ico");
			HidePanel(hBasicSamplePanel);		
			HidePanel(hResultDispPanel);
			HidePanel(hEnvCfgPanel);
			HidePanel(hEnvResultPanel);
			HidePanel(hAdvanceSamplePanel);
			break;
	}
	return 0;
}


//===================================================
//   Analyze_Callback

int CVICALLBACK AnalyzeCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
		
			DispResultTableGraph();
			
			SetPanelPos(resultPanel, 105, 305);       
	 		DisplayPanel(resultPanel);  
			
			SetPanelPos(hResultDispPanel, 105, 1600);
			DisplayPanel(hResultDispPanel);
				
			SetPanelPos(hEnvResultPanel, 556, 1600);
			DisplayPanel(hEnvResultPanel);

			DisplayImageFile (mainPanel, MAIN_PANEL_SELECT, "Resource\\Select.ico");
			DisplayImageFile (mainPanel, MAIN_PANEL_CONFIGURE, "Resource\\Configure.ico"); 
			DisplayImageFile (mainPanel, MAIN_PANEL_ANALYZE, "Resource\\Analyze_pressed.ico");
			
		break;
	}
	return 0;
}

int CVICALLBACK SettingsCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			InstallPopup (setPanel);  //弹出settingsPanel
			HidePanel (hSettingsGraphPanel);
			HidePanel (aboutPanel);
			SetPanelPos(ENVTPanel, 5, 160);
			DisplayPanel(ENVTPanel);
			
			SetCtrlAttribute (setPanel, SETTINGS_PRJBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG_PRESSED);   //project背景色
	     	SetCtrlAttribute (setPanel, SETTINGS_PRJBTN, ATTR_TEXT_COLOR, VAL_WHITE);              //project文本颜色
			
			SetCtrlAttribute (setPanel, SETTINGS_GRAPHBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);         //graph背景色
	     	SetCtrlAttribute (setPanel, SETTINGS_GRAPHBTN, ATTR_TEXT_COLOR, VAL_BLACK);            //graph文本颜色
			
			SetCtrlAttribute (setPanel, SETTINGS_ABOUTBTN, ATTR_TEXT_BGCOLOR, VAL_TEXTBG);         //about背景色
	     	SetCtrlAttribute (setPanel, SETTINGS_ABOUTBTN, ATTR_TEXT_COLOR, VAL_BLACK);            //about文本颜色
			
			GetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU1CLR, &smu1ClrOld);
			GetCtrlVal (hSettingsGraphPanel, SETGRAPH_SMU2CLR, &smu2ClrOld);
			GetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR1, &graph2Color1Old); 
			GetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR2, &graph2Color2Old); 
			GetCtrlVal (hSettingsGraphPanel, SETGRAPH_GRAPH2CLR3, &graph2Color3Old);
			
		break;
	}
	return 0;
}
static int LoadAndDispPrj(FileLableTypeDef *pFileLable, char index)						//index为prj所在位置排序
{
	int hSinglePrjPanel;
	if ((hSinglePrjPanel = LoadPanel (projectPanel, "ProjectPanel.uir", DEFPANEL)) < 0)		//load projects panel
		return -1;
	SetCtrlVal(hSinglePrjPanel, DEFPANEL_NAME, pFileLable->FileName);
	SetCtrlVal(hSinglePrjPanel, DEFPANEL_DATE, pFileLable->FileDate);
	SetCtrlVal(hSinglePrjPanel, DEFPANEL_TIME, pFileLable->FileTime);
	SetCtrlVal(hSinglePrjPanel, DEFPANEL_DESC, pFileLable->FileDesc);
	

	
	SetPanelPos(hSinglePrjPanel, index*117, -10);
	DisplayPanel(hSinglePrjPanel);
	return hSinglePrjPanel;
}

static int LoadAllProject(char* pProjectSavePath)
{
	char tempFileName[512];
	char tempFilePath[512];
	char index=0;
	char tempPathToSearch[512];
	InitSingleProject(SingleProject);
	sprintf(tempPathToSearch, "%s%s", pProjectSavePath, "\\*.sac");
	if(0==GetFirstFile(tempPathToSearch, 1, 1, 1, 1, 1, 0, tempFileName))		//如果第一个文件获取成功
	{
		sprintf(tempFilePath, "%s%s%s", pProjectSavePath, "\\", tempFileName);
		pFileLable[index] = (FileLableTypeDef *)malloc(sizeof(FileLableTypeDef));
		InitFileLable(pFileLable[index], tempFilePath); 				//读文件时间和文件名称及description，并展示
		SingleProject[index].hSinglePrjPanel = LoadAndDispPrj(pFileLable[index], index);
		SingleProject[index].index=index;
		index++;
		while(GetNextFile(tempFileName)==0)								//如果读取正确，持续读取
		{
			sprintf(tempFilePath, "%s%s%s", pProjectSavePath, "\\", tempFileName);
			pFileLable[index] = (FileLableTypeDef *)malloc(sizeof(FileLableTypeDef));
			InitFileLable(pFileLable[index], tempFilePath); //读文件时间和文件名称及description
			SingleProject[index].hSinglePrjPanel = LoadAndDispPrj(pFileLable[index], index);
			SingleProject[index].index=index; 
			index++;
		}
	}
	return 0;
}

int CVICALLBACK ProjectCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch(event)
	{
		case EVENT_LEFT_CLICK_UP:
			InstallPopup (proPanel);
			SetPanelPos(projectPanel, 85, 0);
			SetPanelSize(projectPanel, 380, 1250);
			DisplayPanel(projectPanel);
			LoadAllProject(ProjectSavePath);
			SetCtrlAttribute (proPanel,PROPANEL_PIC_OPENPRJ , ATTR_DIMMED, 1);	  //禁用openproject
			SetCtrlAttribute (proPanel,PROPANEL_TXT_OPENPRJ , ATTR_DIMMED, 1); 
		break;
	}	 
	return 0;
}

int CVICALLBACK ToolsCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			InstallPopup (hToolsPanel); 
			break;
	}
	return 0;
}
//////////////////////////////////校准//////////////////////////////////
int CVICALLBACK OutputVoltageCaliCallback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)	//电压校准
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_OUTVOLCALI, ATTR_DIMMED, 1);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_OUTVOLCALI, ATTR_TEXT_BGCOLOR, VAL_BG_DISABLE);
			
			
			measUartTxBuf1[0] = select_Addr1;
			measUartTxBuf1[1] = 0x15;
			measUartTxBuf1[2] = 0x00;
			measUartTxBuf1[31] = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			
			

			break;

	}
	return 0;
}
								  
//零点校准
int CVICALLBACK ZeroCurrentCaliCallback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int expType;
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_ZEROCURCALI, ATTR_DIMMED, 1);   //禁用
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_ZEROCURCALI, ATTR_TEXT_BGCOLOR, VAL_BG_DISABLE);
			
			//measUartTxBuf1[0] = select_Addr1;
			//measUartTxBuf1[1] = 0x15;
			//measUartTxBuf1[2] = 0x01;
			//measUartTxBuf1[29] = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			//ComWrt(measureComPort, (const char*)measUartTxBuf1, 30);
			
			GetCtrlVal(mainPanel, MAIN_PANEL_SMU1, &expType);							//判断是否选中SMU1板子测试
			if(expType>0)
				select_Addr1=0x01;
			else
				select_Addr1=0x00;
			GetCtrlVal(mainPanel, MAIN_PANEL_SMU2, &expType);							//判断是否选中SMU2板子测试
			if(expType>0)
				select_Addr2=0x02;
			else
				select_Addr2=0x00;
			
			if(select_Addr1 == 0x01)
			{
				measUartTxBuf1[0] = select_Addr1;
				measUartTxBuf1[1] = 0x15;
				measUartTxBuf1[2] = 0x01;
				measUartTxBuf1[31] = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
				ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			}
			if(select_Addr2 == 0x02)
			{
				measUartTxBuf2[0] = select_Addr2;
				measUartTxBuf2[1] = 0x15;
				measUartTxBuf2[2] = 0x01;
				measUartTxBuf2[31] = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
				ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
			}
			break;
	}
	return 0;
}
//电流校准 
int CVICALLBACK CurrentCaliCallback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{ 	
	
	int temp;
	int expType;
	unsigned char Select_Cali_Addr1,Select_Cali_Addr2; 
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_CURRENTCALI, ATTR_DIMMED, 1);   						//禁用
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_CURRENTCALI, ATTR_TEXT_BGCOLOR, VAL_BG_DISABLE);
			GetCtrlVal (hCalibrationPanel,CALIPANEL_RANGESELECT,&temp);											//得到校准档位  
			//measUartTxBuf1[0] = select_Addr1;
			//measUartTxBuf1[1] = 0x15;
			//measUartTxBuf1[2] = 0x02;
			//measUartTxBuf1[3] = (unsigned char)temp;
			//measUartTxBuf1[29] = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
			//ComWrt(measureComPort, (const char*)measUartTxBuf1, 30);
			
			GetCtrlVal(mainPanel, MAIN_PANEL_SMU1, &expType);													//判断是否选中SMU1板子测试
			if(expType>0)
				Select_Cali_Addr1=0x01;
			else
				Select_Cali_Addr1=0x00;
			GetCtrlVal(mainPanel, MAIN_PANEL_SMU2, &expType);													//判断是否选中SMU2板子测试
			if(expType>0)
				Select_Cali_Addr2=0x02;
			else
				Select_Cali_Addr2=0x00;
			
			if(Select_Cali_Addr1 == 0x01)
			{
				measUartTxBuf1[0] = Select_Cali_Addr1;
				measUartTxBuf1[1] = 0x15;
				measUartTxBuf1[2] = 0x02;
				measUartTxBuf1[3] = (unsigned char)temp; 
				measUartTxBuf1[31] = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
				ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			}
			if(Select_Cali_Addr2 == 0x02)
			{
				measUartTxBuf2[0] = Select_Cali_Addr2;
				measUartTxBuf2[1] = 0x15;
				measUartTxBuf2[2] = 0x02;
				measUartTxBuf2[3] = (unsigned char)temp; 
				measUartTxBuf2[31] = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
				ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
			}
			break;
	}
	return 0;
}
//保存校准数据
int CVICALLBACK SaveCaliCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)	
{
	int expType;
	unsigned char Select_Cali_Addr1,Select_Cali_Addr2;
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			GetCtrlVal(mainPanel, MAIN_PANEL_SMU1, &expType);							//判断是否选中SMU1板子测试
			if(expType>0)
				Select_Cali_Addr1=0x01;
			else
				Select_Cali_Addr1=0x00;
			GetCtrlVal(mainPanel, MAIN_PANEL_SMU2, &expType);							//判断是否选中SMU2板子测试
			if(expType>0)
				Select_Cali_Addr2=0x02;
			else
				Select_Cali_Addr2=0x00;
			
			if(Select_Cali_Addr1 == 0x01)
			{
				measUartTxBuf1[0] = Select_Cali_Addr1;
				measUartTxBuf1[1] = 0x15;
				measUartTxBuf1[2] = 0xff;
				measUartTxBuf1[31] = GetXorCheckVal(measUartTxBuf1, SA31_UART_TX_LEN-1);
				ComWrt(measureComPort, (const char*)measUartTxBuf1, SA31_UART_TX_LEN);
			}
			if(Select_Cali_Addr2 == 0x02)
			{
				measUartTxBuf2[0] = Select_Cali_Addr2;
				measUartTxBuf2[1] = 0x15;
				measUartTxBuf2[2] = 0xff;
				measUartTxBuf2[31] = GetXorCheckVal(measUartTxBuf2, SA31_UART_TX_LEN-1);
				ComWrt(measureComPort, (const char*)measUartTxBuf2, SA31_UART_TX_LEN);
			}
			HidePanel(hCalibrationPanel);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_ZEROCURCALI, ATTR_DIMMED, 0);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_ZEROCURCALI, ATTR_TEXT_BGCOLOR, VAL_BG_ENABLE);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_OUTVOLCALI, ATTR_DIMMED, 0);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_OUTVOLCALI, ATTR_TEXT_BGCOLOR, VAL_BG_ENABLE);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_CURRENTCALI, ATTR_DIMMED, 0);
			SetCtrlAttribute (hCalibrationPanel, CALIPANEL_CURRENTCALI, ATTR_TEXT_BGCOLOR, VAL_BG_ENABLE);
			break;
	}
	return 0;
}
//==============================================小马哥写============================================================================== 
static void ChangeStatus(int display)
{
	SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, display); 
	SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_DIMMED, display);	
}
//==============================================小马哥写==============================================================================
int	CheckUpdateSoftware()   //检查公司内测软件是否为最新  
{
	int i, len;
	len = strlen(serversUpdateVersion);
	SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 1);
	for(i=0;i<=len;i++)
	{
	 if(serversUpdateVersion[i] != currentUpdateVersion[i])
	 {
		int temp = ConfirmPopup ("Version update", "Whether to update to the latest Software version");  //更新软件版本提醒
		if(temp  == 1)
		{
			return 1;
		}else
		{
			MessagePopup ("Warning", "Please update");
			SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
			return 0;
		}
	 }
	if(i == len)
	 {
		MessagePopup ("Warning", "It is the latest Software version");
		SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);	
	 }
	}
	return 0;
}
//==============================================小马哥写==============================================================================
int	CheckSoftware()   //检查软件是否为最新  
{
	int i, len;
	len = strlen(serversSoftVersion);
	SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 1);
	for(i=0;i<=len;i++)
	{
	 if(serversSoftVersion[i] != currentSoftVersion[i])
	 {
		int temp = ConfirmPopup ("Version update", "Whether to update to the latest Software version");  //更新软件版本提醒
		if(temp  == 1)
		{
			return 1;
		}else
		{
			MessagePopup ("Warning", "Please update");
			SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
			return 0;
		}
	 }
	if(i == len)
	 {
		MessagePopup ("Warning", "It is the latest Software version");
		SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);	
	 }
	}
	return 0;
}
//==============================================小马哥写============================================================================== 
int	CheckHardware()  																					 //检查硬件件是否为最新  
{
	int i, len;
	len = strlen(serversHardVersion);
	for(i=0;i<=len;i++)
	{
	 if(serversHardVersion[i] != currentHardVersion[i])
	 {
		int temp = ConfirmPopup ("Version update", "Whether to update to the latest Hardware version");  //更新硬件版本提醒
		if(temp  == 1)
		{
			return 1;
		}else
		{
			MessagePopup ("Warning", "Please update");
			return 0;
		}
	 }
	if(i == len)
	 {
		MessagePopup ("Warning", "It is the latest Hardware version");
		SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE , ATTR_VISIBLE, 0);	
	 }
	}
	return 0;
}
//==============================================小马哥写============================================================================== 
static int GetVersions()
{
	int		num, i;
	char	**files = NULL;
	int len;
	SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 1); 
    if ((ftp_handle = InetFTPLogin ("106.15.183.58", "anonymous", "")) < 0)           //用户匿名登陆服务器
	{
		MessagePopup ("An Error Occurred", "Can't Connect To Server");
		return -1;
	}
	if ((InetFTPSetPassiveMode (ftp_handle, 0)) <0)                                   //ftp通信方式为主动模式
	{
		MessagePopup ("An Error Occurred", "Current communication mode is active mode");
		return -1;
	}
	if ((InetFTPChangeDir (ftp_handle, ".//SA3102//updates//")) < 0)
	{
		MessagePopup ("An Error Occurred", "Server current directory error");
		return -1;
	}
	if (( InetFTPGetDirList (ftp_handle, &files, &num)) < 0) 
	{
		MessagePopup ("An Error Occurred", "Failed to get directory list");
		return -1;
	}
	GetProjectDir (getProjectDir);
	MakePathname(getProjectDir, "..\\updates\\", getProjectDirs);
	MakePathname(getProjectDirs, files[4], updateSoftFilePath); 
	error = InetFTPRetrieveFile (ftp_handle, updateSoftFilePath, files[4], INET_FTP_FILE_TYPE_BINARY);
	if(error == -13)  
	{
		MessagePopup ("An Error Occurred", "Can't find the updates file");
		return -2;	
	}
	for(i = 3;i<num;i++)
	{    
		MakePathname(getProjectDirs, files[i], updateSoftFilePath);
		error = InetFTPRetrieveFile (ftp_handle, updateSoftFilePath, files[i], INET_FTP_FILE_TYPE_BINARY);
		if(error < 0)
		{
			MessagePopup ("An Error Occurred", "Can't Connect To Server");
			return -1;	
		}
	}
	InetFTPClose(ftp_handle);
	GetProjectDir (getProjectDir);
	MakePathname(getProjectDir, "..\\updates\\", getProjectDirs);
	MakePathname(getProjectDirs, "updateInfo.xml", updateInfoPath);
	MakePathname(getProjectDirs, "localInfo.xml", localInfoPath);
	
	CVIXMLLoadDocument (updateInfoPath, &hUpDocument);
	CVIXMLGetRootElement (hUpDocument, &hRootElem);
	CVIXMLGetChildElementByTag(hRootElem, "UpdateVersion", &hChildElem); //获得标签UpdateVersion 子元素
	CVIXMLGetAttributeByIndex(hChildElem, 0, &hUpdateCurrAttr);
	CVIXMLGetAttributeNameLength(hUpdateCurrAttr, &len);
	CVIXMLGetAttributeValueLength (hUpdateCurrAttr, &len);
	serversUpdateVersion = malloc (len + 1); 
	CVIXMLGetAttributeValue(hUpdateCurrAttr,serversUpdateVersion);   //从服务器下载.xml文件到本地目录下，再从该XML文件中获取最新更新版本号
     
    CVIXMLLoadDocument (localInfoPath, &hLoDocument);
	CVIXMLGetRootElement (hLoDocument, &hRootElem);
	CVIXMLGetChildElementByTag(hRootElem, "UpdateVersion", &hChildElem); //获得标签UpdateVersion 子元素
	CVIXMLGetAttributeByIndex(hChildElem, 0, &hUpdateCurrAttr);
	CVIXMLGetAttributeNameLength(hUpdateCurrAttr, &len);
	CVIXMLGetAttributeValueLength (hUpdateCurrAttr, &len);
	currentUpdateVersion = malloc (len + 1); 
	CVIXMLGetAttributeValue(hUpdateCurrAttr,currentUpdateVersion);   //从本地目录下XML文件中获取软件当前版本号
	
	CVIXMLLoadDocument (updateInfoPath, &hUpDocument);
	CVIXMLGetRootElement (hUpDocument, &hRootElem);
	CVIXMLGetChildElementByTag(hRootElem, "SoftwareVersion", &hChildElem); //获得标签SoftwareVersion 子元素
	CVIXMLGetAttributeByIndex(hChildElem, 0, &hSoftwareCurrAttr);
	CVIXMLGetAttributeNameLength(hSoftwareCurrAttr, &len);
	CVIXMLGetAttributeName(hSoftwareCurrAttr, attrName);
	CVIXMLGetAttributeValueLength (hSoftwareCurrAttr, &len);
	serversSoftVersion = malloc (len + 1); 
	CVIXMLGetAttributeValue(hSoftwareCurrAttr,serversSoftVersion);   //从服务器下载.xml文件到本地目录下，再从该XML文件中获取最新软件版本号
     
    CVIXMLLoadDocument (localInfoPath, &hLoDocument);
	CVIXMLGetRootElement (hLoDocument, &hRootElem);
	CVIXMLGetChildElementByTag(hRootElem, "SoftwareVersion", &hChildElem); //获得标签SoftwareVersion 子元素
	CVIXMLGetAttributeByIndex(hChildElem, 0, &hSoftwareCurrAttr);
	CVIXMLGetAttributeNameLength(hSoftwareCurrAttr, &len);
	CVIXMLGetAttributeName(hSoftwareCurrAttr, attrName);
	CVIXMLGetAttributeValueLength (hSoftwareCurrAttr, &len);
	currentSoftVersion = malloc (len + 1); 
	CVIXMLGetAttributeValue(hSoftwareCurrAttr,currentSoftVersion);   //从本地目录下XML文件中获取软件当前版本号

	CVIXMLLoadDocument (updateInfoPath, &hUpDocument);
	CVIXMLGetRootElement (hUpDocument, &hRootElem);
	CVIXMLGetChildElementByTag(hRootElem, "HardwareVersion", &hChildElem); //获得标签HardwareVersion 子元素
	CVIXMLGetAttributeByIndex(hChildElem, 0, &hHardCurrAttr);
	CVIXMLGetAttributeNameLength(hHardCurrAttr, &len);
	CVIXMLGetAttributeName(hHardCurrAttr, attrName);
	CVIXMLGetAttributeValueLength (hHardCurrAttr, &len);
	serversHardVersion = malloc (len + 1); 
	CVIXMLGetAttributeValue(hHardCurrAttr,serversHardVersion);   //从服务器下载.xml文件到本地目录下，再从该XML文件中获取最新硬件版本号
	
	CVIXMLLoadDocument (localInfoPath, &hLoDocument);
	CVIXMLGetRootElement (hLoDocument, &hRootElem);
	CVIXMLGetChildElementByTag(hRootElem, "HardwareVersion", &hChildElem); //获得标签HardwareVersion 子元素
	CVIXMLGetAttributeByIndex(hChildElem, 0, &hHardCurrAttr);
	CVIXMLGetAttributeNameLength(hHardCurrAttr, &len);
	CVIXMLGetAttributeName(hHardCurrAttr, attrName);
	CVIXMLGetAttributeValueLength (hHardCurrAttr, &len);
	currentHardVersion = malloc (len + 1); 
	CVIXMLGetAttributeValue(hHardCurrAttr,currentHardVersion);   //从本地目录下XML文件中获取硬件当前版本号
	
	return 0;
}
//===============================================小马哥写==================================================================================  
 int SoftwareSystemUpdate_Download()             //下载软件更新升级包 （下载文件）
{
	int		num, i;
	char	**files = NULL;
	GetProjectDir (getProjectDir);
	if ((ftp_handle = InetFTPLogin ("106.15.183.58", "anonymous", "")) < 0)//用户匿名登陆服务器
	{
		MessagePopup ("An Error Occurred", "Can't Connect To Server");
		return -1;
	}
	if ((InetFTPSetPassiveMode (ftp_handle, 0)) <0)                                   //ftp通信方式为主动模式
	{
		MessagePopup ("An Error Occurred", "Current communication mode is active mode");
		return -1;
	}
	if ((InetFTPChangeDir (ftp_handle, ".//SA3102//SA3102//Resource//")) < 0)
	{
		MessagePopup ("An Error Occurred", "Server current directory error");
		return -1;
	}
	if (( InetFTPGetDirList (ftp_handle, &files, &num)) < 0) 
	{
		MessagePopup ("An Error Occurred", "Failed to get directory list");
		return -1;
	}
	MakePathname(getProjectDir, "..\\softwareFileBackups\\Resource\\", getProjectDirs);
	MakePathname(getProjectDirs, files[2], updateSoftFilePath);
	error = InetFTPRetrieveFile (ftp_handle, updateSoftFilePath, files[2], INET_FTP_FILE_TYPE_BINARY);  //下载服务器当前列表中的软件文件
	if(error == -13)
	{
		MessagePopup ("An Error Occurred", "Can't find the softwareFileBackups\\Resource file");
			return -2;
	}
	for(i = 2;i<num;i++)
		{    
			 MakePathname(getProjectDir, "..\\softwareFileBackups\\Resource\\", getProjectDirs);
			 MakePathname(getProjectDirs, files[i], updateSoftFilePath);
		     if((InetFTPRetrieveFile (ftp_handle, updateSoftFilePath, files[i], INET_FTP_FILE_TYPE_BINARY)) < 0)  //下载服务器当前列表中的软件文件
			 {
				 MessagePopup ("An Error Occurred", "Check the network");
				 return -1;
			 }
		}
	if ((InetFTPChangeDir (ftp_handle, "..//Uir//")) < 0)
	{
	   MessagePopup ("An Error Occurred", "Server current directory error");
	   return -1;
	}
	if ((InetFTPGetDirList (ftp_handle, &files, &num)) < 0)
	{
		MessagePopup ("An Error Occurred", "Failed to get directory list"); 
		return -1;
	}
	MakePathname(getProjectDir, "..\\softwareFileBackups\\Uir\\", getProjectDirs);
	MakePathname(getProjectDirs, files[2], updateSoftFilePath);
	error = InetFTPRetrieveFile (ftp_handle, updateSoftFilePath, files[2], INET_FTP_FILE_TYPE_BINARY);  //下载服务器当前列表中的软件文件
	if(error == -13)
	{
		MessagePopup ("An Error Occurred", "Can't find the softwareFileBackups\\Uir file");
			return -2;
	}
	for(i = 2;i<num;i++)
		{    
			 MakePathname(getProjectDir, "..\\softwareFileBackups\\Uir\\", getProjectDirs);
			 MakePathname(getProjectDirs, files[i], updateSoftFilePath);
		     if((InetFTPRetrieveFile (ftp_handle, updateSoftFilePath, files[i], INET_FTP_FILE_TYPE_BINARY)) < 0)   //下载服务器当前列表中的软件文件
		     {
				 MessagePopup ("An Error Occurred", "Check the network");
				 return -1;
			 } 
		}
	InetFTPClose(ftp_handle);
	return 0;
}
//========================================小马哥写============================================================================
int HardwareSystemUpdate_Download()             //下载嵌入式硬件更新升级包 （下载文件）
{
	GetProjectDir (getProjectDir);
	if ((ftp_handle = InetFTPLogin ("106.15.183.58", "anonymous", "")) < 0)           //用户匿名登陆服务器
	{
		MessagePopup ("An Error Occurred", "Can't Connect To Server");
		return -1;
	}
	if ((InetFTPSetPassiveMode (ftp_handle, 0)) <0)                                   //ftp通信方式为主动模式
	{
		MessagePopup ("An Error Occurred", "Current communication mode is active mode");
		return -1;
	}
	if ((InetFTPChangeDir (ftp_handle, ".//SA3102//")) < 0)	
	{
		MessagePopup ("An Error Occurred", "Server current directory error");
		return -1;
	}
	MakePathname(getProjectDir, "..\\hardFileBackups\\SMU2.0_Code1.bin", updateHardFilePath); 
	error = InetFTPRetrieveFile (ftp_handle, updateHardFilePath, ".//SMU2.0_Code1.bin", INET_FTP_FILE_TYPE_BINARY);
	if(error == -13)  
	{
		MessagePopup ("An Error Occurred", "Can't find the hardFileBackups file");
		return -2;	
	}
	if ((error = InetFTPRetrieveFile (ftp_handle, updateHardFilePath, ".//SMU2.0_Code1.bin", INET_FTP_FILE_TYPE_BINARY)) < 0)  //下载服务器当前列表中的硬件文件
	{
		MessagePopup ("An Error Occurred", "Failed to download hardware file");
		return -1;
	}
	MakePathname(getProjectDir, "..\\hardFileBackups\\SMU2.0_Code2.bin", updateHardFilePath);
	if ((error = InetFTPRetrieveFile (ftp_handle, updateHardFilePath, ".//SMU2.0_Code2.bin", INET_FTP_FILE_TYPE_BINARY)) < 0)  //下载服务器当前列表中的硬件文件
	{
		MessagePopup ("An Error Occurred", "Failed to download hardware file");
		return -1;
	}
	InetFTPClose(ftp_handle);
	return 0;
}
//==============================================小马哥写============================================================================== 
int CVICALLBACK UpDate_Callback (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
		int updateFlag = 0;
		int temp = 0;  
		updateFlag = GetVersions();
		if(updateFlag == -1)
		{
			MessagePopup ("Update failed", "Check your network connection");
			SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
			return 0;
		}
		if(updateFlag == -2)
		{
			SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
			return 0;
		}
		temp = CheckHardware();														//检查嵌入式软件是否为最新
		if(temp == 1)
		{
			updateFlag = HardwareSystemUpdate_Download();
			if(updateFlag == -1)
			{
				MessagePopup ("Update failed", "Check your network connection");
				SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
				return 0;
			}
			if(updateFlag == -2)
			{
				SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
				return 0;
			}
			CVIXMLLoadDocument (localInfoPath, &hLoDocument);
			CVIXMLGetRootElement (hLoDocument, &hRootElem);
			CVIXMLGetChildElementByTag(hRootElem, "HardwareVersion", &hChildElem); //获得标签SoftwareVersion 子元素
			CVIXMLGetAttributeByIndex(hChildElem, 0, &hHardCurrAttr);
			CVIXMLSetAttributeValue (hHardCurrAttr, serversHardVersion);  			//将修改本地xml文件中的嵌入式软件版本号  
			CVIXMLSaveDocument (hLoDocument, 0, NULL);
			temp = 0;
		}
		if(ledFlag == 1)
		{
				temp = CheckUpdateSoftware();  	
		}else
		{
				temp = CheckSoftware();														//检车软件是否为最新      	
		}
		if(temp == 1)
		{
			updateFlag = SoftwareSystemUpdate_Download();
			if(updateFlag == -1)
			{
				MessagePopup ("Update failed", "Check your network connection");
				SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
				return 0;
			}
			if(updateFlag == -2)
			{
				SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
				return 0;
			}
			CVIXMLLoadDocument (localInfoPath, &hLoDocument);
			CVIXMLGetRootElement (hLoDocument, &hRootElem);
			CVIXMLGetChildElementByTag(hRootElem, "SoftwareVersion", &hChildElem); //获得标签SoftwareVersion 子元素
			CVIXMLGetAttributeByIndex(hChildElem, 0, &hSoftwareCurrAttr);
			CVIXMLSetAttributeValue (hSoftwareCurrAttr, serversSoftVersion); 		 //将修改本地xml文件中的版本号  
			CVIXMLSaveDocument (hLoDocument, 0, NULL);
			SetCtrlAttribute (aboutPanel,ABOUT_TEXT_UPDATE, ATTR_VISIBLE, 0);
			MessagePopup ("Warning", "Update software success");
			QuitUserInterface (0);
			LaunchExecutableEx ("..\\updates\\replace.exe", LE_HIDE, &ftp_handle);
		}
		break;
	}
	return 0;
}
void SendVersion(int num)						//更新下位机版本号
{
	int i;
	char addr[] = {0x01,0x02};
	char version[] = {0x01, 0xFE, 
					  0x33, 0x31, 0x30, 0x32,   //3102
					  0x30, 0x30, 				//00
					  0x32, 0x30,0x34, 			//204  硬件电路版本
					  0x32, 0x30, 0x34,			//204  嵌入式软件版本
					  0x31, 0x30, 0x30, 		//101  机械外壳电源
					  0x30, 0x30, 0x30, 0x32,   //0002 同类产品递增
					  0x00, 					//地址位
					  0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,0x30, 0x30, 
					  0x00						//校验位
					 };	
	version[0]   = addr[num-1]; 			   //发送地址
	version[21]  = 0x30 + addr[num-1]; 		   //写入ID地址
	version[11] =  serversHardVersion[4];
	version[12] =  serversHardVersion[5]; 
	version[13] =  serversHardVersion[6]; 
	for(i=0;i<31;i++)
	{
		version[31]^=version[i];
	}
	ComWrt(measureComPort, version, 32);
}
int ReadFromConfigFile(char* temp,char* temp1)
{
	
	FILE* fp;
	char buf[40*1024]={0};
	int size;
	ComWrt(measureComPort,temp1, 32);
    Delay(1);
    temp1[0]='Y';
	fp=fopen(temp,"rb");
	if(fp!=NULL)
	{
	ComWrt(measureComPort,temp1, 1);
	Delay(0.1);
	size=fread(buf, 1,40*1024,fp);
	ComWrt(measureComPort,buf,size);
	Delay(4);
	*temp1 = 0;
	*(temp1+1) = 0; 
	*(temp1+31) = 0;
	ComWrt(measureComPort,temp1, 32);
	Delay(0.3); 
	ComWrt(measureComPort,temp1, 32);
	Delay(0.3); 
	fclose(fp);
	Delay(3);
	MessagePopup("Message","Updata successfully");
	}
	else
	MessagePopup("Message","File not found");
	return 0;
}

//更新SNU1代码
int CVICALLBACK UpdateSMU1Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{   
	int  strLen;
	char temp[MAX_PATHNAME_LEN]="..//hardFileBackups//SMU2.0_Code1.bin";
	char temp1[MAX_PATHNAME_LEN]={0x01,0x16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x17};		//更新跳转命令
	char temp_Check[MAX_PATHNAME_LEN]={0x01,0x17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x16};	//握手命令
	char USART_RX_Buffer_Check[60];
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			ComWrt(measureComPort,temp_Check, 32);													//发送握手命令
			Delay(3);
			strLen = GetInQLen (measureComPort);										
			ComRd (measureComPort,USART_RX_Buffer_Check,strLen);									//读取下位机握手命令，等待握手。
			if((USART_RX_Buffer_Check[0] == 0x01) && (USART_RX_Buffer_Check[1] == 0x17))
			{
				FlushInQ(measureComPort);	   														//Clear input and output buffer
				FlushOutQ(measureComPort);
				Delay(1);
				ReadFromConfigFile(temp,temp1);														//更新下位机代码
				Delay(1); 
				SendVersion(1); 																	//修改对应板卡版本号
			}
			else
			{
				FlushInQ(measureComPort);	   														//Clear input and output buffer
				FlushOutQ(measureComPort);
				MessagePopup("Message","File not found"); 
			}
			break;
	}
	return 0;
}
//更新SNU2代码 
int CVICALLBACK UpdateSMU2Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{   
	int  strLen; 
	char temp[MAX_PATHNAME_LEN]="..//hardFileBackups//SMU2.0_Code2.bin";
	char temp1[MAX_PATHNAME_LEN]={0x02,0x16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x14};
	char temp_Check[MAX_PATHNAME_LEN]={0x02,0x17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x15};  //握手命令
	char USART_RX_Buffer_Check[60];
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			ComWrt(measureComPort,temp_Check, 32);													//发送握手命令
			Delay(3);
			strLen = GetInQLen (measureComPort);										
			ComRd (measureComPort,USART_RX_Buffer_Check,strLen);									//读取下位机握手命令，等待握手。
			if((USART_RX_Buffer_Check[0] == 0x02) && (USART_RX_Buffer_Check[1] == 0x17))
			{
				FlushInQ(measureComPort);	   														//Clear input and output buffer
				FlushOutQ(measureComPort);
				Delay(1);
				ReadFromConfigFile(temp,temp1);														//更新下位机代码
				Delay(1); 
				SendVersion(2); 																//修改对应板卡版本号
			}
			else
			{
				FlushInQ(measureComPort);	   														//Clear input and output buffer
				FlushOutQ(measureComPort);
				MessagePopup("Message","File not found"); 
			}
			break;
	}
	return 0;
}

int CVICALLBACK PrintCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			DisplayImageFile (mainPanel, MAIN_PANEL_PRINT, "Resource\\print_pressed.ico"); 
			PrintPanel (mainPanel, "", 0, VAL_VISIBLE_AREA, 1);
			DisplayImageFile (mainPanel, MAIN_PANEL_PRINT, "Resource\\print.ico");
			break;
	}
	
	return 0;
}

int CVICALLBACK ReadportCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			int SA3102_UART_Status=0;  
			ReadSerialPort(&measureComPort,&controlComPort); 
			SA3102_UART_Status = CheckOpenCom(&measureComPort,&controlComPort,&measure_Uart_Flag,&control_Uart_Flag);
			

			break;
	}
	return 0;
}
int CVICALLBACK LedCallback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			SetCtrlAttribute (aboutPanel, ABOUT_LED, ATTR_OFF_COLOR, VAL_BLACK);
			ledFlag = 1;	
			break;
	}
	return 0;
}
