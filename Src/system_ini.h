//==============================================================================
//
// Title:		system_ini.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/7/27 at 11:44:46 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __system_ini_H__
#define __system_ini_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
 //Include files
#include "inifile.h"
#include "cvidef.h"
//==============================================================================
// Constants

//==============================================================================
// Types
		
//==============================================================================
// External variables

//==============================================================================
// Global functions
int SaveSystemClose(int temp);
int CheckSystemClose();
int InitSystemClose(int temp); 
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __system_ini_H__ */
