//==============================================================================
//
// Title:		Environment.c
// Purpose:		A short description of the implementation.
//
// Created on:	2018/7/26 at 15:46:52 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <userint.h>
#include "Environment.h"
#include "LoadPanel.h"
#include "Cgs_mt.h" 
#include "Graph.h"
#include "Plot.h"
#include "utility.h"
#include "GraphDisp.h"
#include "EnvironmentDisp.h"
#include "MainPanelCb.h"
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
int temp_flag,humidity_flag,pressure_flag;
int tempVal,humidityVal,pressureVal,measureval; 
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?

int CVICALLBACK MesaureCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{ 
	int CheckValue;
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			GetCtrlVal(panel,control,&CheckValue);//Measure ��ѡ 
			if(CheckValue)
			{
				SetCtrlAttribute (panel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 0);		//�ָ�ǰ���������ť
				SetCtrlAttribute (panel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 0);
				
				SetCtrlVal(panel,ENVIRONMEN_TEMPTER,CheckValue);
				SetCtrlVal(panel,ENVIRONMEN_HUMIDITY,CheckValue);
				SetCtrlVal(panel,ENVIRONMEN_PRESSURE,CheckValue);
				
			} 
			else
			{
				SetCtrlVal(panel,ENVIRONMEN_TEMPTER,0);
				SetCtrlVal(panel,ENVIRONMEN_HUMIDITY,0);
				SetCtrlVal(panel,ENVIRONMEN_PRESSURE,0);
				
				SetCtrlAttribute (panel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		//����ǰ���������ť
				SetCtrlAttribute (panel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);
				
			}
			SetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX,CheckValue);			//�����û���ѡ����ʾ��Ӧ�Ļ�����������
			SetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_2,CheckValue); 
			SetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_3,CheckValue);
	
		break;
	}
	return 0;
}


int CVICALLBACK Humidity_displayCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{   
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			temp_hum_pre_display();

			break;
	}
	return 0;
}

int CVICALLBACK Temp_displayCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			temp_hum_pre_display();

			break;
	}
	return 0;
}

int CVICALLBACK Pressure_displayCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			temp_hum_pre_display();
			
			break;
	}
	return 0;
}
//===========================canvas==================================
int CVICALLBACK CANVAS_CB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int LeftButtonDown;
	int x;
	int y;
	int CtrlTop;
	int CtrlLeft;
	static int OldX = 0;	  //�ֲ���̬��������X������ֵ 
	static int OldY = 0;	
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE:
			GetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, &CtrlLeft);
			GetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, &CtrlTop);
			GetGlobalMouseState (&panel, &x, &y, &LeftButtonDown, NULL, NULL);	     // ���������λ�á�����������
			if (LeftButtonDown == 1)	  											 // ������������ʱ
			{
				SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, CtrlLeft + (x-OldX) );
				SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, CtrlTop + (y-OldY) );
				OldX = x;
				OldY = y;				  				 // ����X��Y ����ֵ�Ե�ǰ��ֵ���� 
				if( ( CtrlTop + (y-OldY) )<420)			 //ͼ�����ܳ���graph2�ķ�Χ
				{
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, CtrlLeft + (x-OldX) );
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, 420 );
				}
				if( ( CtrlTop + (y-OldY) )>670)
				{
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, CtrlLeft + (x-OldX) );
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, 670 );
				}
				if( (CtrlLeft + (x-OldX) )<15)
				{
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, 15 );
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, CtrlTop + (y-OldY) );
				}
				if( (CtrlLeft + (x-OldX) )>1010)
				{
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, 1010 );
					SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, CtrlTop + (y-OldY) );
				}
			}
			OldX = x;
			OldY = y;					  // ����X������ֵ�Ե�ǰ��ֵ����
		break;
	}
	return 0;
}
int CVICALLBACK Legend_display_CB (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{   
	int legendval;
	if( event == EVENT_VAL_CHANGED) 
	{   
		GetCtrlVal(hEnvResultPanel, ENVIRPANEL_LEGENDCHECK, &legendval);
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_VISIBLE, legendval);
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_LEFT, 1010);   //����ͼ��λ��
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_TOP, 420);
		if(legendval)
		{
			GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX,&temp_flag);			//Ȼ���ȡ�û�Ҫѡ�е�����
			GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_2,&humidity_flag); 
			GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_3,&pressure_flag);
		}
		if(temp_flag==1)
		{
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);   									//text����ɫ(��canvas��ɫ��ͬ)
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);		 									//"Temperature"��ɫ
			CanvasDrawText (graphDispPanel, GRAPHDISP_CANVAS, "T(��)",VAL_APP_META_FONT, MakeRect(40,5,15,70), VAL_CENTER);	        //д"Temperature"
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, CurveAttr.tempColor);			   					//Temperatureͼ��
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_WIDTH, 2);			        								//�ߵĿ��
			CanvasDrawLine (graphDispPanel, GRAPHDISP_CANVAS, MakePoint (110,47), MakePoint (150, 47));	
		}
		else
		{
			CanvasClear (graphDispPanel, GRAPHDISP_CANVAS, MakeRect(40,15,15,140)); 
		}
		if(humidity_flag==1)
		{
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);    								//text����ɫ(��canvas��ɫ��ͬ)
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);		 									//"humidity"��ɫ
			CanvasDrawText (graphDispPanel, GRAPHDISP_CANVAS, "H(%RH)",VAL_APP_META_FONT, MakeRect(60,15,15,70), VAL_CENTER);	    //д"humidity"
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, CurveAttr.humColor);			  					//humidityͼ��
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_WIDTH, 2);			        								//�ߵĿ��
			CanvasDrawLine (graphDispPanel, GRAPHDISP_CANVAS, MakePoint (110, 67), MakePoint (150, 67));
		}
		else
		{
			CanvasClear (graphDispPanel, GRAPHDISP_CANVAS, MakeRect(60,15,15,140));
		}
		if(pressure_flag==1)
		{
			SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);   									//text����ɫ(��canvas��ɫ��ͬ)
	    	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);		 									//"pressure"��ɫ
	    	CanvasDrawText (graphDispPanel, GRAPHDISP_CANVAS, "p(KPa)",VAL_APP_META_FONT, MakeRect(80,10,15,70), VAL_CENTER);		//д"pressure"
	    	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, CurveAttr.pressColor);			    				//pressureͼ��
	    	SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_WIDTH, 2);			       									//�ߵĿ��
	    	CanvasDrawLine (graphDispPanel, GRAPHDISP_CANVAS, MakePoint (110, 87), MakePoint (150, 87));
		}
		else
		{
			CanvasClear (graphDispPanel, GRAPHDISP_CANVAS, MakeRect(80,15,15,140));
		}
	}
	return 0;
}

int CVICALLBACK TempCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			//int val;
			GetCtrlVal (panel, ENVIRONMEN_TEMPTER, &tempVal);
			GetCtrlVal (panel, ENVIRONMEN_HUMIDITY, &humidityVal); 
			GetCtrlVal (panel, ENVIRONMEN_PRESSURE, &pressureVal); 
			if(tempVal==0&&humidityVal==0&&pressureVal==0)
			{
				SetCtrlVal (hEnvCfgPanel,ENVIRONMEN_MEASURE,0); 
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		//����
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);
			}
			SetCtrlVal (hEnvResultPanel,ENVIRPANEL_CHECKBOX,tempVal);
			
			break;
	}
	return 0;
}

int CVICALLBACK HumidityCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			//int val; 
			GetCtrlVal (panel, ENVIRONMEN_TEMPTER, &tempVal);
			GetCtrlVal (panel, ENVIRONMEN_HUMIDITY, &humidityVal); 
			GetCtrlVal (panel, ENVIRONMEN_PRESSURE, &pressureVal); 
			if(tempVal==0&&humidityVal==0&&pressureVal==0)
			{
				SetCtrlVal (hEnvCfgPanel,ENVIRONMEN_MEASURE,0); 
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		//����
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);
			}
			SetCtrlVal (hEnvResultPanel,ENVIRPANEL_CHECKBOX_2,humidityVal);

			break;
	}
	return 0;
}

int CVICALLBACK PressCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			//int val;
			GetCtrlVal (panel, ENVIRONMEN_TEMPTER, &tempVal);
			GetCtrlVal (panel, ENVIRONMEN_HUMIDITY, &humidityVal); 
			GetCtrlVal (panel, ENVIRONMEN_PRESSURE, &pressureVal); 
			if(tempVal==0&&humidityVal==0&&pressureVal==0)			  //���������ȡ����ѡ�����measureҲȡ����ͬʱ����
			{
				SetCtrlVal (hEnvCfgPanel,ENVIRONMEN_MEASURE,0); 
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_TEMPTER, ATTR_DIMMED, 1);		//����
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_HUMIDITY, ATTR_DIMMED, 1);
				SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_PRESSURE, ATTR_DIMMED, 1);
			}
			SetCtrlVal (hEnvResultPanel,ENVIRPANEL_CHECKBOX_3,pressureVal);

			break;
	}
	return 0;
}
