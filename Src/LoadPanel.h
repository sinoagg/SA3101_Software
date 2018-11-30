//==============================================================================
//
// Title:		LoadPanel.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/16 at 15:21:14 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __LoadPanel_H__
#define __LoadPanel_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include "MainPanel.h"
#include "Experiment List.h"
#include "Id-Vds Configuration.h"
#include "Id-Vgs Configuration.h"
#include "TerminalPanel.h" 
#include "I-T Configuration.h"
#include "I-V Configuration.h"
#include "V-I Configuration.h"
#include "V-T Configuration.h"
#include "R-T Configuration.h"
#include "Sample Configuration.h"
#include "GraphDisp.h"
#include "EnvironmentDisp.h"
#include "Result Menu.h"
#include "TableData.h"
#include "Sample Analyze.h"
#include "SetPanel.h"
#include "ProjectPanel.h"
#include "I_T_Panel.h"
#include "Tools.h"
//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables
extern int mainPanel;
extern int expListPanel;
extern int TwoTerminalPanel;
extern int IdVdPanel;
extern int IdVgPanel;
extern int IVPanel;
extern int VIPanel;
extern int ITPanel;
extern int RTPanel;
extern int ThreeTerminalPanel; 
extern int FourTerminalPanel;
extern int hBasicSamplePanel;
extern int hResultDispPanel;
extern int graphDispPanel;
extern int hEnvCfgPanel;
extern int resultPanel;
extern int tablePanel;
extern int saveDataPanel;
extern int RemindsaveDataPanel;
extern int setPanel;
extern int settingsPanel; 
extern int ENVTPanel;
extern int hSettingsGraphPanel;
extern int aboutPanel;
extern int iTPanel;          //I-T panel
extern int rTPanel;          //R-T panel
extern int proPanel;
extern int defPanel; 
extern int chPanel;          //chose panel
extern int hEnvResultPanel;
extern int hAdvanceSamplePanel; 
extern int projectPanel;
extern int hToolsPanel;
extern int hCalibrationPanel;
extern int checkportPanel;
extern int readportPanel;
//==============================================================================
// Global functions

int LoadInitPanel (void);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __LoadPanel_H__ */
