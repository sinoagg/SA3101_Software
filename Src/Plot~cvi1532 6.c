//==============================================================================
//
// Title:		Plot.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/16 at 17:35:20 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//#include "Plot.h"
#include <userint.h>
#include "Graph.h"
#include "GraphDisp.h"
#include "LoadPanel.h"
#include "Environment.h"
#include "SetPanelCB.h"

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
int graph2tempclr;
int graph2humclr; 
int graph2preclr; 
int smu1Clr;
int smu2Clr;
//==============================================================================
// Global functions
int PlotCurve1(Graph_TypeDef* pGraph, int graphDispPanel, int control, int indexCurveArray) 
{
	int numOfDotsToPlot=(pGraph->pCurveArray + indexCurveArray)->numOfDotsToPlot;		//防止中断端去写入这个数据 
	if(numOfDotsToPlot>0)																//如果有需要画图的点
	{
		if((pGraph->pCurveArray + indexCurveArray)->numOfPlotDots >= 1)					//如果有需要画图的点
		{
			
			SetCtrlVal (hResultDispPanel, SAMPLE_VD, *(Graph.pCurveArray->pDotX-1) );
			SetCtrlVal (hResultDispPanel, SAMPLE_VG, *(Graph.pCurveArray->pDotY-1));
			
			pGraph->plotHandle=PlotXY(graphDispPanel, control, (pGraph->pCurveArray + indexCurveArray)->pDotXPlot-1, (pGraph->pCurveArray + indexCurveArray)->pDotYPlot-1, numOfDotsToPlot+1, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, smu1Clr);  
		}
		else
		{
			pGraph->plotHandle=PlotXY(graphDispPanel, control, (pGraph->pCurveArray + indexCurveArray)->pDotXPlot, (pGraph->pCurveArray + indexCurveArray)->pDotYPlot, numOfDotsToPlot, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, smu1Clr); 
		}
	(pGraph->pCurveArray + indexCurveArray)->numOfPlotDots+=numOfDotsToPlot;		//画图总点数递增
	(pGraph->pCurveArray + indexCurveArray)->pDotXPlot+=numOfDotsToPlot;			//画图点X坐标指针递增
	(pGraph->pCurveArray + indexCurveArray)->pDotYPlot+=numOfDotsToPlot;			//画图点Y坐标指针递增
	(pGraph->pCurveArray + indexCurveArray)->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
	}
	if(pGraph->plotHandle<0)
		return -1;
	else
		return 0;
}

int PlotCurve(Graph_TypeDef* pGraph, int graphDispPanel, int control)							//双源表画俩条曲线
{
	int numOfDotsToPlot=pGraph->pCurveArray->numOfDotsToPlot;									//防止中断端去写入这个数据 
	int numOfDotsToPlot2=(pGraph->pCurveArray + 1)->numOfDotsToPlot;							//防止中断端去写入这个数据 
	
	if(numOfDotsToPlot>0)//如果有需要画的点
	{
		if(Graph.pCurveArray->numOfPlotDots >=1 )//画 第二个 点  
		{
			SetCtrlVal (hResultDispPanel, SAMPLE_VD, *(Graph.pCurveArray->pDotX-1));
			SetCtrlVal (hResultDispPanel, SAMPLE_VG, *(Graph.pCurveArray->pDotY-1));
			pGraph->plotHandle=PlotXY(graphDispPanel, control, pGraph->pCurveArray->pDotXPlot-1, pGraph->pCurveArray->pDotYPlot-1, numOfDotsToPlot+1, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, smu1Clr);
		}else
		{
			SetCtrlVal (hResultDispPanel, SAMPLE_VD, *(Graph.pCurveArray->pDotX-1));
			SetCtrlVal (hResultDispPanel, SAMPLE_VG, *(Graph.pCurveArray->pDotY-1));
			pGraph->plotHandle=PlotXY(graphDispPanel, control, pGraph->pCurveArray->pDotXPlot, pGraph->pCurveArray->pDotYPlot, numOfDotsToPlot, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, smu1Clr);
		}
			pGraph->pCurveArray->numOfPlotDots+=numOfDotsToPlot;		//画图总点数递增
			pGraph->pCurveArray->pDotXPlot+=numOfDotsToPlot;			//画图点X坐标指针递增
			pGraph->pCurveArray->pDotYPlot+=numOfDotsToPlot;			//画图点Y坐标指针递增
			pGraph->pCurveArray->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
	}
	if(numOfDotsToPlot2>0)//如果有需要画的点
	{
		if((pGraph->pCurveArray + 1)->numOfPlotDots >=1 )//画 第二个 点  
		{
			SetCtrlVal (hResultDispPanel, SAMPLE_VD_2, *((pGraph->pCurveArray+1)->pDotX-1));
			SetCtrlVal (hResultDispPanel, SAMPLE_VG_2, *((pGraph->pCurveArray+1)->pDotY-1));
			pGraph->plotHandle=PlotXY(graphDispPanel, control, (pGraph->pCurveArray+1)->pDotXPlot-1, (pGraph->pCurveArray+1)->pDotYPlot-1, numOfDotsToPlot2+1, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, smu2Clr);
		}else
		{
			SetCtrlVal (hResultDispPanel, SAMPLE_VD_2, *((pGraph->pCurveArray+1)->pDotX-1));
			SetCtrlVal (hResultDispPanel, SAMPLE_VG_2, *((pGraph->pCurveArray+1)->pDotY-1));
			pGraph->plotHandle=PlotXY(graphDispPanel, control, (pGraph->pCurveArray+1)->pDotXPlot, (pGraph->pCurveArray+1)->pDotYPlot, numOfDotsToPlot2, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, smu2Clr);
		}
		(pGraph->pCurveArray + 1)->numOfPlotDots+=numOfDotsToPlot2;		//画图总点数递增
		(pGraph->pCurveArray + 1)->pDotXPlot+=numOfDotsToPlot2;			//画图点X坐标指针递增
		(pGraph->pCurveArray + 1)->pDotYPlot+=numOfDotsToPlot2;			//画图点Y坐标指针递增
		(pGraph->pCurveArray + 1)->numOfDotsToPlot-=numOfDotsToPlot2;		//防止中断端在画图期间接收到新的数据.
	}
	if(pGraph->plotHandle<0)
		return -1;
	else
		return 0;
}
//temp,humidity,pressure; 
int PlotCurve_Temp(Graph_TypeDef* pGraph_Temp, int graphDispPanel, int control)
{
	int numOfDotsToPlot=pGraph_Temp->pCurveArray->numOfDotsToPlot;						//防止中断端去写入这个数据
	if(pGraph_Temp->pCurveArray->numOfPlotDots >=1 )	//画 第二个 点
	{
			if(numOfDotsToPlot>0)																
			{
				if(temp_flag == 1)  
				{
					pGraph_Temp->plotHandle=PlotXY(graphDispPanel, control, (pGraph_Temp->pCurveArray +1)->pDotXPlot-1, (pGraph_Temp->pCurveArray +1)->pDotYPlot-1, 
												   numOfDotsToPlot+1, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2tempclr);
				}
				if(humidity_flag == 1)
				{
					pGraph_Temp->plotHandle=PlotXY(graphDispPanel, control, pGraph_Temp->pCurveArray->pDotXPlot-1, pGraph_Temp->pCurveArray->pDotYPlot-1, 
												   numOfDotsToPlot+1, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2humclr);
				}
				if(pressure_flag == 1)
				{
					pGraph_Temp->plotHandle=PlotXY(graphDispPanel, control, (pGraph_Temp->pCurveArray +2)->pDotXPlot-1, (pGraph_Temp->pCurveArray +2)->pDotYPlot-1, 
												   numOfDotsToPlot+1, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2preclr);
				}
				pGraph_Temp->pCurveArray->numOfPlotDots+=numOfDotsToPlot;				//画图总点数递增
				pGraph_Temp->pCurveArray->pDotXPlot+=numOfDotsToPlot;					//画图点X坐标指针递增
				pGraph_Temp->pCurveArray->pDotYPlot+=numOfDotsToPlot;					//画图点Y坐标指针递增
				pGraph_Temp->pCurveArray->numOfDotsToPlot-=numOfDotsToPlot;				//防止中断端在画图期间接收到新的数据.
				
				(pGraph_Temp->pCurveArray + 1)->numOfPlotDots+=numOfDotsToPlot;			//画图总点数递增
				(pGraph_Temp->pCurveArray + 1)->pDotXPlot+=numOfDotsToPlot;				//画图点X坐标指针递增
				(pGraph_Temp->pCurveArray + 1)->pDotYPlot+=numOfDotsToPlot;				//画图点Y坐标指针递增
				(pGraph_Temp->pCurveArray + 1)->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
				
				(pGraph_Temp->pCurveArray + 2)->numOfPlotDots+=numOfDotsToPlot;			//画图总点数递增
				(pGraph_Temp->pCurveArray + 2)->pDotXPlot+=numOfDotsToPlot;				//画图点X坐标指针递增
				(pGraph_Temp->pCurveArray + 2)->pDotYPlot+=numOfDotsToPlot;				//画图点Y坐标指针递增
				(pGraph_Temp->pCurveArray + 2)->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
			}
    }
	else //画第一个点
	{
			if(numOfDotsToPlot>0)																//如果有需要画图的点
			{	
				if(temp_flag == 1)
				{
					pGraph_Temp->plotHandle=PlotXY(graphDispPanel, control, (pGraph_Temp->pCurveArray +1)->pDotXPlot, (pGraph_Temp->pCurveArray +1)->pDotYPlot, 
												   numOfDotsToPlot, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2tempclr);
				}
				
				if(humidity_flag == 1) 
				{
					pGraph_Temp->plotHandle=PlotXY(graphDispPanel, control, pGraph_Temp->pCurveArray->pDotXPlot, pGraph_Temp->pCurveArray->pDotYPlot, 
												   numOfDotsToPlot, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2humclr);
				}
				if(pressure_flag == 1)
				{
					pGraph_Temp->plotHandle=PlotXY(graphDispPanel, control, (pGraph_Temp->pCurveArray +2)->pDotXPlot, (pGraph_Temp->pCurveArray +2)->pDotYPlot, 
												   numOfDotsToPlot, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2preclr);
				}
				pGraph_Temp->pCurveArray->numOfPlotDots+=numOfDotsToPlot;		//画图总点数递增
				pGraph_Temp->pCurveArray->pDotXPlot+=numOfDotsToPlot;			//画图点X坐标指针递增
				pGraph_Temp->pCurveArray->pDotYPlot+=numOfDotsToPlot;			//画图点Y坐标指针递增
				pGraph_Temp->pCurveArray->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
				
				(pGraph_Temp->pCurveArray + 1)->numOfPlotDots+=numOfDotsToPlot;		//画图总点数递增
				(pGraph_Temp->pCurveArray + 1)->pDotXPlot+=numOfDotsToPlot;			//画图点X坐标指针递增
				(pGraph_Temp->pCurveArray + 1)->pDotYPlot+=numOfDotsToPlot;			//画图点Y坐标指针递增
				(pGraph_Temp->pCurveArray + 1)->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
				
				(pGraph_Temp->pCurveArray + 2)->numOfPlotDots+=numOfDotsToPlot;		//画图总点数递增
				(pGraph_Temp->pCurveArray + 2)->pDotXPlot+=numOfDotsToPlot;			//画图点X坐标指针递增
				(pGraph_Temp->pCurveArray + 2)->pDotYPlot+=numOfDotsToPlot;			//画图点Y坐标指针递增
				(pGraph_Temp->pCurveArray + 2)->numOfDotsToPlot-=numOfDotsToPlot;		//防止中断端在画图期间接收到新的数据.
			}
	}
	if(pGraph_Temp->plotHandle<0)
		return -1;
	else
		return 0;
}
void temp_hum_pre_display()
{

	DeleteGraphPlot (graphDispPanel, GRAPHDISP_GRAPH2, -1, VAL_IMMEDIATE_DRAW);	//先清除环境测量曲线图中的所有曲线
	GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX,&temp_flag);		//然后读取用户要选中的曲线
	GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_2,&humidity_flag); 
	GetCtrlVal(hEnvResultPanel,ENVIRPANEL_CHECKBOX_3,&pressure_flag);

	if((temp_flag == 1)&&((Graph_Temp.pCurveArray  + 1)->numOfPlotDots>0))
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);    //text背景色(与canvas颜色相同)
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);		 //"Temperature"颜色
		CanvasDrawText (graphDispPanel, GRAPHDISP_CANVAS, "T",VAL_APP_META_FONT, MakeRect(40,15,15,70), VAL_CENTER);	                            //写"Temperature"
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, graph2tempclr);			    //Temperature图例
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_WIDTH, 2);			        //线的宽度
		CanvasDrawLine (graphDispPanel, GRAPHDISP_CANVAS, MakePoint (110,47), MakePoint (150, 47));		//canvas画线
		PlotXY(graphDispPanel, GRAPHDISP_GRAPH2, (Graph_Temp.pCurveArray  + 1)->pDotXHead, (Graph_Temp.pCurveArray  + 1)->pDotYHead, 
			   (Graph_Temp.pCurveArray  + 1)->numOfPlotDots, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2tempclr);
	}
	else
	{
	  CanvasClear (graphDispPanel, GRAPHDISP_CANVAS, MakeRect(40,15,15,140));
	}
	if((humidity_flag == 1)&&(Graph_Temp.pCurveArray->numOfPlotDots>0))
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);    //text背景色(与canvas颜色相同)
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);		 //"humidity"颜色
		CanvasDrawText (graphDispPanel, GRAPHDISP_CANVAS, "H",VAL_APP_META_FONT, MakeRect(60,15,15,70), VAL_CENTER);	                            //写"humidity"
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, graph2humclr);			    //humidity图例
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_WIDTH, 2);			        //线的宽度
		CanvasDrawLine (graphDispPanel, GRAPHDISP_CANVAS, MakePoint (110, 67), MakePoint (150, 67));		//canvas画线
		PlotXY(graphDispPanel, GRAPHDISP_GRAPH2, Graph_Temp.pCurveArray->pDotXHead, Graph_Temp.pCurveArray->pDotYHead, 
			   Graph_Temp.pCurveArray->numOfPlotDots, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2humclr);
	}
	else
	{
	  CanvasClear (graphDispPanel, GRAPHDISP_CANVAS, MakeRect(60,15,15,140));
	}
	
	if((pressure_flag == 1)&&((Graph_Temp.pCurveArray  + 2)->numOfPlotDots>0))
	{
		SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);    //text背景色(与canvas颜色相同)
	    SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);		 //"pressure"颜色
	    CanvasDrawText (graphDispPanel, GRAPHDISP_CANVAS, "P",VAL_APP_META_FONT, MakeRect(80,15,15,70), VAL_CENTER);	                            //写"pressure"
	    SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_COLOR, graph2preclr);			    //pressure图例
	    SetCtrlAttribute (graphDispPanel, GRAPHDISP_CANVAS, ATTR_PEN_WIDTH, 2);			        //线的宽度
	    CanvasDrawLine (graphDispPanel, GRAPHDISP_CANVAS, MakePoint (110, 87), MakePoint (150, 87));		//canvas画线
		PlotXY(graphDispPanel, GRAPHDISP_GRAPH2, (Graph_Temp.pCurveArray  + 2)->pDotXHead, (Graph_Temp.pCurveArray  + 2)->pDotYHead, 
			   (Graph_Temp.pCurveArray  + 2)->numOfPlotDots, VAL_FLOAT, VAL_FLOAT, VAL_CONNECTED_POINTS, VAL_DOTTED_SOLID_SQUARE, VAL_SOLID, 1, graph2preclr);
	}
	else
	{
	 CanvasClear (graphDispPanel, GRAPHDISP_CANVAS, MakeRect(80,15,15,140)); 
	}
	
}


