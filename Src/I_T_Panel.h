//==============================================================================
//
// Title:		I_T_Panel.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/7/14 at 10:17:00 by 斌 何.
// Copyright:	内蒙古民族大学. All Rights Reserved.
//
//==============================================================================

#ifndef __I_T_Panel_H__
#define __I_T_Panel_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include "Protocol.h"

//==============================================================================
// Constants

//==============================================================================
// Types
typedef struct
{
	int cfgIT_Voltage_start;					//voltage  start
	int cfgIT_Voltage_stop;						//voltage  stop
	int cfgIT_Voltage_step;						//voltage  step
	
	int cfgIT_Current_start;					//current  start
	int cfgIT_Current_stop;						//current  stop
	int cfgIT_Current_step;						//current  step
	
	//int cfgIT_Voltage_start2;						//voltage_2  start
	//int cfgIT_Voltage_stop2;						//voltage_2  stop
	//int cfgIT_Voltage_step2;						//voltage_2  step
	//
	//int cfgIT_Current_start2;						//current_2  start
	//int cfgIT_Current_stop2;						//current_2  stop
	//int cfgIT_Current_step2;						//current_2  step
	
}ITCfg_TypeDef;
//==============================================================================
// External variables
//extern ITCfg_TypeDef ITCfg1;
//extern ITCfg_TypeDef ITCfg2;

extern ExpPanelTypeDef I_T_Panel1;
extern ExpPanelTypeDef I_T_Panel2;

extern ExpPanelTypeDef I_V_Panel1;
extern ExpPanelTypeDef I_V_Panel2;

extern ExpPanelTypeDef V_I_Panel1;
extern ExpPanelTypeDef V_I_Panel2;

extern ExpPanelTypeDef R_T_Panel1;
extern ExpPanelTypeDef R_T_Panel2;

extern ExpPanelTypeDef V_T_Panel1;
extern ExpPanelTypeDef V_T_Panel2;


//==============================================================================
// Global functions
int Get_IT_Cfg (int panelHandle);  
void GetTestPara(ExpPanelTypeDef* pExpPanel, TestParaTypeDef* pTestPara); //得到面板中用户设置的参数
void GetTestPara_T(ExpPanelTypeDef* pExpPanel, TestParaTypeDef* pTestPara); //得到面板中用户设置的参数  
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __I_T_Panel_H__ */
