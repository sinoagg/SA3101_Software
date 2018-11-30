//==============================================================================
//
// Title:		Serial Command.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/7/8 at 8:21:34 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <rs232.h>
#include "SerialCommand.h"

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
void CommandRun(int comSelect, char* UartTxBuf)
{
	char UartTxBuf[16];
	UartTxBuf[0]=0x11;
	ComWrt(comSelect, UartTxBuf, 17);		//Send Command
}

void CommandStop(int comSelect, char* UartTxBuf)
{
   	char UartTxBuf[16];
	UartTxBuf[0]=0x12;
	ComWrt(comSelect, UartTxBuf, 17);		//Send Command
}

void CommandConfig(int comSelect, char* UartTxBuf)
{
	char UartTxBuf[16];
	UartTxBuf[0]=0x13;
	ComWrt(comSelect, UartTxBuf, 17);		//Send Command
}
void CommandCalibrate(int comSelect, char* UartTxBuf)
{
	char UartTxBuf[16];
	UartTxBuf[0]=0x13;
	ComWrt(comSelect, UartTxBuf, 17, char* UartTxBuf);		//Send Command
}
