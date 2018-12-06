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
	int hSinglePrjPanel;				//���غ�ÿ����Ŀ��panel���
	int index;							//�Զ����Ŀ¼��������λ�ú���ʾ
}PrjHandleTypeDef;
//==============================================================================
// External variables
extern FileLableTypeDef *pFileLable[];
extern PrjHandleTypeDef SingleProject[];
extern int tempVal,humidityVal,pressureVal; 
extern int measureval;
extern int stop_Flag;
extern int logFlag;
extern int threadFlag;
//==============================================================================
// Global functions
static int SaveConfigToFile(char* pConfigSavePath);
void StopKeyAction(unsigned char select_Addr1,unsigned char select_Addr2);						//ֹͣ��ť���º������һϵ�ж���  
void Dispgraph();																				//��ͬģʽ��ʾ��ͬ�ĵ�λ 				   
#ifdef __cplusplus
    }
#endif						 

#endif  /* ndef __MainPanel_H__ */
