#include <ansi_c.h>
#include <utility.h>
#include <userint.h>
#include "LoadPanel.h"

//==============================================================================
//
// Title:		File.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/10 at 18:02:54 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "File.h"
//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?


void InitFileLable(FileLableTypeDef* pFileLable, char* pFileName)
{
	//int fileHandle;
	int month, day, year;
	int hours, minutes, seconds;
	pFileLable->pFileName=pFileLable->FileName;
	pFileLable->pFileDate=pFileLable->FileDate;
	pFileLable->pFileTime=pFileLable->FileTime;
	pFileLable->pFileDesc=pFileLable->FileDesc;
	strcpy (pFileLable->pFileName, pFileName);
	//pFileLable->pFileName=pFileName;
	GetFileDate (pFileName, &month, &day, &year);
	sprintf(pFileLable->pFileDate, "%02d/%02d/%4d", month, day, year);
	GetFileTime (pFileName, &hours, &minutes, &seconds);
	sprintf(pFileLable->pFileTime, "%02d:%02d:%02d", hours, minutes, seconds);
	FILE * fp = NULL;							//表示打开的文件
	fp= fopen (pFileName, "a+");
	fseek(fp, -64, SEEK_END);					//从最后起向前64
	fgets(pFileLable->pFileDesc, 63, fp);
	fclose(fp);

	
}
