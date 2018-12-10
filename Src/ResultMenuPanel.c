//==============================================================================
//
// Title:		LoadPanel.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/16 at 15:21:14 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files 
#include "excelreport.h"
#include "excel2000.h"
#include "GraphDisp.h"
#include <userint.h>
#include "toolbox.h"   
#include "ResultMenuPanel.h"
#include "Graph.h"
#include "LoadPanel.h"
#include "Result Menu.h" 
//==============================================================================
// Constants

//==============================================================================
// Types
enum ResultDispSelect
{
	DISP_TABLE=0,
	DISP_GRAPH=1
};
enum GraphDispSelect
{
	DISP_SINGLE_GRAPH=0,
	DISP_DOUBLE_GRAPH=1	
};
//==============================================================================
// Static global variables
char sheetSavePath[512];
char graph1SavePath[512];
char graph2SavePath[512];
Rect rc;
int nBitmapID;
//==============================================================================
// Static functions
//==============================================================================
// Global variables
enum ResultDispSelect resultDispSelect=DISP_GRAPH;
enum GraphDispSelect graphDispSelect=DISP_SINGLE_GRAPH;
//==============================================================================
// Global functions	

static void DispTable(void)
{
	SetPanelPos(tablePanel, 172, 305);        
	DisplayPanel(tablePanel);
}

static void DispSingleGraph(void)
{
	SetPanelPos(graphDispPanel, 172, 305);  
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH1 , ATTR_HEIGHT, 700);
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH2, ATTR_VISIBLE, 0);
	DisplayPanel(graphDispPanel);
	SetCtrlAttribute (hEnvResultPanel, ENVIRPANEL_LEGENDCHECK, ATTR_VISIBLE, 0);
	SetCtrlAttribute (hEnvResultPanel, ENVIRPANEL_TXT_LEGEND, ATTR_VISIBLE, 0);
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_VISIBLE, 0);  
}

static void DispDoubleGraph(void)
{
	SetPanelPos(graphDispPanel, 172, 305); 
	SetCtrlAttribute (graphDispPanel,GRAPHDISP_GRAPH1 , ATTR_HEIGHT, 380); //如果CheckBox是选中状态则显示两个graph     
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_GRAPH2, ATTR_VISIBLE, 1);
	DisplayPanel(graphDispPanel);
	SetCtrlAttribute (hEnvResultPanel, ENVIRPANEL_LEGENDCHECK, ATTR_VISIBLE, 1);
	SetCtrlAttribute (hEnvResultPanel, ENVIRPANEL_TXT_LEGEND, ATTR_VISIBLE, 1);
	
	int legendval;
	GetCtrlVal(hEnvResultPanel, ENVIRPANEL_LEGENDCHECK, &legendval);
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_VISIBLE, legendval);
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, 1010); 
	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, 420);

	
}

static void DispGraphSelectCheckBox(void)
{
	if(graphDispSelect==DISP_SINGLE_GRAPH)
	{
		SetCtrlVal(chPanel, CHPANEL_CHECKBOX, 0);
	}
	else{
		SetCtrlVal(chPanel, CHPANEL_CHECKBOX, 1);
	 
	}
	SetPanelPos(chPanel, 172, 1457);      
 	DisplayPanel(chPanel);
}
void DispResultTableGraph(void)
{
	if(resultDispSelect==DISP_TABLE)
		DispTable();
	else
	{
	  	if(graphDispSelect==DISP_SINGLE_GRAPH) 
		DispSingleGraph();
		else if(graphDispSelect==DISP_DOUBLE_GRAPH)
		DispDoubleGraph();
	}
}
//======================Table=================================  
int CVICALLBACK TableCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	
	//点击table图标切换到table面板
	switch(event)
	{
		case EVENT_LEFT_CLICK_UP:
			DisplayImageFile (resultPanel, RESULTMENU_TABLE, "Resource\\Table_pressed.ico"); 
			DisplayImageFile (resultPanel, RESULTMENU_GRAPH, "Resource\\Graph.ico"); 
			DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
			resultDispSelect=DISP_TABLE;
			DispResultTableGraph();
			break;
	}
	return 0;
}
//======================Graph=================================  
int CVICALLBACK GraphCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int val; 
	switch(event)			   //点击graph图标切换到graph面板
	{
		case EVENT_LEFT_CLICK_UP:
			
			DisplayImageFile (resultPanel, RESULTMENU_GRAPH, "Resource\\Graph_pressed.ico");
			DisplayImageFile (resultPanel, RESULTMENU_TABLE, "Resource\\Table.ico"); 
			DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
			
			if(resultDispSelect==DISP_GRAPH)				//如果此时在显示图的时候还去点击图的图标，则认为是想切换显示
			{	
		  		DispGraphSelectCheckBox();
			}
			else
			{
				resultDispSelect=DISP_GRAPH;
				DispResultTableGraph();
			}
			
			
	  	    GetCtrlVal(chPanel, CHPANEL_CHECKBOX, &val);
			if(val)
			{
				DisplayImageFile (resultPanel, RESULTMENU_GRAPH, "Resource\\DoubleGraph.ico");
			}
			else
			{
				DisplayImageFile (resultPanel, RESULTMENU_GRAPH, "Resource\\Graph_pressed.ico");
			}
			
				
			break;
	}	
	return 0;
}
//======================saveSheet==========================
static void savesheet()
{
	int    row,column;
	char Z = 65; 
	//char Z1 = 66;
	char   str[20];
	static ExcelObj_App               ExcelAppHandle       = 0;				//Excel应用程序对象句柄       
	static ExcelObj_Workbooks         ExcelWorkbooksHandle = 0; 			//工作簿集合对象句柄
	static ExcelObj_Workbook          ExcelWorkbookHandle  = 0;  			//工作簿对象句柄
	static ExcelObj_Sheets            ExcelSheetsHandle    = 0;    			//工作表集合对象句柄
	static ExcelObj_Worksheet         ExcelWorksheetHandle = 0;				//单张表格对象句柄
	
	GetNumTableRows (tablePanel, TABLE_TABLE1, &row);						//返回指定表控件中的行数。
	GetNumTableColumns (tablePanel,TABLE_TABLE1,&column);					//返回指定表格控件中的列数
	
	Z = Z + (char)column;																				//计算单元格范围 起始：A1--Zn  目前最大26列 
	sprintf(str,"%s%c%d","A1:",Z,row+1);																//
	Excel_NewApp (NULL, 1, LOCALE_NEUTRAL, 0, &ExcelAppHandle);	  										//create a new Application object, and obtain a handle to the object.创建新的应用程序对象，并获取对象的句柄。
	// Excel_SetProperty (ExcelAppHandle, NULL, Excel_AppVisible, CAVT_BOOL, VTRUE);	  				//set application visible
	Excel_GetProperty (ExcelAppHandle, NULL, Excel_AppWorkbooks, CAVT_OBJHANDLE, &ExcelWorkbooksHandle);
	Excel_WorkbooksAdd (ExcelWorkbooksHandle, NULL, CA_DEFAULT_VAL,&ExcelWorkbookHandle);
	Excel_GetProperty (ExcelAppHandle, NULL, Excel_AppSheets,CAVT_OBJHANDLE, &ExcelSheetsHandle);
	Excel_SheetsItem (ExcelSheetsHandle, NULL, CA_VariantInt(1),&ExcelWorksheetHandle);					//得到工作表集合中第一张表格对象句柄
	Excel_WorksheetActivate (ExcelWorksheetHandle, NULL);
	ExcelRpt_WriteDataFromTableControl (ExcelWorksheetHandle, str, tablePanel, TABLE_TABLE1);			//write data from table control
	ExcelRpt_WorkbookSave (ExcelWorkbookHandle, sheetSavePath, ExRConst_DefaultFileFormat);
	Excel_AppQuit (ExcelAppHandle, NULL);
	
	CA_DiscardObjHandle(ExcelAppHandle);																//释放句柄
	CA_DiscardObjHandle(ExcelWorkbooksHandle);
	CA_DiscardObjHandle(ExcelWorkbookHandle); 
	CA_DiscardObjHandle(ExcelSheetsHandle); 
	CA_DiscardObjHandle(ExcelWorksheetHandle); 
}

//======================Chose=================================
int CVICALLBACK ChoseCallback (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int val; 
	if( event == EVENT_LEFT_CLICK_UP) //EVENT_VAL_CHANGED
		{
			HidePanel(chPanel); 
		  	GetCtrlVal(chPanel, CHPANEL_CHECKBOX, &val);
		    if(val)
			{	
			    DisplayImageFile (resultPanel, RESULTMENU_GRAPH, "Resource\\DoubleGraph.ico");
				DispDoubleGraph();
				graphDispSelect=DISP_DOUBLE_GRAPH;
			}
			else
			{   
				DisplayImageFile (resultPanel, RESULTMENU_GRAPH, "Resource\\Graph_pressed.ico");
				DispSingleGraph();
				graphDispSelect = DISP_SINGLE_GRAPH;
			}
		}
	return 0;
}
//======================SaveData=================================  
int CVICALLBACK SaveDataCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch(event)
	{
		case EVENT_LEFT_CLICK_UP:
 
			DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData_pressed.ico"); 
			/*InstallPopup(saveDataPanel);
			HidePanel (RemindsaveDataPanel);//弹出savedata面板 */
			//RemovePopup(RemindsaveDataPanel);
			
			DisplayPanel(saveDataPanel);
			if(graphDispSelect==DISP_SINGLE_GRAPH)  
			{
				SetCtrlAttribute (saveDataPanel, SAVEDATA_BROWSEGRAPH2, ATTR_DIMMED, 1);
				SetCtrlAttribute (saveDataPanel, SAVEDATA_SAVEGRAPH2, ATTR_DIMMED, 1); 
			}
			else if(graphDispSelect==DISP_DOUBLE_GRAPH)
			{
				SetCtrlAttribute (saveDataPanel, SAVEDATA_BROWSEGRAPH2, ATTR_DIMMED, 0);
				SetCtrlAttribute (saveDataPanel, SAVEDATA_SAVEGRAPH2, ATTR_DIMMED, 0); 
			}
			break;
	}	
	return 0;
}
//======================Exit=================================  
int CVICALLBACK ExitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			//RemovePopup (saveDataPanel);	 													//移除、关闭savedata面板 
			HidePanel (saveDataPanel);															//隐藏saveDataPanel面板
			DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
			
		break;
			
	}
	return 0; 
}
//======================BrowseSheet=================================
int CVICALLBACK BrowseSheetCallback (int panel, int control, int event,
								   	 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{			
		case EVENT_LEFT_CLICK_UP:
			if(FileSelectPopup ("C:\\SINOAGG\\SA6101\\", ".xlsx", "*.xlsx", "Select Path", VAL_OK_BUTTON, 0, 1, 1, 1, sheetSavePath)>0)
			{
				SetCtrlVal(panel, SAVEDATA_SHEETPATH, sheetSavePath); 
			}
			else 
				return -1;
			break;
	}
	return 0;
}
//======================BrowseGraph1=================================		 
int CVICALLBACK BrowseGraph1Callback (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
		     
			if(FileSelectPopup ("C:\\SINOAGG\\SA6101\\UserData", ".jpg", "*.jpg;*.png;*.bmp;*.tif", "Select Path", VAL_OK_BUTTON, 0, 0, 1, 1, graph1SavePath)>0)
			{
				SetCtrlVal(panel, SAVEDATA_GRAPH1PATH, graph1SavePath);
			}
			break;
	}
	return 0;
}
//======================BrowseGraph2=================================
int CVICALLBACK BrowseGraph2Callback (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			if(FileSelectPopup ("C:\\SINOAGG\\SA6101\\UserData", ".jpg", "*.jpg;*.png;*.bmp;*.tif", "Select Path", VAL_OK_BUTTON, 0, 0, 1, 1, graph2SavePath)>0)
			{
				SetCtrlVal(panel, SAVEDATA_GRAPH2PATH, graph2SavePath);
			}
			break;
	}
	return 0;
}

//=======================saveGraph1=====================
static void savegraph1()
{
	GetCtrlDisplayBitmap (graphDispPanel, GRAPHDISP_GRAPH1, 0, &nBitmapID);	   //dlp
	SaveBitmapToJPEGFile (nBitmapID, graph1SavePath, JPEG_INTERLACE, 100);
	DiscardBitmap (nBitmapID);
}
//=======================saveGraph2=====================
static void savegraph2()
{
	GetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS_2, ATTR_TOP, &rc.top);		//得到所要截取的波形图表坐标  
	GetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS_2, ATTR_LEFT, &rc.left);
	GetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS_2, ATTR_HEIGHT, &rc.height);
	GetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS_2, ATTR_WIDTH, &rc.width);
	GetPanelDisplayBitmap (graphDispPanel, VAL_FULL_PANEL, rc, &nBitmapID);		 //dlp
	SaveBitmapToJPEGFile (nBitmapID, graph2SavePath, JPEG_INTERLACE, 100);
	DiscardBitmap (nBitmapID);
}
//=======================SaveGraph1Callback===================== 
int CVICALLBACK SaveGraph1Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)	//保存图表2数据
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
		
		GetCtrlVal(panel, SAVEDATA_GRAPH1PATH, graph1SavePath);   				 											//得到用户输入的路径 
		
		if(graph1SavePath[0]=='\0')
		{
			MessagePopup ("", "Please select a path!"); 	
		}
		else
		{
			if( FileExists (graph1SavePath,0))
			{
				if( ConfirmPopup ("Warning", "File already exists.Do you want to replace the original file ?") )
				{
					InstallPopup(RemindsaveDataPanel);
					savegraph1();
					RemovePopup(RemindsaveDataPanel);
					MessagePopup("Tips","Saved successfully!!!");
					DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");	
				}
			}
			else
			{
				InstallPopup(RemindsaveDataPanel);
				savegraph1();
				RemovePopup(RemindsaveDataPanel);
				MessagePopup("Tips","Saved successfully!!!");
				DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
			}
			
		}
		break;
	}
	return 0;
}
//=======================SaveGraph2Callback===================== 
int CVICALLBACK SaveGraph2Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)	//保存图表2数据
{

	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
		
		GetCtrlVal(panel, SAVEDATA_GRAPH2PATH, graph2SavePath);   				 											//得到用户输入的路径
			
		if(graph2SavePath[0]=='\0') 
		{
			MessagePopup ("", "Please select a path!");
		}
		else
		{
			if( FileExists (graph2SavePath,0))
			{
				if( ConfirmPopup ("Warning", "File already exists.Do you want to replace the original file ?") )
				{
					InstallPopup(RemindsaveDataPanel);
					savegraph2();
					RemovePopup(RemindsaveDataPanel);
					MessagePopup("Tips","Saved successfully!!!");
					DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
				}
			}
			else
			{
				InstallPopup(RemindsaveDataPanel);
				savegraph2();
				RemovePopup(RemindsaveDataPanel);
				MessagePopup("Tips","Saved successfully!!!");
				DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
			}
			
		}
		break;
	}
	return 0;
}
//=======================SaveSheetCallback=====================
int CVICALLBACK SaveSheetCallback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)	//保存表格数据
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
		
			GetCtrlVal(panel, SAVEDATA_SHEETPATH, sheetSavePath);  
			
			if(sheetSavePath[0]=='\0')
			{
				MessagePopup ("", "Please select a path!"); 
			}
			else
			{
				if( FileExists (sheetSavePath,0))
				{
					if( ConfirmPopup ("Warning", "File already exists.Do you want to replace the original file ?") )
					{
						InstallPopup(RemindsaveDataPanel);
						savesheet();
						RemovePopup(RemindsaveDataPanel);
						MessagePopup("Tips","Saved successfully!!!");
						DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
					}
				}
				else
				{
					InstallPopup(RemindsaveDataPanel);
					savesheet();
					RemovePopup(RemindsaveDataPanel);
					MessagePopup("Tips","Saved successfully!!!");
					DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
				}
				
			}
			break;
	}
	return 0;
}


//=======================SaveAllCallback=====================
int CVICALLBACK SaveAllCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
		GetCtrlVal(panel, SAVEDATA_GRAPH1PATH, graph1SavePath);   				 											//得到用户输入的路径 
		GetCtrlVal(panel, SAVEDATA_GRAPH2PATH, graph2SavePath);   				 											
		GetCtrlVal(panel, SAVEDATA_SHEETPATH , sheetSavePath);  
			
			if(graphDispSelect==DISP_SINGLE_GRAPH)  
			{
				if((sheetSavePath[0]=='\0')||(graph1SavePath[0]=='\0'))
					MessagePopup ("", "Please select a path!");  
				else
				{
					if( (FileExists (sheetSavePath,0))||( FileExists (graph1SavePath,0) ))
					{
						if( ConfirmPopup ("Warning", "File already exists.Do you want to replace the original file ?") )
						{
							InstallPopup(RemindsaveDataPanel);
							savesheet();
							savegraph1();
							RemovePopup(RemindsaveDataPanel);
							MessagePopup("Tips","Saved successfully!!!");
							DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
						}
					}
					else
					{
						InstallPopup(RemindsaveDataPanel);
						savesheet();
						savegraph1();
						RemovePopup(RemindsaveDataPanel);
						MessagePopup("Tips","Saved successfully!!!");
						DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
					}
				}
			}
			else if(graphDispSelect==DISP_DOUBLE_GRAPH)
			{
				if((sheetSavePath[0]=='\0')||(graph2SavePath[0]=='\0')||(graph1SavePath[0]=='\0')) 
					MessagePopup ("", "Please select a path!"); 
				else
				{
					if( (FileExists (sheetSavePath,0))||( FileExists (graph1SavePath,0) )||( FileExists (graph2SavePath,0) ))
					{
						if( ConfirmPopup ("Warning", "File already exists.Do you want to replace the original file ?") )
						{
							InstallPopup(RemindsaveDataPanel);
							savesheet();
							savegraph1();
							savegraph2();
							RemovePopup(RemindsaveDataPanel);
							MessagePopup("Tips","Saved successfully!!!");
							DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
						}
					}
					else
					{
						InstallPopup(RemindsaveDataPanel);
						savesheet();
						savegraph1();
						savegraph2();
						RemovePopup(RemindsaveDataPanel);
						MessagePopup("Tips","Saved successfully!!!");
						DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");
					}
				}
			}
		break;
	}
	return 0;
}


static int SaveGraph(int panel, int control, int plotHandle, const char path[])
{
	int bitmapID;
	
	
	
	if(plotHandle<=0)																//no valid plot behavior
		MessagePopup ("Message", "No valid plot. Please run test first.");
	else
	{
		if(GetCtrlBitmap(panel, control, plotHandle,&bitmapID)<0)
			return -1;
		if(SaveBitmapToFile(path, bitmapID)<0)		 								//need check the file format
			return -1;
		DiscardBitmap(bitmapID);
	}
	return 0;
}

int CVICALLBACK SaveDataPanelCallbck (int panel, int event, void *callbackData,
									  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			
			RemovePopup (saveDataPanel);	 //移除、关闭savedata面板  
			DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");

			break;
	}
	return 0;
}

int CVICALLBACK REMINDSAVE_CB (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			
			RemovePopup (RemindsaveDataPanel);	 //移除、关闭savedata面板  
			DisplayImageFile (resultPanel, RESULTMENU_SAVE, "Resource\\SaveData.ico");

			break;
	}
	return 0;
}
