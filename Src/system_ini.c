//==============================================================================
//
// Title:		system_ini.c
// Purpose:		A short description of the implementation.
//
// Created on:	2018/7/27 at 11:44:46 by Ð¡Âí¸ç.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include "system_ini.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
static IniText myInifile;
static char pathName[500];
static char dirName[500];
//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

int SaveSystemClose(int temp)
{
	GetProjectDir(dirName);
	MakePathname (dirName, "systemClose.ini", pathName);
	if (!(myInifile = Ini_New (0)))						/* Create a new Inifile object and read it from a file */ 
	{
		MessagePopup("Inifile","Error allocating memory for Inifile");
		return -1;
	}
	Ini_PutInt (myInifile,"System","systemClose",temp);	
	
	Ini_WriteToFile(myInifile,pathName);
	Ini_Dispose(myInifile);
	myInifile = 0;
	return 0;
}

int CheckSystemClose()
{
	int temp;
	GetProjectDir(dirName);
	MakePathname (dirName, "systemClose.ini", pathName); 
	if (!(myInifile = Ini_New (0)))						/* Create a new Inifile object and read it from a file */ 
	{
		MessagePopup("Inifile","Error allocating memory for Inifile");
		return -1;						   
	}
	if ((Ini_ReadFromFile (myInifile, pathName)) < 0)
	{
		MessagePopup("Inifile","Error reading Inifile");
		return -1;
	}

	Ini_GetInt (myInifile, "System", "systemClose", &temp);
	if(temp != 0)
	{
	 temp = ConfirmPopup ("Hint","The software abnormally closed in the last time, whether to view the saved data");
	}
	return temp;	 
}

int InitSystemClose(int temp)
{
 	GetProjectDir(dirName);
	MakePathname (dirName, "systemClose.ini", pathName);
	if (!(myInifile = Ini_New (0)))					/* Create a new Inifile object and read it from a file */ 
	{
		MessagePopup("Inifile","Error allocating memory for Inifile");
		return -1;
	}
	Ini_PutInt (myInifile,"System","systemClose",temp);	
	Ini_WriteToFile(myInifile,pathName);
	Ini_Dispose(myInifile);
	myInifile = 0;
	return 0;	
}


