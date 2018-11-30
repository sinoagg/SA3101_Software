//==============================================================================
//
// Title:		table.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/7/20 at 10:36:04 by 斌 何.
// Copyright:	内蒙古民族大学. All Rights Reserved.
//
//==============================================================================

#ifndef __table_H__
#define __table_H__

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
	int column;									    //列数
	int row;										//行数
	int column_width;								//列宽  
}Table_TypeDef;
extern char ABC1[11][20];
		
//==============================================================================
// External variables

//==============================================================================
// Global functions

void Table_init(char table_title_IV[][20], int column, int column_width,int row);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __table_H__ */
