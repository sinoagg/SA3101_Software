//==============================================================================
//
// Title:		File.h
// Purpose:		A short description of the interface.
//
// Created on:	18/7/10 at 18:02:54 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __File_H__
#define __File_H__

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
	char FileName[512];
	char FileDate[16];
	char FileTime[16];
	char FileDesc[64];
	char *pFileName;
	char *pFileDate;
	char *pFileTime;
	char *pFileDesc;
}FileLableTypeDef;


//==============================================================================
// External variables

//==============================================================================
// Global functions
void InitFileLable(FileLableTypeDef* pFileLable, char* pFileName);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __File_H__ */
