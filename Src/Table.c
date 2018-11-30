//#include "Table.h"
#include <userint.h>
#include "SaveData.h"
#include "LoadPanel.h"

//==============================================================================
//
// Title:		Table.c
// Purpose:		A short description of the implementation.
//
// Created on:	2018/7/19 at 22:18:03 by 斌 何.
// Copyright:	内蒙古民族大学. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
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
char ccc[1][2] ={"A","B"};

int CVICALLBACK COMMANDBUTTON_Back (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			for(i=1;i<6;i++)
			
//			InsertTableColumns(tablePanel,TABLE_TABLE1,1,5,VAL_CELL_STRING);
			
			
			//SetTableColumnAttribute(tablePanel,TABLE_TABLE,1,ATTR_COLUMN_WIDTH,200);
			//SetTableColumnAttribute(tablePanel,TABLE_TABLE,2,ATTR_COLUMN_WIDTH,200);
			//SetTableColumnAttribute(tablePanel,TABLE_TABLE,3,ATTR_COLUMN_WIDTH,200);
			//SetTableColumnAttribute(tablePanel,TABLE_TABLE,4,ATTR_COLUMN_WIDTH,200);
			//SetTableColumnAttribute(tablePanel,TABLE_TABLE,5,ATTR_COLUMN_WIDTH,200);
			
			//for(i=1;i<6;i++)
			//	SetTableColumnAttribute(tablePanel,TABLE_TABLE,i,ATTR_USE_LABEL_TEXT,1);
			//
			//
			//SetTableColumnAttribute(tablePanel, TABLE_TABLE, 1, ATTR_LABEL_TEXT,  ccc[0][0]);
			//SetTableColumnAttribute(tablePanel, TABLE_TABLE, 2, ATTR_LABEL_TEXT, "222");
			//SetTableColumnAttribute(tablePanel, TABLE_TABLE, 3, ATTR_LABEL_TEXT, "333");
			//SetTableColumnAttribute(tablePanel, TABLE_TABLE, 4, ATTR_LABEL_TEXT, "444");
			//SetTableColumnAttribute(tablePanel, TABLE_TABLE, 5, ATTR_LABEL_TEXT, "555");
			
		//SetTableColumnAttribute(tablePanel,TABLE_TABLE,-1,1,1);
		//	
		//SetTableColumnAttribute(tablePanel, TABLE_TABLE, 1, ATTR_LABEL_TEXT, "测试结果");	
			

			break;
	}
	return 0;
}
