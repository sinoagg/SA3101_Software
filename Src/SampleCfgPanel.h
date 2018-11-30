//==============================================================================
//
// Title:		SampleCfgPanel.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/10 at 20:38:31 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __SampleCfgPanel_H__
#define __SampleCfgPanel_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"


//==============================================================================
// Constants

//==============================================================================
// Types
typedef struct
{
	int cfgQuiteTime;								//quiet time usually 2 seconds    
	int cfgTimeInterval;							//data interval
	int cfgRunTime;									//run time
	int cfgSampleRate;								//samples per second usually 2000
	int cfgSampleNum;								//average number usually 400
	int cfgRangeSetting;							//range settings
}SampleCfg_TypeDef;
//==============================================================================
// External variables

extern SampleCfg_TypeDef SampleCfg; 

//==============================================================================
// Global functions

int GetSampleCfg (int panelHandle); 

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __SampleCfgPanel_H__ */
