//==============================================================================
//
// Title:		Source File.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/8 at 8:21:34 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __SourceFile_H__
#define __SourceFile_H__

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

void CommandRun(int comSelect);
void CommandStop(int comSelect);
void CommandConfig(int comSelect);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __SourceFile_H__ */
