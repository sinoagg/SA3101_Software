//==============================================================================
//
// Title:		AbnormalDataCache.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/8/28 at 10:40:52 by Ma Hongwei.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __AbnormalDataCache_H__
#define __AbnormalDataCache_H__

#ifdef __cplusplus
    extern "C" {
#endif
//==============================================================================
// Include files
#include "excelreport.h"
#include <utility.h> 
#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions
int WriteAndSaveExcel(int panelHandle, int controlID);
int LaunchExcelCBShow();
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __AbnormalDataCache_H__ */
