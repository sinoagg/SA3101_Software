//==============================================================================
//
// Title:		ResultMenuPanel.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/23 at 12:04:35 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __ResultMenuPanel_H__
#define __ResultMenuPanel_H__

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

//==============================================================================
// External variables

//==============================================================================
// Global functions

static int SaveGraph(int panel, int control, int plotHandle, const char path[]);
void DispResultTableGraph(void); 
extern void DispSingleGraph(void);
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __ResultMenuPanel_H__ */
