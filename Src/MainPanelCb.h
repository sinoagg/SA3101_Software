//==============================================================================
//
// Title:		MainPanel.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/16 at 15:58:31 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __MainPanelCB_H__
#define __MainPanelCB_H__											

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include "ExpListPanel.h"
#include "SampleCfgPanel.h"
#include "IdVdsPanel.h"
#include "IdVgsPanel.h"
#include "Protocol.h"
#include "Graph.h"
#include "Curve.h"
#include "File.h"
//==============================================================================
// Constants

//==============================================================================
// Types
typedef struct
{
	int hSinglePrjPanel;				//加载后每个项目的panel句柄
	int index;							//自定义的目录，决定了位置和显示
}PrjHandleTypeDef;
//==============================================================================
// External variables
extern FileLableTypeDef *pFileLable[];
extern PrjHandleTypeDef SingleProject[];
extern int tempVal,humidityVal,pressureVal; 
extern int measureval;   
//extern int logFlag;
//==============================================================================
// Global functions
static int SaveConfigToFile(char* pConfigSavePath);
void StopKeyAction(unsigned char select_Addr1,unsigned char select_Addr2);																			//停止按钮按下后产生的一系列动作  
void Dispgraph();																		//不同模式显示不同的单位 				   
int CVICALLBACK AbnmDCThreadFunction (void *functionData);		 //声明多线程回调函数 

#ifdef __cplusplus
    }
#endif						 

#endif  /* ndef __MainPanel_H__ */
