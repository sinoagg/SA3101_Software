//==============================================================================
//
// Title:		IdVgsPanel.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/11 at 13:36:34 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __IdVgsPanel_H__
#define __IdVgsPanel_H__

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
	int cfgVdstart;									//voltage drain start
	int cfgVdstop;									//voltage drain stop
	int cfgVdstep;									//voltage drain step
	int cfgVgstart;									//voltage gate start
	int cfgVgstop;									//voltage gate stop
	int cfgVgstep;									//voltage gate step
}IdVgCfg_TypeDef;
//==============================================================================
// External variables
extern IdVgCfg_TypeDef IdVgCfg;
//==============================================================================
// Global functions

int GetIdVgCfg (int panelHandle); 

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __IdVgsPanel_H__ */
