//==============================================================================
//
// Title:		IdVdsPanel.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/10 at 18:10:32 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __IdVdsPanel_H__
#define __IdVdsPanel_H__

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
}IdVdCfg_TypeDef;
//==============================================================================
// External variables
//extern IdVdCfg_TypeDef IdVdCfg;
//==============================================================================
// Global functions

//int GetIdVdCfg (int panelHandle); 

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __IdVdsPanel_H__ */
