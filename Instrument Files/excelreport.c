/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 2000-2017.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       ExcelReport.c                                                 */
/* Purpose:     Source file for CVI Excel Report Generation Instrument Driver */
/* Notes: 		This library communicates with the Microsoft Excel Automation */
/*				Server via a generated ActiveX controller instrument driver   */
/*				(Excel2000.fp).  This generated driver must be present to use */
/*				this library.												  */
/*                                                                            */
/*============================================================================*/

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
#include <cvirte.h>
#include <cviauto.h>
#include <utility.h>

#include "ExcelReport.h"
#include "Excel2000.h"
#include "toolbox.h"

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//	Global Data
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
static long gThreadLocalErrorInfo;

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//	Utility functions
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
static HRESULT CheckForEmbeddedChart(CAObjHandle chartHandle, int *isEmbedded, CAObjHandle *chartObjHandle);
static long BGRtoRGB(long bgr);
static long RGBtoBGR(long rgb);
static void AtExitHandler(void);

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//  Exported ExcelReport functions
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
//	Function	: ExcelRpt_GetErrorInfo
//
//	Purpose     : Returns the ERRORINFO* for the current thread.
//
//	Return      : ActiveX error info.
//------------------------------------------------------------------------------------------------------------------
ERRORINFO * CVIFUNC ExcelRpt_GetErrorInfo(void)
{
	ERRORINFO * result = NULL;
	
	if (gThreadLocalErrorInfo)
		CmtGetThreadLocalVar(gThreadLocalErrorInfo, &result);
	
	return result;
}

//------------------------------------------------------------------------------------------------------------------
//	Function	: ExcelRpt_ApplicationNew
//
//	Purpose     : Starts a new application in Excel and returns a CVI handle to this new application.
//                The input parameter makeVisible indicates wheather the application is to be visible or not.
//
//	Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ApplicationNew(int makeVisible, CAObjHandle *applicationHandle)
{
	HRESULT __result = S_OK;
    CAObjHandle appHandleL = 0;

    if(!applicationHandle)
        __caErrChk (E_INVALIDARG);
	
	if (InterlockedCompareExchange(&gThreadLocalErrorInfo, 0, 0) == 0)
	{
		CmtTLVHandle threadLocal;
	 	if (CmtNewThreadLocalVar(sizeof(ERRORINFO), NULL, NULL, NULL, &threadLocal) < 0)
			return E_UNEXPECTED;
		if (InterlockedCompareExchange(&gThreadLocalErrorInfo, threadLocal, 0) != 0) 
			// Got initialized by another thread
			CmtDiscardThreadLocalVar(threadLocal);
		else
			atexit(AtExitHandler);
	}

    __caErrChk (Excel_NewApp (NULL, 1, LOCALE_NEUTRAL, 0, &appHandleL));

    __caErrChk (Excel_SetProperty (appHandleL, ExcelRpt_GetErrorInfo(), Excel_AppVisible, CAVT_BOOL, makeVisible ? VTRUE : VFALSE));

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(appHandleL);
    else
        *applicationHandle = appHandleL;
	return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function	: ExcelRpt_ApplicationQuit
//
//  Purpose     : Quits the Excel application
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ApplicationQuit(CAObjHandle applicationHandle)
{
   HRESULT __result = S_OK;

   __caErrChk (Excel_AppQuit (applicationHandle, ExcelRpt_GetErrorInfo()));

Error:
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_RunMacro
//
//  Purpose     : Runs the macro named by the variable macroName
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_RunMacro(CAObjHandle applicationHandle, const char *macroName)
{
    HRESULT __result = S_OK;
    VARIANT MacroNameV;

    CA_VariantSetEmpty (&MacroNameV);

    SetWaitCursor(1);

    __caErrChk (CA_VariantSetCString (&MacroNameV, macroName));

    __caErrChk (Excel_AppRun (applicationHandle, ExcelRpt_GetErrorInfo(), MacroNameV, CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
		CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
    	CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
		CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
		CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
		CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, NULL));

Error:
    CA_VariantClear(&MacroNameV);
    SetWaitCursor(0);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_SetApplicationAttribute
//
//  Purpose     : Sets the application attributes
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC_C ExcelRpt_SetApplicationAttribute(CAObjHandle applicationHandle, int attribute, ...)
{
    HRESULT __result = S_OK;
    int  boolVal;
    double doubleVal;
    enum ExREnum_ExWindowState windowState;
    va_list pargs;

    va_start (pargs, attribute);

    switch(attribute)
    {
        case ER_AP_ATTR_VISIBLE:
            boolVal = va_arg (pargs, int);
            __caErrChk (Excel_SetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppVisible, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_AP_ATTR_WINDOW_STATE:
            windowState = va_arg (pargs, enum ExREnum_ExWindowState);
            __caErrChk (Excel_SetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppWindowState, 
				CAVT_LONG, windowState));
            break;
        case ER_AP_ATTR_TOP:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_SetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppTop, 
				CAVT_DOUBLE, doubleVal));
            break;
        case ER_AP_ATTR_LEFT:
            doubleVal = va_arg (pargs, double);
            __caErrChk (Excel_SetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppLeft, 
				CAVT_DOUBLE, doubleVal));
            break;
        case ER_AP_ATTR_WIDTH:
            doubleVal = va_arg (pargs, double);
            __caErrChk (Excel_SetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppWidth, 
				CAVT_DOUBLE, doubleVal));
            break;
        case ER_AP_ATTR_HEIGHT:
            doubleVal = va_arg (pargs, double);
            __caErrChk (Excel_SetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppHeight, 
				CAVT_DOUBLE, doubleVal));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

Error:
    va_end(pargs);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_GetApplicationAttribute
//
//  Purpose     : Gets the application attributes
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetApplicationAttribute(CAObjHandle applicationHandle, int attribute, void *attributeValue)
{
    HRESULT __result = S_OK;
    VBOOL boolVal;

    if(!attributeValue)
        __caErrChk (E_INVALIDARG);

    switch (attribute)
	{
        case ER_AP_ATTR_VISIBLE:
            __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppVisible, 
				CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_AP_ATTR_WINDOW_STATE:
            __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppWindowState, 
				CAVT_LONG, attributeValue));
            break;
        case ER_AP_ATTR_TOP:
            __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppTop, 
				CAVT_DOUBLE, attributeValue));
            break;
        case ER_AP_ATTR_LEFT:
            __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppLeft, 
				CAVT_DOUBLE, attributeValue));
            break;
        case ER_AP_ATTR_WIDTH:
            __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppWidth, 
				CAVT_DOUBLE, attributeValue));
            break;
        case ER_AP_ATTR_HEIGHT:
            __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppHeight, 
				CAVT_DOUBLE, attributeValue));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

Error:
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_WorkbookNew
//
//  Purpose     : Creates a new workbook in the application
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WorkbookNew(CAObjHandle applicationHandle, CAObjHandle *workbookHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle  ExcelWorkbooksHandle = 0;
    CAObjHandle workbookHandleL = 0;

    if(!workbookHandle)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppWorkbooks, CAVT_OBJHANDLE,
    							   &ExcelWorkbooksHandle));

    __caErrChk (Excel_WorkbooksAdd (ExcelWorkbooksHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, &workbookHandleL));

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(workbookHandleL);
    else
        *workbookHandle = workbookHandleL;
    CA_DiscardObjHandle(ExcelWorkbooksHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_WorkbookOpen
//
//  Purpose     : Opens a workbook
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WorkbookOpen(CAObjHandle applicationHandle, const char *fileName,
									  CAObjHandle *workbookHandle)
{
	HRESULT __result = S_OK;
	CAObjHandle WorkbooksHandle = 0;
	CAObjHandle WorkbookHandleL = 0;

	if(!workbookHandle)
		__caErrChk (E_INVALIDARG);

	__caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppWorkbooks, 
		CAVT_OBJHANDLE, &WorkbooksHandle));

	__caErrChk (Excel_WorkbooksOpen (WorkbooksHandle, ExcelRpt_GetErrorInfo(), fileName, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
									 CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
									 CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
									 CA_DEFAULT_VAL, CA_DEFAULT_VAL, &WorkbookHandleL));

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(WorkbookHandleL);
    else
        *workbookHandle=WorkbookHandleL;
    CA_DiscardObjHandle(WorkbooksHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_WorkbookClose
//
//  Purpose     : Close a workbook
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WorkbookClose(CAObjHandle workbookHandle, int saveChanges)
{
    HRESULT __result = S_OK;

    __caErrChk (Excel_WorkbookClose (workbookHandle, ExcelRpt_GetErrorInfo(), 
		CA_VariantBool ((saveChanges) ? VTRUE : VFALSE), CA_DEFAULT_VAL, CA_VariantBool (VFALSE)));

Error:
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
//  Function    : ExcelRpt_WorkbookSave
//
//  Purpose     : Save a workbook (when filename or format is NULL default values are used)
//
//  Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WorkbookSave(CAObjHandle workbookHandle, const char *fileName,
									  enum ExREnum_ExFileFormat format)
{
    HRESULT  __result = S_OK;
    VARIANT FileNameV;

    CA_VariantSetEmpty (&FileNameV);

    if(fileName != NULL)
    {
        __caErrChk (CA_VariantSetCString (&FileNameV, fileName));

        __caErrChk (Excel_WorkbookSaveAs (workbookHandle, ExcelRpt_GetErrorInfo(), FileNameV,
        								  (format != ExRConst_DefaultFileFormat) ? CA_VariantInt(format) : CA_DEFAULT_VAL,
        								  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        								  ExcelConst_xlNoChange, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        								  CA_DEFAULT_VAL));
    }
    else
    {
        __caErrChk (Excel_WorkbookSave (workbookHandle, NULL));
    }

Error:
    CA_VariantClear(&FileNameV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function    : ExcelRpt_SetMinimumMonitor
//
// 	Purpose     : Sets the minimum monitor intended to be used by clients to view a particular Excel file
//                when saved as a webpage
//
// 	Return		: ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_SetMinimumMonitor(CAObjHandle workbookHandle, long resolution, long pixelsPerInch)
{
    HRESULT __result = S_OK;
    CAObjHandle webOptionsHandleL = 0;

    // Get the WebOptions object for this workbook
    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWebOptions, CAVT_OBJHANDLE,
    							   &webOptionsHandleL));

    __caErrChk (Excel_SetProperty (webOptionsHandleL, ExcelRpt_GetErrorInfo(), Excel_WebOptionsScreenSize,
    							   CAVT_LONG, resolution));

    __caErrChk (Excel_SetProperty (webOptionsHandleL, ExcelRpt_GetErrorInfo(), Excel_WebOptionsPixelsPerInch,
    							   CAVT_LONG, pixelsPerInch));

Error:
	CA_DiscardObjHandle(webOptionsHandleL);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_AllowVMLSupport
//
// 	Purpose     : Sets the document to either use or not use VML (vector markup language) when it is
//                saved as a webpage
//
// 	Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_AllowVMLSupport(CAObjHandle workbookHandle, int value)
{
    HRESULT __result = S_OK;
    CAObjHandle webOptionsHandleL = 0;

    // Get the WebOptions object for this workbook
    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWebOptions, CAVT_OBJHANDLE,
    							   &webOptionsHandleL));

    __caErrChk (Excel_SetProperty (webOptionsHandleL, ExcelRpt_GetErrorInfo(), Excel_WebOptionsRelyOnVML,
    							   CAVT_BOOL, value ? VTRUE : VFALSE));

Error:
	CA_DiscardObjHandle(webOptionsHandleL);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function  	: ExcelRpt_AllowCSSSupport
//
// 	Purpose     : Sets the document to either use or not use CSS (cascading style sheets) when it is
//                saved as a webpage
//
// 	Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_AllowCSSSupport(CAObjHandle workbookHandle, int value)
{
    HRESULT __result = S_OK;
    CAObjHandle webOptionsHandleL = 0;

    // Get the WebOptions object for this workbook
    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWebOptions, CAVT_OBJHANDLE,
    							   &webOptionsHandleL));

    __caErrChk (Excel_SetProperty (webOptionsHandleL, ExcelRpt_GetErrorInfo(), Excel_WebOptionsRelyOnCSS,
    							   CAVT_BOOL, value ? VTRUE : VFALSE));

Error:
	CA_DiscardObjHandle(webOptionsHandleL);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_AllowPNGSupport
//
// 	Purpose     : Sets the document to either use or not use the PNG image format when it is saved as
//                a webpage
//
// 	Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_AllowPNGSupport(CAObjHandle workbookHandle, int value)
{
    HRESULT __result = S_OK;
    CAObjHandle webOptionsHandleL = 0;

    // Get the WebOptions object for this workbook
    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWebOptions, CAVT_OBJHANDLE,
    							   &webOptionsHandleL));

    __caErrChk (Excel_SetProperty (webOptionsHandleL, ExcelRpt_GetErrorInfo(), Excel_WebOptionsAllowPNG,
    							   CAVT_BOOL, value ? VTRUE : VFALSE));

Error:
	CA_DiscardObjHandle(webOptionsHandleL);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_SetEncoding
//
// 	Purpose     : Sets the character set encoding for the document when it is saved as a webpage
//
// 	Return      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_SetEncoding(CAObjHandle workbookHandle, int encoding)
{
    HRESULT __result = S_OK;
    CAObjHandle webOptionsHandleL = 0;
    CAObjHandle defaultWebOptionsHandleL = 0;
    CAObjHandle appHandleL = 0;

    // Get the application object
    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookApplication,
    							   CAVT_OBJHANDLE, &appHandleL));

    // Get the DefaultWebOptions object
    __caErrChk (Excel_GetProperty (appHandleL, ExcelRpt_GetErrorInfo(), Excel_AppDefaultWebOptions,
    							   CAVT_OBJHANDLE, &defaultWebOptionsHandleL));

    // Get the WebOptions object for this workbook
    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWebOptions, CAVT_OBJHANDLE,
    							   &webOptionsHandleL));

    __caErrChk (Excel_SetProperty (defaultWebOptionsHandleL, ExcelRpt_GetErrorInfo(),
    							   Excel_DefaultWebOptionsAlwaysSaveInDefaultEncoding, CAVT_BOOL, VFALSE));

    __caErrChk (Excel_SetProperty (webOptionsHandleL, ExcelRpt_GetErrorInfo(), Excel_WebOptionsEncoding, 
		CAVT_LONG, encoding));

Error:
	CA_DiscardObjHandle(webOptionsHandleL);
	CA_DiscardObjHandle(defaultWebOptionsHandleL);
	CA_DiscardObjHandle(appHandleL);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetWorksheetFromIndex
//
// 	Purpose     : Gets a Worksheet Handle given the Index of the worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetWorksheetFromIndex(CAObjHandle workbookHandle, int Index, CAObjHandle *worksheetHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle sheetsHandle = 0;
    CAObjHandle worksheetHandleL = 0;
    LPUNKNOWN worksheetIfacePtr = 0;
    int didAddref = 0;
    long sheetType;

    if(!worksheetHandle)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookSheets, 
		CAVT_OBJHANDLE, &sheetsHandle));

    __caErrChk (Excel_SheetsItem (sheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(Index), &worksheetHandleL));

    __result = CA_GetInterfaceFromObjHandle (worksheetHandleL, &Excel_IID_Worksheet, 0, &worksheetIfacePtr,
    										 &didAddref);

    if(FAILED(__result))
    {
        if(__result == E_NOINTERFACE)
        {
            __caErrChk (E_INVALIDARG);
        }
        else
        {
            __caErrChk (__result);
        }
    }

    __caErrChk (Excel_GetProperty (worksheetHandleL, ExcelRpt_GetErrorInfo(), Excel_WorksheetType, 
		CAVT_LONG, &sheetType));

    if(sheetType != ExRConst_Worksheet)
    {
        __caErrChk (E_INVALIDARG);
    }

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(worksheetHandleL);
    else
        *worksheetHandle = worksheetHandleL;
    if(didAddref)
        worksheetIfacePtr->lpVtbl->Release(worksheetIfacePtr);
    CA_DiscardObjHandle(sheetsHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetChartsheetFromIndex
//
// 	Purpose     : Gets a Chartsheet Handle given the Index of the worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetChartsheetFromIndex(CAObjHandle workbookHandle, int Index, CAObjHandle *chartsheetHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle sheetsHandle = 0;
    CAObjHandle chartsheetHandleL = 0;
    LPUNKNOWN chartIfacePtr = 0;
    int didAddref = 0;

    if(!chartsheetHandle)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookSheets, 
		CAVT_OBJHANDLE, &sheetsHandle));

    __caErrChk (Excel_SheetsItem (sheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(Index), &chartsheetHandleL));

    __result = CA_GetInterfaceFromObjHandle (chartsheetHandleL, &Excel_IID_Chart, 0, &chartIfacePtr, &didAddref);

    if(FAILED(__result))
    {
        if(__result == E_NOINTERFACE)
        {
            __caErrChk (E_INVALIDARG);
        }
        else
        {
            __caErrChk (__result);
        }
    }

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(chartsheetHandleL);
    else
        *chartsheetHandle = chartsheetHandleL;
    if(didAddref)
        chartIfacePtr->lpVtbl->Release(chartIfacePtr);
    CA_DiscardObjHandle(sheetsHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetChartsheetFromName
//
// 	Purpose     : Gets a Chartsheet Handle given the Name of the Chart sheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetChartsheetFromName(CAObjHandle workbookHandle, const char *chartName,
											   CAObjHandle *chartsheetHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle chartsheetsHandle = 0;
    CAObjHandle chartsheetHandleL = 0;
    char *name_of_chart = NULL;
    int i, total_num_charts;

    if(!chartsheetHandle || chartName == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookCharts, 
		CAVT_OBJHANDLE, &chartsheetsHandle));

    __caErrChk (Excel_GetProperty (chartsheetsHandle, ExcelRpt_GetErrorInfo(), Excel_ChartsCount,CAVT_LONG, 
		&total_num_charts));

    for(i = 1; i <= total_num_charts; i++)
    {
        __caErrChk (Excel_SheetsItem (chartsheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(i), &chartsheetHandleL));

        __caErrChk (Excel_GetProperty (chartsheetHandleL, ExcelRpt_GetErrorInfo(), Excel_ChartName, 
			CAVT_CSTRING, &name_of_chart));

        if(strcmp(name_of_chart, chartName) == 0)
        {
        	// Found a match
            break;
        }
        else
        {
            CA_DiscardObjHandle(chartsheetHandleL);
            chartsheetHandleL = 0;
			CA_FreeMemory(name_of_chart);
			name_of_chart = NULL;
        }
    }

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(chartsheetHandleL);
    else
    	*chartsheetHandle = chartsheetHandleL;
    CA_DiscardObjHandle(chartsheetsHandle);
	CA_FreeMemory(name_of_chart);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetWorksheetFromName
//
// 	Purpose     : Gets a Worksheet Handle given the name of the worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetWorksheetFromName(CAObjHandle workbookHandle, const char *worksheetName,
											  CAObjHandle *worksheetHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle worksheetsHandle = 0;
    CAObjHandle worksheetHandleL = 0;
    char *name_of_sheet = NULL;
    int i, total_num_sheets;

    if(!worksheetHandle || worksheetName == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWorksheets, 
		CAVT_OBJHANDLE, &worksheetsHandle));

    __caErrChk (Excel_GetProperty (worksheetsHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetsCount, 
		CAVT_LONG, &total_num_sheets));

    for(i = 1; i <= total_num_sheets; i++)
    {
        __caErrChk (Excel_SheetsItem (worksheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(i), &worksheetHandleL));

        __caErrChk (Excel_GetProperty (worksheetHandleL, ExcelRpt_GetErrorInfo(), Excel_WorksheetName, 
			CAVT_CSTRING, &name_of_sheet));

        if(strcmp(name_of_sheet, worksheetName) == 0)
        {
            break;
        }
        else
        {
            CA_DiscardObjHandle(worksheetHandleL);
            worksheetHandleL = 0;
            CA_FreeMemory(name_of_sheet);
            name_of_sheet = NULL;
        }
    }

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(worksheetHandleL);
    else
		*worksheetHandle = worksheetHandleL;
	CA_DiscardObjHandle(worksheetsHandle);
	CA_FreeMemory(name_of_sheet);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetSheetTypefromIndex
//
// 	Purpose     : Gets the type of sheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetSheetTypeFromIndex(CAObjHandle workbookHandle, int Index, enum ExREnum_ExSheetType *sheetType)
{
    HRESULT __result = S_OK;
    CAObjHandle sheetsHandle = 0;
    CAObjHandle sheetHandle = 0;
    LPUNKNOWN chartIfacePtr = 0;
    int didAddref = 0;

	if (!sheetType)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookSheets, 
		CAVT_OBJHANDLE, &sheetsHandle));

    __caErrChk (Excel_SheetsItem (sheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(Index), &sheetHandle));

    __result = CA_GetInterfaceFromObjHandle (sheetHandle, &Excel_IID_Chart, 0, &chartIfacePtr, &didAddref);

    if(FAILED(__result))
    {
        if(__result == E_NOINTERFACE)
        {
            __caErrChk (Excel_GetProperty (sheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetType, 
				CAVT_LONG, sheetType));
        }
        else
        {
            __caErrChk (__result);
        }
    }
    else
    {
        *sheetType = ExRConst_Chart;
    }

Error:
    if(didAddref)
        chartIfacePtr->lpVtbl->Release(chartIfacePtr);
    CA_DiscardObjHandle(sheetsHandle);
    CA_DiscardObjHandle(sheetHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetWorkbookAttribute
//
// 	Purpose     : Get Workbook attributes
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetWorkbookAttribute(CAObjHandle workbookHandle, int attribute, void *attributeValue)
{
    HRESULT __result = S_OK;
    CAObjHandle WorksheetsHandle = 0;
    CAObjHandle ChartsHandle = 0;

    if(!attributeValue)
        __caErrChk (E_INVALIDARG);

    switch(attribute)
    {
        case ER_WB_ATTR_NUM_WORKSHEETS:
            __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookWorksheets, 
				CAVT_OBJHANDLE, &WorksheetsHandle));
            __caErrChk (Excel_GetProperty (WorksheetsHandle, ExcelRpt_GetErrorInfo(), 
				Excel_WorksheetsCount, CAVT_LONG, attributeValue));
            break;
        case ER_WB_ATTR_NUM_CHARTS:
            __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookCharts, 
				CAVT_OBJHANDLE, &ChartsHandle));
            __caErrChk (Excel_GetProperty (ChartsHandle, ExcelRpt_GetErrorInfo(), Excel_ChartsCount, 
				CAVT_LONG, attributeValue));
            break;
        case ER_WB_ATTR_NUM_TOTAL_SHEETS:
            __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookSheets, 
				CAVT_OBJHANDLE, &WorksheetsHandle));
            __caErrChk (Excel_GetProperty (WorksheetsHandle, ExcelRpt_GetErrorInfo(), Excel_SheetsCount, 
				CAVT_LONG, attributeValue));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

Error:
    CA_DiscardObjHandle(WorksheetsHandle);
    CA_DiscardObjHandle(ChartsHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_WorksheetNew
//
// 	Purpose     : Adds a new worksheet to a workbook
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WorksheetNew(CAObjHandle workbookHandle, int insertBeforeIndex,
									  CAObjHandle *worksheetHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle ExcelsheetsHandle = 0;
    CAObjHandle InsworksheetHandle = 0;
    CAObjHandle worksheetHandleL = 0;
    VARIANT SheetV;
    LPDISPATCH dispatchPtr = NULL;
    int sheetnum;

    CA_VariantSetEmpty (&SheetV);

    if (!worksheetHandle)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookSheets, 
		CAVT_OBJHANDLE, &ExcelsheetsHandle));

    if(insertBeforeIndex == -1)
    {
        __caErrChk (Excel_GetProperty (ExcelsheetsHandle, ExcelRpt_GetErrorInfo(), Excel_SheetsCount, 
			CAVT_LONG, &sheetnum));
    }
    else
    {
        sheetnum = insertBeforeIndex;
    }

    __caErrChk (Excel_SheetsItem (ExcelsheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(sheetnum), &InsworksheetHandle));

    __caErrChk (CA_GetInterfaceFromObjHandle (InsworksheetHandle, &IID_IDispatch, 1, &dispatchPtr, NULL));

    CA_VariantSetDispatch (&SheetV, dispatchPtr);
    dispatchPtr = NULL;

    __caErrChk (Excel_SheetsAdd (ExcelsheetsHandle, ExcelRpt_GetErrorInfo(), 
		(insertBeforeIndex != -1) ? SheetV : CA_DEFAULT_VAL, (insertBeforeIndex == -1) ? SheetV : CA_DEFAULT_VAL, 
		CA_DEFAULT_VAL, CA_DEFAULT_VAL, &worksheetHandleL));

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(worksheetHandleL);
    else
        *worksheetHandle = worksheetHandleL;
    if (dispatchPtr)
		dispatchPtr->lpVtbl->Release(dispatchPtr);
    CA_DiscardObjHandle(ExcelsheetsHandle);
    CA_DiscardObjHandle(InsworksheetHandle);
    CA_VariantClear(&SheetV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_WorksheetDelete
//
// 	Purpose     : Delete the worksheet from the workbook
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WorksheetDelete (CAObjHandle worksheetHandle)
{
    HRESULT __result = S_OK;

    __caErrChk (Excel_WorksheetDelete (worksheetHandle, ExcelRpt_GetErrorInfo()));

Error:
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_InsertRow
//
// 	Purpose     : Insert Rows
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_InsertRow (CAObjHandle worksheetHandle, int insertBeforeRowIndex, int numRows)
{
    HRESULT __result = S_OK;
    VARIANT ResultV;
    CAObjHandle rangeHandle = 0;
    CAObjHandle rowHandle = 0;
    int i;

    CA_VariantSetEmpty (&ResultV);

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetRows, 
		CAVT_OBJHANDLE, &rangeHandle));

    __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(insertBeforeRowIndex), 
		CA_DEFAULT_VAL, &ResultV));

    __caErrChk (CA_VariantGetObjHandle (&ResultV, &rowHandle));

    for(i = 0; i < numRows; i++)
    {
        __caErrChk (Excel_RangeInsert (rowHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, NULL));
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(rowHandle);
    CA_VariantClear(&ResultV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_InsertColumn
//
// 	Purpose     : Inserts a Column
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_InsertColumn (CAObjHandle worksheetHandle, int insertBeforeColumnIndex, int numColumns)
{
    HRESULT __result = S_OK;
    CAObjHandle columnHandle = 0;
    CAObjHandle rangeHandle = 0;
    VARIANT ColumnV;
    int i;

    CA_VariantSetEmpty (&ColumnV);

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetColumns, 
		CAVT_OBJHANDLE, &rangeHandle));

    __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), 
		CA_VariantInt(insertBeforeColumnIndex), CA_DEFAULT_VAL, &ColumnV));

    __caErrChk (CA_VariantGetObjHandle (&ColumnV, &columnHandle));

    for(i = 0; i < numColumns; i++)
    {
        __caErrChk (Excel_RangeInsert (columnHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, NULL));
    }

Error:
    CA_DiscardObjHandle(columnHandle);
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&ColumnV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_DeleteRow
//
// 	Purpose     : Deletes a Row
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_DeleteRow(CAObjHandle worksheetHandle, int rowIndex, int numRows)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle rowHandle = 0;
    VARIANT RowV;
    int i;

    CA_VariantSetEmpty (&RowV);

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetRows, 
		CAVT_OBJHANDLE, &rangeHandle));

    for(i = 0; i < numRows; i++)
    {
	    __caErrChk (Excel_RangeGetItem (rangeHandle,ExcelRpt_GetErrorInfo(), CA_VariantInt(rowIndex), 
			CA_DEFAULT_VAL, &RowV));
	    __caErrChk (CA_VariantGetObjHandle (&RowV, &rowHandle));
        __caErrChk (Excel_RangeDelete (rowHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, NULL));
        CA_VariantClear(&RowV);
        CA_DiscardObjHandle(rowHandle);
        rowHandle = 0;
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(rowHandle);
    CA_VariantClear(&RowV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_DeleteColumn
//
// 	Purpose     : Deletes a Column
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_DeleteColumn(CAObjHandle worksheetHandle, int columnIndex, int numColumns)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle columnHandle = 0;
    VARIANT ColumnV;
    int i;

    CA_VariantSetEmpty (&ColumnV);

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetColumns, 
		CAVT_OBJHANDLE, &rangeHandle));

    for(i = 0; i < numColumns; i++)
    {
        __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(columnIndex), 
			CA_DEFAULT_VAL, &ColumnV));
        __caErrChk (CA_VariantGetObjHandle (&ColumnV, &columnHandle));
        __caErrChk (Excel_RangeDelete (columnHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, NULL));
        CA_VariantClear(&ColumnV);
        CA_DiscardObjHandle(columnHandle);
        columnHandle = 0;
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(columnHandle);
    CA_VariantClear(&ColumnV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcleRpt_InsertPicture
//
// 	Purpose     : To insert a picture to the worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_InsertPicture(CAObjHandle worksheetHandle, const char *fileName, int top, int left,
									   int width, int height)
{
    HRESULT __result = S_OK;
    CAObjHandle shapesHandle = 0;
    CAObjHandle pictureHandle = 0;

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetShapes, 
		CAVT_OBJHANDLE, &shapesHandle));

    __caErrChk (Excel_ShapesAddPicture (shapesHandle, ExcelRpt_GetErrorInfo(), fileName, ExcelConst_msoFalse, 
		ExcelConst_msoTrue, left, top, width, height, &pictureHandle));

Error:
    CA_DiscardObjHandle(shapesHandle);
    CA_DiscardObjHandle(pictureHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_ActivateWorksheet
//
// 	Purpose     : To activate a worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ActivateWorksheet(CAObjHandle worksheetHandle)
{
    HRESULT __result = S_OK;

    __caErrChk (Excel_WorksheetActivate (worksheetHandle, ExcelRpt_GetErrorInfo()));

Error:
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_Find
//
// 	Purpose     : Finds the cell that holds the search value
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_Find(CAObjHandle worksheetHandle, enum ExREnum_ExDataType dataType, const void *data,
							  const char *afterCell, enum ExREnum_ExFindLookIn lookIn, enum ExREnum_ExLookAt lookAt,
							  enum ExREnum_ExSearchOrder searchOrder, enum ExREnum_ExSearchDirection searchDirection,
							  int matchCase, int matchByte, char foundRange[])
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle worksheetRangeHandle = 0;
    CAObjHandle cellHandle = 0;
    CAObjHandle resultHandle = 0;
    VARIANT CellRangeV;
    VARIANT CellV;
    VARIANT DataV;
    VARIANT LookforV;
    LPDISPATCH dispatchPtr = NULL;
	char *charPtr = NULL;
    int i;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&CellV);
    CA_VariantSetEmpty (&DataV);
    CA_VariantSetEmpty (&LookforV);

    if(!foundRange)
        __caErrChk (E_INVALIDARG);

    if ((dataType != ExRConst_dataEmpty) && (data == NULL))
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetCells, CAVT_OBJHANDLE,
    							   &worksheetRangeHandle));

    __caErrChk (CA_VariantSetCString (&CellRangeV, afterCell));

    __caErrChk (Excel_WorksheetRange(worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeGetItem(rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));

    __caErrChk (CA_VariantGetObjHandle(&CellV, &cellHandle));

    __caErrChk (CA_GetInterfaceFromObjHandle (cellHandle, &IID_IDispatch, 1, &dispatchPtr, NULL));

    CA_VariantSetDispatch (&LookforV, dispatchPtr);
    dispatchPtr = NULL;

    switch(dataType)
    {
        case ExRConst_dataDouble:
            __caErrChk (CA_VariantSetDouble (&DataV, *((double *)data)));
            break;
        case ExRConst_dataString:
            __caErrChk (CA_VariantSetCString (&DataV, (char *)data));
            break;
        case ExRConst_dataCurrency:
            __caErrChk (CA_VariantSetCurrency (&DataV, *((CURRENCY *)data)));
            break;
        case ExRConst_dataDate:
            __caErrChk (CA_VariantSetDate (&DataV, *((DATE *)data)));
            break;
        case ExRConst_dataEmpty:
            __caErrChk (CA_VariantSetEmpty(&DataV));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

    __caErrChk (Excel_RangeFind (worksheetRangeHandle, ExcelRpt_GetErrorInfo(), DataV, LookforV, CA_VariantInt(lookIn),
    							 CA_VariantInt(lookAt), CA_VariantInt(searchOrder),
								 (enum ExcelEnum_XlSearchDirection)searchDirection,
    							 CA_VariantBool(matchCase ? VTRUE : VFALSE),
    							 CA_VariantBool(matchByte ? VTRUE : VFALSE), &resultHandle));

    if (!resultHandle)
        strcpy(foundRange, "");
    else
    {
        __caErrChk (Excel_RangeAddress (resultHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
			ExcelConst_xlA1, CA_DEFAULT_VAL, CA_DEFAULT_VAL, &charPtr));
        i = (int)strlen(charPtr);
        if(i < MAX_FOUND_CELLRANGE_LENGTH)
            strcpy(foundRange, charPtr);
        else
        {
            for(i = 0; i < (MAX_FOUND_CELLRANGE_LENGTH - 1); i++)
            {
                if(charPtr[i] != ',')
                    foundRange[i] = charPtr[i];
                else
                {
                    break;
                }
            }
			foundRange[i] = '\0';
        }
    }

Error:
    CA_DiscardObjHandle(worksheetRangeHandle);
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_DiscardObjHandle(resultHandle);
    if (dispatchPtr)
		dispatchPtr->lpVtbl->Release(dispatchPtr);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&CellV);
    CA_VariantClear(&DataV);
    CA_VariantClear(&LookforV);
	CA_FreeMemory(charPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetEmbeddedChartFromIndex
//
// 	Purpose     : Gets a chartHandle of an embedded chart
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetEmbeddedChartFromIndex(CAObjHandle worksheetHandle, int Index, CAObjHandle *chartHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle chartobjectsHandle = 0;
    CAObjHandle chartobjHandle = 0;
    CAObjHandle chartHandleL = 0;

    if(!chartHandle)
		__caErrChk (E_INVALIDARG);

    __caErrChk (Excel_WorksheetChartObjects (worksheetHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, 
		&chartobjectsHandle));

    __caErrChk (Excel_ChartObjectsItem (chartobjectsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(Index), 
		&chartobjHandle));

    __caErrChk (Excel_GetProperty (chartobjHandle , ExcelRpt_GetErrorInfo(), Excel_ChartObjectChart, 
		CAVT_OBJHANDLE, &chartHandleL));

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(chartHandleL);
    else
        *chartHandle = chartHandleL;
    CA_DiscardObjHandle(chartobjectsHandle);
    CA_DiscardObjHandle(chartobjHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetEmbeddedChartFromName
//
// 	Purpose     : Gets a chartHandle of an embedded chart
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetEmbeddedChartFromName(CAObjHandle worksheetHandle, const char *chartName,
												  CAObjHandle *chartHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle chartobjectsHandle = 0;
    CAObjHandle chartobjHandle = 0;
    CAObjHandle chartHandleL = 0;
    int i, number_of_chartobj;
    char *name_of_chartobj = NULL;

    if(chartHandle == NULL || chartName == NULL)
		__caErrChk (E_INVALIDARG);

    __caErrChk (Excel_WorksheetChartObjects (worksheetHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, 
		&chartobjectsHandle));

    __caErrChk (Excel_GetProperty (chartobjectsHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectsCount, CAVT_LONG,
    							   &number_of_chartobj));

    for(i = 1; i <= number_of_chartobj; i++)
    {
        __caErrChk (Excel_ChartObjectsItem (chartobjectsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(i), 
			&chartobjHandle));
        __caErrChk (Excel_GetProperty (chartobjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectName, 
			CAVT_CSTRING, &name_of_chartobj));
        if(strcmp(name_of_chartobj, chartName) == 0)
        {
            __caErrChk (Excel_GetProperty (chartobjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectChart, 
				CAVT_OBJHANDLE, &chartHandleL));
            break;
        }
        CA_FreeMemory(name_of_chartobj);
        name_of_chartobj = NULL;
        CA_DiscardObjHandle(chartobjHandle);
        chartobjHandle = 0;
    }

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(chartHandleL);
    else
        *chartHandle = chartHandleL;
    CA_FreeMemory(name_of_chartobj);
    CA_DiscardObjHandle(chartobjHandle);
    CA_DiscardObjHandle(chartobjectsHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_SetWorksheetAttribute
//
// 	Purpose     : Set Worksheet attributes
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC_C ExcelRpt_SetWorksheetAttribute(CAObjHandle worksheetHandle, int attribute, ...)
{
    HRESULT __result = S_OK;
    CAObjHandle pagesetupHandle = 0;
    va_list pargs;
    char *charPtr;
    enum ExREnum_ExPageOrientation orientation;

    va_start (pargs, attribute);

    switch(attribute)
	{
	    case ER_WS_ATTR_PRINT_PAGE_ORIENTATION:
	        orientation = va_arg(pargs, enum ExREnum_ExPageOrientation);
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetPageSetup, 
				CAVT_OBJHANDLE, &pagesetupHandle));
	        __caErrChk (Excel_SetProperty (pagesetupHandle, ExcelRpt_GetErrorInfo(), Excel_PageSetupOrientation, 
				CAVT_LONG, orientation));
	        break;
	    case ER_WS_ATTR_NAME:
	        charPtr = va_arg(pargs, char*);
	        __caErrChk (Excel_SetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetName, 
				CAVT_CSTRING, charPtr));
	        break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
	}

Error:
    CA_DiscardObjHandle(pagesetupHandle);
    va_end(pargs);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetWorksheetAttribute
//
// 	Purpose     : Get Worksheet attributes
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetWorksheetAttribute(CAObjHandle worksheetHandle, int attribute, void *attributeValue)
{
    HRESULT __result = S_OK;
    CAObjHandle pagesetupHandle = 0;
    CAObjHandle rangeHandle = 0;
    CAObjHandle chartobjectsHandle = 0;
    char *name = NULL;

    if(!attributeValue)
        __caErrChk (E_INVALIDARG);

    switch(attribute)
	{
	    case ER_WS_ATTR_USED_RANGE:
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetUsedRange, 
				CAVT_OBJHANDLE, &rangeHandle));
	        __caErrChk (Excel_RangeAddress (rangeHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
				ExcelConst_xlA1, CA_DEFAULT_VAL, CA_DEFAULT_VAL, &name));
	        strcpy(attributeValue, name);
	        break;
	    case ER_WS_ATTR_USED_RANGE_LEN:
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetUsedRange, 
				CAVT_OBJHANDLE, &rangeHandle));
	        __caErrChk (Excel_RangeAddress (rangeHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
				ExcelConst_xlA1, CA_DEFAULT_VAL, CA_DEFAULT_VAL, &name));
	        *((int*)attributeValue) = (int)strlen(name);
	        break;
	    case ER_WS_ATTR_PRINT_PAGE_ORIENTATION:
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetPageSetup, 
				CAVT_OBJHANDLE, &pagesetupHandle));
	        __caErrChk (Excel_GetProperty (pagesetupHandle, ExcelRpt_GetErrorInfo(), Excel_PageSetupOrientation, 
				CAVT_LONG, attributeValue));
	        break;
	    case ER_WS_ATTR_NAME:
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetName, 
				CAVT_CSTRING, &name));
	        strcpy(attributeValue, name);
	        break;
	    case ER_WS_ATTR_NAME_LEN:
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetName, 
				CAVT_CSTRING, &name));
	        *((int*)attributeValue) = (int)strlen(name);
	        break;
	    case ER_WS_ATTR_NUM_EMBEDDED_CHARTS:
	        __caErrChk (Excel_WorksheetChartObjects (worksheetHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, 
				&chartobjectsHandle));
	        __caErrChk (Excel_GetProperty (chartobjectsHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectsCount, 
				CAVT_LONG, attributeValue));
	        break;
	    case ER_WS_ATTR_INDEX:
	        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetIndex, 
				CAVT_LONG, attributeValue));
	        break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
	}

Error:
    CA_DiscardObjHandle(pagesetupHandle);
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(chartobjectsHandle);
	CA_FreeMemory(name);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_RangeSelect
//
// 	Purpose     : Selects a range of cells
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_RangeSelect(CAObjHandle worksheetHandle, const char *cellRange)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT ResultV;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&ResultV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeSelect (rangeHandle, ExcelRpt_GetErrorInfo(), &ResultV));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&ResultV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_RangeBorder
//
// 	Purpose     : Set border on the range
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_RangeBorder(CAObjHandle worksheetHandle, const char *cellRange,
									 enum ExREnum_ExLineStyle Linestyle, long color,
									 enum ExREnum_ExBorderWeight weight, unsigned char edges)
{
    HRESULT __result = S_OK;
    CAObjHandle ExcelrangeHandle = 0;
    CAObjHandle ExcelBordersHandle = 0;
    CAObjHandle ExcelBorderHandle = 0;
    long bgrColor;
    VARIANT CellRangeV;

    CA_VariantSetEmpty(&CellRangeV);

    bgrColor = RGBtoBGR(color);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, 
		&ExcelrangeHandle));

    __caErrChk (Excel_GetProperty (ExcelrangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeBorders, CAVT_OBJHANDLE, 
		&ExcelBordersHandle));

    if ((edges & (unsigned char)ExRConst_InsideHorizontal) == ((unsigned char)ExRConst_InsideHorizontal))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlInsideHorizontal, 
			&ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
        							   CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderWeight,
			CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderColor,
			CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges & (unsigned char)ExRConst_InsideVertical) == ((unsigned char)ExRConst_InsideVertical))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlInsideVertical, 
			&ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
        							   CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderWeight,CAVT_VARIANT,
			CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderColor,CAVT_VARIANT,
			CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges & (unsigned char)ExRConst_DiagonalDown) == ((unsigned char)ExRConst_DiagonalDown))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), 
			ExcelConst_xlDiagonalDown, &ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), 
			Excel_BorderLineStyle, CAVT_VARIANT, CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),
			Excel_BorderWeight,CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),
			Excel_BorderColor,CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges & (unsigned char)ExRConst_DiagonalUp) == ((unsigned char)ExRConst_DiagonalUp))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), 
			ExcelConst_xlDiagonalUp, &ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), 
			Excel_BorderLineStyle, CAVT_VARIANT, CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),
			Excel_BorderWeight,CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),
			Excel_BorderColor,CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges  & (unsigned char)ExRConst_EdgeBottom)  == ((unsigned char)ExRConst_EdgeBottom))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), 
			ExcelConst_xlEdgeBottom, &ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), 
			Excel_BorderLineStyle, CAVT_VARIANT, CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),
			Excel_BorderWeight,CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),
			Excel_BorderColor,CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges & (unsigned char)ExRConst_EdgeLeft) == ((unsigned char)ExRConst_EdgeLeft))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlEdgeLeft, &ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
        							   CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderWeight,CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderColor,CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges & (unsigned char)ExRConst_EdgeRight) == ((unsigned char)ExRConst_EdgeRight))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlEdgeRight, &ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
        							   CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderWeight,CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderColor,CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

    if ((edges & (unsigned char)ExRConst_EdgeTop) == ((unsigned char)ExRConst_EdgeTop))
    {
        __caErrChk (Excel_BordersItem (ExcelBordersHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlEdgeTop, &ExcelBorderHandle));
        __caErrChk (Excel_SetProperty (ExcelBorderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
        							   CA_VariantInt(Linestyle)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderWeight,CAVT_VARIANT,CA_VariantInt(weight)));
        __caErrChk (Excel_SetProperty(ExcelBorderHandle,ExcelRpt_GetErrorInfo(),Excel_BorderColor,CAVT_VARIANT,CA_VariantLong(bgrColor)));
        CA_DiscardObjHandle(ExcelBorderHandle);
        ExcelBorderHandle = 0;
    }

Error:
    CA_DiscardObjHandle(ExcelrangeHandle);
    CA_DiscardObjHandle(ExcelBordersHandle);
    CA_DiscardObjHandle(ExcelBorderHandle);
    CA_VariantClear(&CellRangeV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcleRpt_AddHyperlink
//
// 	Purpose     : Add a hyperlink to a cell range
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_AddHyperlink(CAObjHandle worksheetHandle, const char *cellRange, const char *address,
									  const char *screenTip)
{
    HRESULT __result = S_OK;
    CAObjHandle HyperlinksHandle = 0;
    CAObjHandle HyperlinkHandle = 0;
    CAObjHandle ExcelrangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT ScreenTipV;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&ScreenTipV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    if (screenTip != NULL)
        __caErrChk (CA_VariantSetCString (&ScreenTipV, screenTip));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, 
		&ExcelrangeHandle));

    __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetHyperlinks, CAVT_OBJHANDLE,
    							   &HyperlinksHandle));

    __caErrChk (Excel_HyperlinksAdd (HyperlinksHandle, ExcelRpt_GetErrorInfo(), ExcelrangeHandle, address, CA_DEFAULT_VAL,
    								 (screenTip != NULL) ? ScreenTipV : CA_DEFAULT_VAL, CA_DEFAULT_VAL,
    								 &HyperlinkHandle));

Error:
    CA_DiscardObjHandle(HyperlinksHandle);
    CA_DiscardObjHandle(HyperlinkHandle);
    CA_DiscardObjHandle(ExcelrangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&ScreenTipV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_RangePrint
//
// 	Purpose     : Print the Excel range
//
// 	Result      : ActiveX error code
//-----------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_RangePrint(CAObjHandle worksheetHandle, const char *cellRange, int displayDialog)

{
    HRESULT __result = S_OK;
    CAObjHandle DialogsHandle = 0;
    CAObjHandle DialogHandle = 0;
    CAObjHandle applicationHandle = 0;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;

    CA_VariantSetEmpty(&CellRangeV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    if(displayDialog)
    {
        __caErrChk (Excel_GetProperty (worksheetHandle, ExcelRpt_GetErrorInfo(), Excel_WorksheetApplication, CAVT_OBJHANDLE,
        							   &applicationHandle));
        __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppDialogs, 
			CAVT_OBJHANDLE, &DialogsHandle));
        __caErrChk (Excel_DialogsItem (DialogsHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlDialogPrint, &DialogHandle));
        __caErrChk (Excel_RangeSelect (rangeHandle, ExcelRpt_GetErrorInfo(), NULL));
        __caErrChk (Excel_DialogShow (DialogHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_VariantInt(1), CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, NULL));
    }
    else
    {
        __caErrChk (Excel_RangePrintOut (rangeHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, 
			CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, 
			CA_DEFAULT_VAL, NULL));
    }

Error:
    CA_DiscardObjHandle(DialogHandle);
    CA_DiscardObjHandle(DialogsHandle);
    CA_DiscardObjHandle(applicationHandle);
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_Fill
//
// 	Purpose     : Fills the selected range with the value of a particular cell
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_Fill(CAObjHandle worksheetHandle, enum ExREnum_ExFillOptions direction,
							  const char *cellRange)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;

    CA_VariantSetEmpty (&CellRangeV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(),CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    switch(direction)
	{
        case ExRConst_UP:
            __caErrChk (Excel_RangeFillUp (rangeHandle, ExcelRpt_GetErrorInfo(), NULL));
            break;
        case ExRConst_DOWN:
            __caErrChk (Excel_RangeFillDown (rangeHandle, ExcelRpt_GetErrorInfo(), NULL));
            break;
        case ExRConst_LEFT:
            __caErrChk (Excel_RangeFillLeft (rangeHandle, ExcelRpt_GetErrorInfo(), NULL));
            break;
        case ExRConst_RIGHT:
            __caErrChk (Excel_RangeFillRight (rangeHandle, ExcelRpt_GetErrorInfo(), NULL));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
	}

Error:
    CA_DiscardObjHandle(rangeHandle) ;
    CA_VariantClear(&CellRangeV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_Sort
//
// 	Purpose     : Sort a given range
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_Sort(CAObjHandle worksheetHandle, const char *cellRange, const char *key1,
							  enum ExREnum_ExSortOrder sortOrder1, const char *key2,
							  enum ExREnum_ExSortOrder sortOrder2, enum ExREnum_ExSortOrientation sortOrientation,
							  enum ExREnum_ExYesNo header)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle keyHandle1 = 0;
    CAObjHandle keyHandle2 = 0;
    VARIANT CellRangeV;
    VARIANT KeyV1;
    VARIANT ObjkeyV1;
    VARIANT KeyV2;
    VARIANT ObjkeyV2;
    LPDISPATCH dispatchPtr1 = NULL;
    LPDISPATCH dispatchPtr2 = NULL;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&KeyV1);
    CA_VariantSetEmpty (&ObjkeyV1);
    CA_VariantSetEmpty (&KeyV2);
    CA_VariantSetEmpty (&ObjkeyV2);

    __caErrChk (CA_VariantSetCString (&KeyV1, key1));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), KeyV1, CA_DEFAULT_VAL, &keyHandle1));

    __caErrChk (CA_GetInterfaceFromObjHandle (keyHandle1, &IID_IDispatch, 1, &dispatchPtr1, NULL));

    CA_VariantSetDispatch (&ObjkeyV1, dispatchPtr1);
    dispatchPtr1 = NULL;

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    if(key2 != NULL && strlen(key2) != 0)
    {
        __caErrChk (CA_VariantSetCString (&KeyV2, key2));

        __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), KeyV2, CA_DEFAULT_VAL, &keyHandle2));

        __caErrChk (CA_GetInterfaceFromObjHandle (keyHandle2, &IID_IDispatch, 1, &dispatchPtr2, NULL));

        CA_VariantSetDispatch (&ObjkeyV2, dispatchPtr2);
    	dispatchPtr2 = NULL;
     }

    __caErrChk (Excel_RangeSort (rangeHandle, ExcelRpt_GetErrorInfo(), ObjkeyV1,
								 (enum ExcelEnum_XlSortOrder)sortOrder1,
    							 (key2 != NULL && strlen(key2) != 0) ? ObjkeyV2 : CA_DEFAULT_VAL, CA_DEFAULT_VAL,
    							 (enum ExcelEnum_XlSortOrder)sortOrder2,
								 CA_DEFAULT_VAL, ExcelConst_xlAscending,
								 (enum ExcelEnum_XlYesNoGuess)header, CA_DEFAULT_VAL,
    							 CA_DEFAULT_VAL,
								 (enum ExcelEnum_XlSortOrientation)sortOrientation, ExcelConst_xlPinYin, NULL));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(keyHandle1);
    CA_DiscardObjHandle(keyHandle2);
    if (dispatchPtr1)
		dispatchPtr1->lpVtbl->Release(dispatchPtr1);
    if (dispatchPtr2)
		dispatchPtr2->lpVtbl->Release(dispatchPtr2);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&KeyV1);
    CA_VariantClear(&ObjkeyV1);
    CA_VariantClear(&KeyV2);
    CA_VariantClear(&ObjkeyV2);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_RangeInsert
//
// 	Purpose     : Insert a given range to a worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_RangeInsert(CAObjHandle worksheetHandle, const char *cellRange,
									 enum ExREnum_ExInsertShiftDirection shiftDirection)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT ResultV;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&ResultV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeInsert (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(shiftDirection), &ResultV));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&ResultV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_RangeDelete
//
// 	Purpose     : Insert a given range to a worksheet
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_RangeDelete(CAObjHandle worksheetHandle, const char *cellRange,
									 enum ExREnum_ExDeleteShiftDirection shiftDirection)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT ResultV;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&ResultV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeDelete (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(shiftDirection), &ResultV));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&ResultV);
	return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_Show
//
// 	Purpose     : Show the selected range and center the left uppermost cell in the screen
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_Show(CAObjHandle worksheetHandle, const char *cellRange)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT ReturnV;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&ReturnV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeShow (rangeHandle, ExcelRpt_GetErrorInfo(), &ReturnV));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&ReturnV);
	return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_WriteData
//
// Purpose      : Writes data to an Excel range
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WriteData(CAObjHandle worksheetHandle, const char *cellRange,
								   enum ExREnum_ExDataType dataType, size_t dim1, size_t dim2, const void *data)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT *Varray = NULL;
    VARIANT CellRangeV;
    VARIANT ArrayV;
    LPSAFEARRAY safeArray = NULL;
    int i, j;
    int bNeedToClearVariantsInArray = TRUE;

	CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&ArrayV);

    if(data == NULL || dim1 == 0 || dim2 == 0)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    Varray = (VARIANT *) CA_AllocMemory (dim1 * dim2 * sizeof(VARIANT));

    if (!Varray)
        __caErrChk (E_OUTOFMEMORY);

    for(i=0;i < dim1 /* Rows */;i++)
    {
        for(j=0; j < dim2 /* Columns */; j++)
        {
		    if(dataType == ExRConst_dataDouble)
		    {
            	__caErrChk (CA_VariantSetDouble (&Varray[i*dim2+j], ((double*)data)[i*dim2+j]));
            }
		    else if(dataType == ExRConst_dataString)
		    {
				__caErrChk (CA_VariantSetCString (&Varray[i*dim2+j], ((char**)data)[i*dim2+j]));
		    }
		    else if(dataType == ExRConst_dataCurrency)
		    {
				__caErrChk (CA_VariantSetCurrency (&Varray[i*dim2+j], ((CURRENCY*)data)[i*dim2+j]));
		    }
		    else if(dataType == ExRConst_dataDate)
		    {
				__caErrChk (CA_VariantSetDate (&Varray[i*dim2+j], ((DATE*)data)[i*dim2+j]));
		    }
		    else
		    {
		        __caErrChk (E_INVALIDARG);
		    }
        }
    }

    __caErrChk (CA_Array2DToSafeArray (Varray, CAVT_VARIANT, dim1, dim2, &safeArray));
    bNeedToClearVariantsInArray = FALSE;

    __caErrChk (CA_VariantSetSafeArray (&ArrayV, CAVT_VARIANT, safeArray));
    safeArray = NULL;

    __caErrChk (Excel_SetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, ArrayV));

Error:
    CA_DiscardObjHandle (rangeHandle);
    if (Varray)
    {
        if(bNeedToClearVariantsInArray)
		    for(i = 0; i < dim1 /* Rows */; i++)
		        for(j = 0; j < dim2 /* Columns */; j++)
                	CA_VariantClear (&Varray[i*dim2+j]);
        CA_FreeMemory (Varray);
    }
    CA_SafeArrayDestroy (safeArray);
    CA_VariantClear (&ArrayV);
    CA_VariantClear (&CellRangeV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_ReadData
//
// 	Purpose     : Reads data from excel
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ReadData(CAObjHandle worksheetHandle, const char *cellRange,
								  enum ExREnum_ExDataType dataType, void *data)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT *Varray = NULL;
    VARIANT *ValueVPtr;
    VARIANT CellRangeV;
    VARIANT RangeV;
	size_t i, j;
    size_t dim1 = 0, dim2 = 0;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&RangeV);

    if(data == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &RangeV));

    __caErrChk (CA_VariantGet2DArray (&RangeV, CAVT_VARIANT, &Varray, &dim1, &dim2));

    memset (data, 0, dim1*dim2);

    for (i = 0; i < dim1; i++)
    {
        for (j = 0; j < dim2; j++)
        {
            ValueVPtr = &(Varray[i*dim2+j]);

		    if(dataType == ExRConst_dataDouble)
		    {
				__caErrChk (CA_VariantConvertToType (ValueVPtr, dataType, &(((double*)data)[i*dim2+j])));
		    }
		    else if (dataType == ExRConst_dataString)
		    {
				__caErrChk (CA_VariantConvertToType (ValueVPtr, dataType, &(((char**)data)[i*dim2+j])));
		    }
		    else if (dataType == ExRConst_dataCurrency)
		    {
				__caErrChk (CA_VariantConvertToType (ValueVPtr, dataType, &(((CURRENCY*)data)[i*dim2+j])));
		    }
		    else if (dataType == ExRConst_dataDate)
		    {
				__caErrChk (CA_VariantConvertToType (ValueVPtr, dataType, &(((DATE*)data)[i*dim2+j])));
		    }
		    else
		    {
		        __caErrChk (E_INVALIDARG);
		    }
        }
    }

Error:
    if (Varray)
    {
		if (FAILED(__result))
			for (i = 0; i < dim1; i++)
		        for (j = 0; j < dim2; j++)
	            	CA_VariantClear (&Varray[i*dim2+j]);
    	CA_FreeMemory (Varray);
    }
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&RangeV);
    if (FAILED(__result) && dataType == ExRConst_dataString)
    	for (i = 0; i < dim1 * dim2; i++)
    		CA_FreeMemory(((char **)data)[i]);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_FreeStringData
//
// 	Purpose     : Use this function to free strings returned by ExcelRpt_ReadData
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_FreeStringData(char **data, size_t count)
{
    HRESULT __result = S_OK;
    int i;

    if(data!=NULL)
    {
        for (i = 0; i < count; i++)
			CA_FreeMemory (data[i]);
    }

    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetNumCellsInRange
//
// 	Purpose     : Gets the number of cells in the range
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetNumCellsInRange(CAObjHandle worksheetHandle, const char *cellRange, int *numCells)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT RangeV;
    size_t dim1, dim2;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&RangeV);

    if(numCells == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &RangeV));

    __caErrChk (CA_VariantGet2DArraySize (&RangeV, &dim1, &dim2));

    *numCells = (int)(dim1 * dim2);

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&RangeV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetRangeDataType
//
// 	Purpose     : Gets the data type of the range
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetRangeDataType(CAObjHandle worksheetHandle, const char *cellRange,
										  enum ExREnum_ExDataType *dataType)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT *Varray = NULL;
    VARIANT CellRangeV;
    VARIANT RangeV;
    enum ExREnum_ExDataType dataTypeL = ExRConst_dataNotInitialized;
    size_t i, j;
	size_t dim1 = 0, dim2 = 0;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&RangeV);

    if(dataType == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &RangeV));

    __caErrChk (CA_VariantGet2DArray (&RangeV, CAVT_VARIANT, &Varray, &dim1, &dim2));

    for (i=0; i < dim1; i++)
    {
        for (j=0; j < dim2; j++)
        {
           	if(CA_VariantIsEmpty (&Varray[i*dim2+j]))
            {
                dataTypeL = ExRConst_dataEmpty;
                j = dim2;
                i = dim1;
            }
            else
            {
                if(dataTypeL == ExRConst_dataNotInitialized)
                    dataTypeL = CA_VariantGetType (&Varray[i*dim2+j]);
                else
                {
	                if( (dataTypeL == ExRConst_dataCurrency) &&
	                    (CA_VariantHasDouble (&Varray[i*dim2+j]) || CA_VariantHasDate (&Varray[i*dim2+j])))
	                    dataTypeL = ExRConst_dataDouble;

	                if( (dataTypeL == ExRConst_dataDate) &&
	                    (CA_VariantHasDouble (&Varray[i*dim2+j]) || CA_VariantHasCurrency (&Varray[i*dim2+j])))
	                    dataTypeL = ExRConst_dataDouble;

	                if ((dataTypeL == ExRConst_dataCurrency || dataTypeL == ExRConst_dataDate ||
	                    dataTypeL == ExRConst_dataDouble) && (CA_VariantHasCString (&Varray[i*dim2+j])))
	                    dataTypeL = ExRConst_dataString;
                }
            }
        }
    }

Error:
    if(SUCCEEDED(__result))
        *dataType = dataTypeL;
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&RangeV);
    if (Varray)
    {
	    for (i=0; i < dim1; i++)
	        for (j=0; j < dim2; j++)
				CA_VariantClear (&Varray[i*dim2+j]);
		CA_FreeMemory (Varray);
    }
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_WriteDataFromTableControl
//
// 	Purpose     : Write data from CVI table to Excel range
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_WriteDataFromTableControl(CAObjHandle worksheetHandle, const char *cellRange, int panel,
												   int control)
{
    HRESULT __result = S_OK;
    int error = 0;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT SetValueV;
    VARIANT RangeV;
    Point cell;
    int Rows, Columns;
    int i, j;
	size_t dim1, dim2;
    int cellType, dataType;
    char charVal;
    unsigned char ucharVal;
    int intVal;
    unsigned uintVal;
    short int shortintVal;
    unsigned short int ushortintVal;
    long long int64Val;
    unsigned long long uint64Val;
    float floatVal;
    double doubleVal;
    char *charPtr = NULL;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&SetValueV);
    CA_VariantSetEmpty (&RangeV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &RangeV));

    if (CA_VariantHasArray (&RangeV))
    {
        __caErrChk (CA_VariantGet2DArraySize (&RangeV, &dim1, &dim2));
    }
    else
    {
        dim2 = 1;
        dim1 = 1;
    }

    errChk (GetNumTableRows (panel, control, &Rows));

    errChk (GetNumTableColumns (panel, control, &Columns));

    if(dim1<Rows)
        Rows = (int)dim1;

    if(dim2<Columns)
        Columns = (int)dim2;

    for(i=0; i<Rows ;i++)
    {
        for(j=0; j<Columns ;j++)
        {
        	cell = MakePoint(j+1, i+1);
            errChk (GetTableCellAttribute (panel, control, cell, ATTR_CELL_TYPE, &cellType));

            if (cellType == VAL_CELL_NUMERIC)
            {
                errChk (GetTableCellAttribute (panel, control, cell, ATTR_DATA_TYPE, &dataType));

               	switch(dataType)
                {
                    case VAL_CHAR:
                        errChk (GetTableCellVal (panel, control, cell, &charVal));
                        __caErrChk (CA_VariantSetChar (&SetValueV, charVal));
                        break;
                    case VAL_UNSIGNED_CHAR:
                        errChk (GetTableCellVal (panel, control, cell, &ucharVal));
                        __caErrChk (CA_VariantSetUChar (&SetValueV, ucharVal));
                        break;
                    case VAL_INTEGER:
#ifndef _WIN64
					case VAL_INTPTR_T:
#endif
                        errChk (GetTableCellVal (panel, control, cell, &intVal));
                        __caErrChk (CA_VariantSetInt (&SetValueV, intVal));
                        break;
                    case VAL_UNSIGNED_INTEGER:
#ifndef _WIN64
					case VAL_UINTPTR_T:
#endif
                        errChk (GetTableCellVal (panel, control, cell, &uintVal));
                        __caErrChk (CA_VariantSetUInt (&SetValueV, uintVal));
                        break;
                    case VAL_64BIT_INTEGER:
#ifdef _WIN64
					case VAL_INTPTR_T:
#endif
                       errChk (GetTableCellVal (panel, control, cell, &int64Val));
                        __caErrChk (CA_VariantSetLongLong (&SetValueV, int64Val));
                        break;
                    case VAL_UNSIGNED_64BIT_INTEGER:
#ifdef _WIN64
					case VAL_UINTPTR_T:
#endif
                        errChk (GetTableCellVal (panel, control, cell, &uint64Val));
                        __caErrChk (CA_VariantSetULongLong (&SetValueV, uint64Val));
                        break;
                    case VAL_SHORT_INTEGER:
                        errChk (GetTableCellVal (panel, control, cell, &shortintVal));
                        __caErrChk (CA_VariantSetShort (&SetValueV, shortintVal));
                        break;
                    case VAL_UNSIGNED_SHORT_INTEGER:
                        errChk (GetTableCellVal (panel, control, cell, &ushortintVal));
                        __caErrChk (CA_VariantSetUShort (&SetValueV, ushortintVal));
                        break;
                    case VAL_FLOAT:
                        errChk (GetTableCellVal (panel, control, cell, &floatVal));
                        __caErrChk (CA_VariantSetFloat (&SetValueV, floatVal));
                        break;
                    case VAL_DOUBLE:
                        errChk (GetTableCellVal (panel, control, cell, &doubleVal));
                        __caErrChk (CA_VariantSetDouble (&SetValueV, doubleVal));
                        break;
			        default:
			        	__caErrChk (E_INVALIDARG);
			        	break;
                }
           	}
           	else if (cellType == VAL_CELL_STRING || cellType ==  VAL_CELL_RING || cellType ==  VAL_CELL_COMBO_BOX || cellType ==  VAL_CELL_BUTTON)
            {
                errChk (GetTableCellValLength (panel, control, cell, &intVal));
                charPtr = (char *) CA_AllocMemory ((intVal+1) * sizeof(char));
                if(!charPtr)
                    __caErrChk (E_OUTOFMEMORY);
                errChk (GetTableCellVal (panel, control, cell, charPtr));
                __caErrChk (CA_VariantSetCString (&SetValueV, charPtr));
                CA_FreeMemory (charPtr);
                charPtr = NULL;
            }
			else if (cellType ==  VAL_CELL_PICTURE)
			{
				continue;
			}
            else
            {
	        	__caErrChk (E_INVALIDARG);
            }

	        __caErrChk (Excel_RangeSetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(i+1), CA_VariantInt(j+1), SetValueV));
	        CA_VariantClear (&SetValueV);
		}
	}

Error:
    if(charPtr)
        CA_FreeMemory (charPtr);
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&RangeV);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&SetValueV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_ReadDataToTableControl
//
// 	Purpose		: Read data from Excel range to CVI table
//
// 	Result		: ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ReadDataToTableControl(CAObjHandle worksheetHandle, const char *cellRange,
												Point startpoint, enum ExREnum_ExInsertMode insertmode, int panel,
												int control)
{
    HRESULT __result = S_OK;
    int error = 0;
    CAObjHandle rangeHandle = 0;
    VARIANT *Varray = NULL;
    VARIANT CellRangeV;
    VARIANT RangeV;
    Point cell;
    double doubleVal;
    char  *charPtr = NULL;
    int numRows, numColumns;
	size_t dim1 = 0, dim2 = 0;
    int type , startRowIndex, startColIndex;
    int row, column;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&RangeV);

    startRowIndex = startpoint.y;
    startColIndex = startpoint.x;

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &RangeV));

    if (CA_VariantHasArray (&RangeV))
    {
       __caErrChk (CA_VariantGet2DArray (&RangeV, CAVT_VARIANT, &Varray, &dim1, &dim2));
    }
    else
    {
        Varray = (VARIANT *) CA_AllocMemory(1*sizeof(VARIANT));
        if(!Varray)
            __caErrChk (E_OUTOFMEMORY);
        __caErrChk (CA_VariantCopy (&RangeV, &Varray[0]));
        dim2 = 1;
        dim1 = 1;
    }

    errChk (GetNumTableRows (panel, control, &numRows));
    errChk (GetNumTableColumns (panel, control, &numColumns));

    if((startRowIndex > numRows && numRows != 0) || (startColIndex > numColumns && numColumns != 0) ||
       startRowIndex < -1 || startColIndex < -1)
        __caErrChk (E_INVALIDARG);

    if(numRows == 0 && numColumns == 0)
    {
        startRowIndex = 1;
        startColIndex = 1;
    }

    if(startRowIndex < 0)
        startRowIndex = numRows+1;

    if(startColIndex < 0)
        startColIndex = numColumns+1;

    if (insertmode == ExRConst_Overwrite)
    {
    // do not add any rows or columns
    }
    else if(insertmode == ExRConst_OverwriteGrow)
    {
        if(startRowIndex+dim1>numRows && dim1-numRows+startRowIndex-1>0)
        {
            errChk (InsertTableRows (panel, control, numRows+1, (int)dim1-numRows+startRowIndex-1,
            						 VAL_USE_MASTER_CELL_TYPE));
            numRows = startRowIndex+(int)dim1;
        }
        if(startColIndex+dim2>numColumns && dim2-numColumns+startColIndex-1 >0)
        {
            errChk (InsertTableColumns (panel, control, numColumns+1, (int)dim2-numColumns+startColIndex-1,
            							VAL_USE_MASTER_CELL_TYPE));
            numColumns = startColIndex+(int)dim2;
        }
    }
    else if(insertmode == ExRConst_InsertRow)
    {
        errChk (InsertTableRows (panel, control, startRowIndex, (int)dim1, VAL_USE_MASTER_CELL_TYPE));

        if(dim2-numColumns>0)
        {
            errChk (InsertTableColumns (panel, control, numColumns+1, (int)dim2-numColumns+startColIndex-1,
            							VAL_USE_MASTER_CELL_TYPE));
            numColumns = (int)dim2+startColIndex;
        }
        numRows = numRows+(int)dim1;
    }
    else if(insertmode == ExRConst_InsertColumn)
    {
        errChk (InsertTableColumns (panel, control, startColIndex, (int)dim2, VAL_USE_MASTER_CELL_TYPE));

        if(dim1-numRows>0)
        {
            errChk (InsertTableRows (panel, control, numRows+1, (int)dim1-numRows+startRowIndex-1,
            						 VAL_USE_MASTER_CELL_TYPE));
            numRows = (int)dim1+startRowIndex;
        }
        numColumns = numColumns+(int)dim2;

    }
    else if(insertmode == ExRConst_InsertRowAndColumn)
    {
        errChk (InsertTableRows (panel, control, startRowIndex, (int)dim1, VAL_USE_MASTER_CELL_TYPE));

        errChk (InsertTableColumns (panel, control, startColIndex, (int)dim2, VAL_USE_MASTER_CELL_TYPE));

        numColumns = numColumns+(int)dim2;
        numRows = numRows+(int)dim1;
    }
    else
    {
        __caErrChk (E_INVALIDARG);
    }

    for (row = 0; row < dim1; row++)
    {
        for (column = 0; column < dim2; column++)
        {
            if((startRowIndex+row) <= numRows && (startColIndex+column) <= numColumns)
            {
                type = CA_VariantGetType (&Varray[row*dim2+column]);
				cell = MakePoint(startColIndex+column, startRowIndex+row);

                switch(type)
                {
                    case ExRConst_dataDouble:
                        __caErrChk (CA_VariantGetDouble (&Varray[row*dim2+column], &doubleVal));
                        errChk (SetTableCellAttribute (panel, control, cell, ATTR_CELL_TYPE, VAL_CELL_NUMERIC));
                        errChk (SetTableCellVal (panel, control, cell, doubleVal));
                        break;

                    case ExRConst_dataString:
                        __caErrChk (CA_VariantGetCString (&Varray[row*dim2+column], &charPtr));
                        errChk (SetTableCellAttribute (panel, control, cell, ATTR_CELL_TYPE, VAL_CELL_STRING));
                        errChk (SetTableCellVal (panel, control, cell, charPtr));
                        CA_FreeMemory(charPtr);
                        charPtr = NULL;
                        break;

                    case ExRConst_dataCurrency:
                    case ExRConst_dataDate:
			        default: /* Try to convert any other types to string if possible. */
                        __result = CA_VariantConvertToType (&Varray[row*dim2+column], CAVT_CSTRING, &charPtr);
						if (FAILED(__result))
							errChk (E_INVALIDARG);
                        errChk (SetTableCellAttribute (panel, control, cell, ATTR_CELL_TYPE, VAL_CELL_STRING));
                        errChk (SetTableCellVal (panel, control, cell, charPtr));
                        CA_FreeMemory(charPtr);
                        charPtr = NULL;
                        break;
                }
            }
        }
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&RangeV);
    CA_FreeMemory(charPtr);
    if (Varray)
    {
		if (FAILED(__result))
	        for (row=0; row < dim1; row++)
	        	for (column=0; column < dim2; column++)
	            	CA_VariantClear(&Varray[row*dim2+column]);
        CA_FreeMemory (Varray);
    }
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// 	Function	: ExcelRpt_GetCellDataType
//
// 	Purpose     : Gets the data type of the upper left most cell
//
// 	Result      : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetCellDataType(CAObjHandle worksheetHandle, const char *cellRange,
										 enum ExREnum_ExDataType *dataType)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle cellHandle = 0;
    VARIANT CellRangeV;
    VARIANT CellV;

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&CellV);

    if(dataType == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));

    __caErrChk (CA_VariantGetObjHandle (&CellV, &cellHandle));

    __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &CellV));

    if(CA_VariantHasDouble(&CellV))
        *dataType = ExRConst_dataDouble;
	else if(CA_VariantHasBSTR(&CellV))
        *dataType = ExRConst_dataString;
	else if(CA_VariantIsEmpty(&CellV))
        *dataType = ExRConst_dataEmpty;
	else if(CA_VariantHasCurrency(&CellV))
        *dataType = ExRConst_dataCurrency;
	else if(CA_VariantHasDate(&CellV))
        *dataType = ExRConst_dataDate;

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&CellV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function		: ExcelRpt_GetCellValueStringLen
//
// Purpose		: Gets the length of the string in the cell
//
// Result		: ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetCellValueStringLen(CAObjHandle worksheetHandle, const char *cellRange, int *length)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle cellHandle = 0;
    VARIANT CellRangeV;
    VARIANT CellV;
    VARIANT ValueV;
    char *charPtr = NULL;

    CA_VariantSetEmpty(&CellRangeV);
    CA_VariantSetEmpty(&CellV);
    CA_VariantSetEmpty(&ValueV);

    if(length == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));

    __caErrChk (CA_VariantGetObjHandle (&CellV, &cellHandle));

    __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &ValueV));

    if (CA_VariantIsEmpty (&ValueV))
    {
        *length = 0;
    }
    else
    {
        __caErrChk (CA_VariantConvertToType (&ValueV, CAVT_CSTRING, &charPtr));
        *length = (int)strlen(charPtr);
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&CellV);
    CA_VariantClear(&ValueV);
	CA_FreeMemory(charPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_GetCellValue
//
// Purpose      : Gets the value of the upper left most cell in the range
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetCellValue(CAObjHandle worksheetHandle, const char *cellRange,
									  enum ExREnum_ExDataType dataType, void *dataValue)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle cellHandle = 0;
    VARIANT CellRangeV;
    VARIANT CellV;
    VARIANT ValueV;
    char *charPtr = NULL;

    CA_VariantSetEmpty(&CellRangeV);
    CA_VariantSetEmpty(&CellV);
    CA_VariantSetEmpty(&ValueV);

    if(dataValue == NULL)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));

    __caErrChk (CA_VariantGetObjHandle (&CellV, &cellHandle));

    __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, &ValueV));

    if (dataType == ExRConst_dataString)
    {
        __caErrChk (CA_VariantConvertToType (&ValueV, dataType, &charPtr));
        strcpy(dataValue, charPtr);
    }
    else
    {
        __caErrChk (CA_VariantConvertToType (&ValueV, dataType, dataValue));
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&CellV);
    CA_VariantClear(&ValueV);
    CA_FreeMemory(charPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_SetCellValue
//
// Purpose      : Sets the value of the upper left most cell in the range
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC_C ExcelRpt_SetCellValue(CAObjHandle worksheetHandle, const char *cellRange,
									  enum ExREnum_ExDataType dataType, ...)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle cellHandle = 0;
    VARIANT CellRangeV;
    VARIANT CellV;
    VARIANT SetValueV;
    short shortVal;
    int intVal;
    float floatVal;
    double doubleVal;
    char *charPtr;
    CURRENCY currencyVal;
    DATE dateVal;
    va_list pargs;

    CA_VariantSetEmpty(&CellRangeV);
    CA_VariantSetEmpty(&CellV);
    CA_VariantSetEmpty(&SetValueV);

    va_start (pargs, dataType);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));

    __caErrChk (CA_VariantGetObjHandle (&CellV, &cellHandle));

    switch(dataType)
    {
        case ExRConst_dataShort:
            shortVal = va_arg(pargs, short);
            __caErrChk (CA_VariantSetShort (&SetValueV, shortVal));
            break;
        case ExRConst_dataInt:
            intVal = va_arg(pargs, int);
            __caErrChk (CA_VariantSetInt (&SetValueV, intVal));
            break;
        case ExRConst_dataFloat:
            floatVal = va_arg(pargs, float);
            __caErrChk (CA_VariantSetFloat (&SetValueV, floatVal));
            break;
        case ExRConst_dataDouble:
            doubleVal = va_arg(pargs, double);
            __caErrChk (CA_VariantSetDouble (&SetValueV, doubleVal));
            break;
        case ExRConst_dataString:
            charPtr = va_arg(pargs, char*);
            __caErrChk (CA_VariantSetCString (&SetValueV, charPtr));
            break;
        case ExRConst_dataCurrency:
            currencyVal = va_arg(pargs, CURRENCY);
            __caErrChk (CA_VariantSetCurrency (&SetValueV, currencyVal));
            break;
        case ExRConst_dataDate:
            dateVal = va_arg(pargs, DATE);
            __caErrChk (CA_VariantSetDate (&SetValueV, dateVal));
            break;
        case ExRConst_dataEmpty:
            __caErrChk (CA_VariantSetEmpty (&SetValueV));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

	__caErrChk (Excel_SetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeValue, CAVT_VARIANT, SetValueV));

Error:
    va_end(pargs);
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&CellV);
    CA_VariantClear(&SetValueV);
    return __result;
}


//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_SetCellRangeAttribute
//
// Purpose      : Set CellRange attributes
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC_C ExcelRpt_SetCellRangeAttribute(CAObjHandle worksheetHandle, const char *cellRange,
											   int attribute, ...)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle cellHandle = 0;
    CAObjHandle FontHandle = 0;
    CAObjHandle InteriorHandle = 0;
    CAObjHandle CommentHandle = 0;
    VARIANT CellRangeV;
    VARIANT SetValueV;
    VARIANT CellV;
    VARIANT FontNameV;
    va_list pargs;
    int boolVal;
    double doubleVal;
    int intVal;
    long longVal;
    char *charPtr ;
    long bgr;
    enum ExREnum_ExUnderlineStyle underlineStyle;

    va_start (pargs, attribute);

    CA_VariantSetEmpty (&CellRangeV);
    CA_VariantSetEmpty (&CellV);
    CA_VariantSetEmpty (&SetValueV);
    CA_VariantSetEmpty (&FontNameV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeFont, CAVT_OBJHANDLE, &FontHandle));

    switch(attribute)
    {
        case ER_CR_ATTR_COMMENT:
			__caErrChk (Excel_RangeGetItem (rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));
			__caErrChk (CA_VariantGetObjHandle (&CellV, &cellHandle));
            charPtr = va_arg(pargs, char*);
            __caErrChk (CA_VariantSetCString (&SetValueV, charPtr));
            __caErrChk (Excel_RangeAddComment (cellHandle, ExcelRpt_GetErrorInfo(), SetValueV, &CommentHandle));
            break;
        case ER_CR_ATTR_FORMULA:
            charPtr = va_arg(pargs, char*);
            __caErrChk (CA_VariantSetCString (&SetValueV, charPtr));
            __caErrChk (Excel_SetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeFormula, CAVT_VARIANT, SetValueV));
            break;
        case ER_CR_ATTR_NUMBER_FORMAT:
            charPtr = va_arg(pargs, char*);
            __caErrChk (CA_VariantSetCString(&SetValueV, charPtr));
            __caErrChk (Excel_SetProperty (rangeHandle,ExcelRpt_GetErrorInfo(),Excel_RangeNumberFormat,CAVT_VARIANT,SetValueV));
            break;
        case ER_CR_ATTR_HORIZ_ALIGN:
            longVal = va_arg(pargs, long);
            __caErrChk (Excel_SetProperty (rangeHandle,ExcelRpt_GetErrorInfo(),Excel_RangeHorizontalAlignment,CAVT_VARIANT,
            							   CA_VariantLong(longVal)));
            break;
        case ER_CR_ATTR_VERT_ALIGN:
            longVal = va_arg(pargs, long);
            __caErrChk (Excel_SetProperty (rangeHandle,ExcelRpt_GetErrorInfo(),Excel_RangeVerticalAlignment,CAVT_VARIANT,
            							   CA_VariantLong(longVal)));
            break;
        case ER_CR_ATTR_BGCOLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR (longVal);
            __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeInterior, CAVT_OBJHANDLE, &InteriorHandle));
            __caErrChk (Excel_SetProperty (InteriorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT,
            							   CA_VariantLong(bgr)));
            break;
        case ER_CR_ATTR_FONT_BOLD:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontBold, CAVT_VARIANT,
            							   CA_VariantBool(boolVal ? VTRUE : VFALSE)));
            break;
        case ER_CR_ATTR_FONT_COLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR (longVal);
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontColor, CAVT_VARIANT, CA_VariantLong(bgr)));
            break;
        case ER_CR_ATTR_FONT_NAME:
            charPtr = va_arg(pargs, char*);
            __caErrChk (CA_VariantSetCString (&FontNameV, charPtr));
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontName, CAVT_VARIANT, FontNameV));
            break;
        case ER_CR_ATTR_FONT_ITALIC:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontItalic, CAVT_VARIANT,
            							   CA_VariantBool(boolVal ? VTRUE : VFALSE)));
            break;
        case ER_CR_ATTR_FONT_SIZE:
            intVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontSize, CAVT_VARIANT, CA_VariantInt(intVal)));
            break;
        case ER_CR_ATTR_FONT_STRIKETHROUGH:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontStrikethrough, CAVT_VARIANT,
            							   CA_VariantBool(boolVal ? VTRUE : VFALSE)));
            break;
        case ER_CR_ATTR_FONT_UNDERLINE:
            underlineStyle = va_arg(pargs, enum ExREnum_ExUnderlineStyle);
            __caErrChk (Excel_SetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontUnderline, CAVT_VARIANT,
            							   CA_VariantInt(underlineStyle)));
            break;
        case ER_CR_ATTR_WRAP_TEXT:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeWrapText, CAVT_VARIANT,
            							   CA_VariantBool(boolVal ? VTRUE : VFALSE)));
            break;
        case ER_CR_ATTR_ROW_HEIGHT:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_SetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeRowHeight, CAVT_VARIANT,
            							   CA_VariantDouble(doubleVal)));
            break;
        case ER_CR_ATTR_COLUMN_WIDTH:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_SetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeColumnWidth, CAVT_VARIANT,
            							   CA_VariantDouble(doubleVal)));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

Error:
    va_end(pargs);
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_DiscardObjHandle(FontHandle);
    CA_DiscardObjHandle(InteriorHandle);
    CA_DiscardObjHandle(CommentHandle);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&SetValueV);
    CA_VariantClear(&CellV);
    CA_VariantClear(&FontNameV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_GetCellRangeAttribute
//
// Purpose      : Get CellRange attributes
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetCellRangeAttribute(CAObjHandle worksheetHandle,const char *cellRange, int attribute,
											   void *attributeValue)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    CAObjHandle cellHandle = 0;
    CAObjHandle FontHandle = 0;
    CAObjHandle InteriorHandle = 0;
    CAObjHandle CommentHandle = 0;
    VARIANT CellRangeV;
    VARIANT GetValueV;
    VARIANT CellV;
    VARIANT FontNameV;
    VBOOL boolVal;
    char *charPtr = NULL;
    double doubleVal;
	unsigned long len;

    CA_VariantSetEmpty(&CellRangeV);
    CA_VariantSetEmpty(&CellV);
    CA_VariantSetEmpty(&GetValueV);
    CA_VariantSetEmpty(&FontNameV);

    if(!attributeValue)
        __caErrChk (E_INVALIDARG);

    __caErrChk (CA_VariantSetCString(&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange(worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_RangeGetItem(rangeHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(1), CA_DEFAULT_VAL, &CellV));

    __caErrChk (CA_VariantGetObjHandle(&CellV, &cellHandle));

    __caErrChk (Excel_GetProperty(cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeFont, CAVT_OBJHANDLE, &FontHandle));

    switch(attribute)
    {
        case ER_CR_ATTR_TEXT:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeText, CAVT_VARIANT, &GetValueV));
			__caErrChk (CA_VariantGetCStringLen (&GetValueV, &len));
			__caErrChk (CA_VariantGetCStringBuf (&GetValueV, attributeValue, len+1));
            break;
        case ER_CR_ATTR_TEXT_LEN:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeText, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetCStringLen (&GetValueV, attributeValue));
            break;
        case ER_CR_ATTR_COMMENT:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeComment, CAVT_OBJHANDLE, &CommentHandle));
            __caErrChk (Excel_CommentText (CommentHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
            							   &charPtr));
            strcpy(attributeValue, charPtr);
            break;
        case ER_CR_ATTR_COMMENT_LEN:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeComment, CAVT_OBJHANDLE, &CommentHandle));
            __caErrChk (Excel_CommentText (CommentHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL,CA_DEFAULT_VAL,
            							   &charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CR_ATTR_FORMULA:
            __caErrChk (Excel_GetProperty (cellHandle,ExcelRpt_GetErrorInfo(), Excel_RangeFormula, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetCString (&GetValueV, &charPtr));
            strcpy(attributeValue, charPtr);
            break;
        case ER_CR_ATTR_FORMULA_LEN:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeFormula, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetCString (&GetValueV, &charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CR_ATTR_NUMBER_FORMAT:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeNumberFormat, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetCString (&GetValueV, &charPtr));
            strcpy(attributeValue, charPtr);
            break;
        case ER_CR_ATTR_NUMBER_FORMAT_LEN:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeNumberFormat, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetCString (&GetValueV, &charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CR_ATTR_HORIZ_ALIGN:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeHorizontalAlignment, CAVT_VARIANT,
            							   &GetValueV));
            __caErrChk (CA_VariantGetLong (&GetValueV, attributeValue));
            break;
        case ER_CR_ATTR_VERT_ALIGN:
            __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeVerticalAlignment, CAVT_VARIANT,
            							   &GetValueV));
            __caErrChk (CA_VariantGetLong (&GetValueV, attributeValue));
            break;
        case ER_CR_ATTR_BGCOLOR:
            __caErrChk (Excel_GetProperty (rangeHandle, ExcelRpt_GetErrorInfo(), Excel_RangeInterior, CAVT_OBJHANDLE, &InteriorHandle));
            __caErrChk (Excel_GetProperty (InteriorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetDouble (&GetValueV, &doubleVal));
            *((long*)attributeValue) = BGRtoRGB((long)doubleVal);
            break;
        case ER_CR_ATTR_FONT_BOLD:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontBold, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetBool (&GetValueV, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CR_ATTR_FONT_COLOR:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontColor, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetDouble (&GetValueV, &doubleVal));
            *((long*)attributeValue) = BGRtoRGB((long)doubleVal);
            break;
        case ER_CR_ATTR_FONT_NAME:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontName, CAVT_VARIANT, &FontNameV));
            __caErrChk (CA_VariantGetCString (&FontNameV, &charPtr));
            strcpy(attributeValue, charPtr);
            break;
        case ER_CR_ATTR_FONT_NAME_LEN:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontName, CAVT_VARIANT, &FontNameV));
            __caErrChk (CA_VariantGetCString (&FontNameV, &charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CR_ATTR_FONT_ITALIC:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontItalic, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetBool (&GetValueV, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CR_ATTR_FONT_SIZE:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontSize, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetDouble (&GetValueV, &doubleVal));
            *((int*)attributeValue) = (int)doubleVal;
            break;
        case ER_CR_ATTR_FONT_STRIKETHROUGH:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontStrikethrough, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetBool (&GetValueV, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CR_ATTR_FONT_UNDERLINE:
            __caErrChk (Excel_GetProperty (FontHandle, ExcelRpt_GetErrorInfo(), Excel_FontUnderline, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetInt (&GetValueV, attributeValue));
            break;
        case ER_CR_ATTR_WRAP_TEXT:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeWrapText, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetInt (&GetValueV, attributeValue));
            break;
        case ER_CR_ATTR_ROW_HEIGHT:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeRowHeight, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetDouble(&GetValueV, attributeValue));
            break;
        case ER_CR_ATTR_COLUMN_WIDTH:
            __caErrChk (Excel_GetProperty (cellHandle, ExcelRpt_GetErrorInfo(), Excel_RangeColumnWidth, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetDouble(&GetValueV, attributeValue));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_DiscardObjHandle(cellHandle);
    CA_DiscardObjHandle(FontHandle);
    CA_DiscardObjHandle(InteriorHandle);
    CA_DiscardObjHandle(CommentHandle);
    CA_VariantClear(&GetValueV);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&CellV);
    CA_VariantClear(&FontNameV);
    CA_FreeMemory(charPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function		: ExcelRpt_ChartNew
//
// Purpose      : Adds a new chart to a workbook
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartNew(CAObjHandle workbookHandle, int insertBeforeIndex, CAObjHandle *chartHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle sheetsHandle = 0;
    CAObjHandle insWorksheetHandle = 0;
    VARIANT SheetV;
    LPDISPATCH dispatchPtr = NULL;
    int sheetnum;

    CA_VariantSetEmpty (&SheetV);

    if(!chartHandle)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_GetProperty (workbookHandle, ExcelRpt_GetErrorInfo(), Excel_WorkbookSheets, CAVT_OBJHANDLE, &sheetsHandle));

    if(insertBeforeIndex == -1)
    {
        __caErrChk (Excel_GetProperty (sheetsHandle, ExcelRpt_GetErrorInfo(), Excel_SheetsCount, CAVT_LONG, &sheetnum));
    }
    else
    {
        sheetnum = insertBeforeIndex;
    }

    __caErrChk (Excel_SheetsItem (sheetsHandle, ExcelRpt_GetErrorInfo(), CA_VariantInt(sheetnum), &insWorksheetHandle));

    __caErrChk (CA_GetInterfaceFromObjHandle (insWorksheetHandle, &IID_IDispatch, 1, &dispatchPtr, NULL));

    CA_VariantSetDispatch (&SheetV, dispatchPtr);
    dispatchPtr = NULL;

    __caErrChk (Excel_SheetsAdd (sheetsHandle, ExcelRpt_GetErrorInfo(), (insertBeforeIndex != -1) ? SheetV : CA_DEFAULT_VAL,
    							 (insertBeforeIndex == -1) ? SheetV : CA_DEFAULT_VAL, CA_DEFAULT_VAL,
    							 CA_VariantDouble(ExcelConst_xlChart), chartHandle));

	// Excel does not correctly insert a new chart after the last sheet, so we have to move it.
	if(insertBeforeIndex == -1)
		__caErrChk (Excel_ChartMove (*chartHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, SheetV));

Error:
    CA_DiscardObjHandle(sheetsHandle);
    CA_DiscardObjHandle(insWorksheetHandle);
    CA_VariantClear(&SheetV);
    if (dispatchPtr)
		dispatchPtr->lpVtbl->Release(dispatchPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function		: ExcelRpt_ChartAddtoWorksheet
//
// Purpose      : Adds a chart to the worksheet
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartAddtoWorksheet(CAObjHandle worksheetHandle, double left, double top, double width,
											 double height, CAObjHandle *chartHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle chartsHandle = 0;
    CAObjHandle chartObjHandle = 0;
    CAObjHandle chartHandleL = 0;

    if(!chartHandle)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_WorksheetChartObjects (worksheetHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, &chartsHandle));

    __caErrChk (Excel_ChartObjectsAdd (chartsHandle, ExcelRpt_GetErrorInfo(), left, top, width, height, &chartObjHandle));

    __caErrChk (Excel_GetProperty (chartObjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectChart, CAVT_OBJHANDLE, &chartHandleL));

Error:
    if(FAILED(__result))
        CA_DiscardObjHandle(chartHandleL);
    else
        *chartHandle = chartHandleL;
    CA_DiscardObjHandle(chartsHandle);
    CA_DiscardObjHandle(chartObjHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_ChartDelete
//
// Purpose      : Deletes a chart
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartDelete(CAObjHandle chartHandle)
{
	HRESULT __result = S_OK;
	int isEmbedded;
	CAObjHandle chartObjHandle = 0;

	__caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));

	if(isEmbedded)
	{
		__caErrChk (Excel_ChartObjectDelete (chartObjHandle, ExcelRpt_GetErrorInfo(), NULL));
	}
	else
	{
		__caErrChk (Excel_ChartDelete (chartHandle, ExcelRpt_GetErrorInfo()));
	}

Error:
	CA_DiscardObjHandle(chartObjHandle);
	return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_ChartPrint
//
// Purpose      : Print out a chart
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartPrint(CAObjHandle chartHandle, int displayPrintDialog)
{
    HRESULT __result = S_OK;
    CAObjHandle chartObjHandle = 0;
    CAObjHandle DialogsHandle = 0;
    CAObjHandle DialogHandle = 0;
    CAObjHandle applicationHandle = 0;
	int isEmbedded;

    if(displayPrintDialog)
    {
        __caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));

		if(isEmbedded)
		{
            __caErrChk (Excel_GetProperty (chartObjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectApplication, CAVT_OBJHANDLE,
            							   &applicationHandle));
		}
		else
		{
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartApplication, CAVT_OBJHANDLE,
            							   &applicationHandle));
		}

        __caErrChk (Excel_GetProperty (applicationHandle, ExcelRpt_GetErrorInfo(), Excel_AppDialogs, CAVT_OBJHANDLE, &DialogsHandle));

        __caErrChk (Excel_DialogsItem (DialogsHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlDialogPrint, &DialogHandle));

        __caErrChk (Excel_DialogShow (DialogHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_VariantInt(1), CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        							  CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, NULL));
    }
    else
    {
        __caErrChk (Excel_ChartPrintOut (chartHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        								 CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL, CA_DEFAULT_VAL,
        								 CA_DEFAULT_VAL));
    }

Error:
    CA_DiscardObjHandle(chartObjHandle);
    CA_DiscardObjHandle(DialogsHandle);
    CA_DiscardObjHandle(DialogHandle);
    CA_DiscardObjHandle(applicationHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_ChartActivate
//
// Purpose      : Activates a chart
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartActivate(CAObjHandle chartHandle)
{
    HRESULT __result = S_OK;
    CAObjHandle chartObjHandle = 0;
	int isEmbedded;

	__caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));

	if(isEmbedded)
	{
		__caErrChk (Excel_ChartObjectActivate (chartObjHandle, ExcelRpt_GetErrorInfo(), NULL));
	}
	else
	{
		__caErrChk (Excel_ChartActivate (chartHandle, ExcelRpt_GetErrorInfo()));
	}

Error:
    CA_DiscardObjHandle(chartObjHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_ChartSetSourceRange
//
// Purpose      : Sets the range of cells that wills be used as data for a chart
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartSetSourceRange(CAObjHandle chartHandle, CAObjHandle worksheetHandle,
											 const char *cellRange)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;

    CA_VariantSetEmpty(&CellRangeV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, cellRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (Excel_ChartSetSourceData (chartHandle, ExcelRpt_GetErrorInfo(), rangeHandle, CA_DEFAULT_VAL));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&CellRangeV);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_ChartWizard
//
// Purpose      : Use chart wizard to create chart
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_ChartWizard(CAObjHandle chartHandle, CAObjHandle worksheetHandle, const char *sourceRange,
                                     enum ExREnum_ExChartWizardGallery chartGallery, int format, enum ExREnum_ExRowCol plotBy,
                                     int categoryLabels, int seriesLabels, int hasLegend, const char *title,
                                     const char *categoryAxisTitle, const char *valueAxisTitle,
                                     const char *extraAxisTitle)
{
    HRESULT __result = S_OK;
    CAObjHandle rangeHandle = 0;
    VARIANT CellRangeV;
    VARIANT SourceV;
    VARIANT TitleV;
    VARIANT CategoryAxisTitleV;
    VARIANT ValueAxisTitleV;
    VARIANT ExtraAxisTitleV;
    LPDISPATCH dispatchPtr = NULL;

    CA_VariantSetEmpty(&CellRangeV);
    CA_VariantSetEmpty(&SourceV);
    CA_VariantSetEmpty(&TitleV);
    CA_VariantSetEmpty(&CategoryAxisTitleV);
    CA_VariantSetEmpty(&ValueAxisTitleV);
    CA_VariantSetEmpty(&ExtraAxisTitleV);

    __caErrChk (CA_VariantSetCString (&CellRangeV, sourceRange));

    __caErrChk (Excel_WorksheetRange (worksheetHandle, ExcelRpt_GetErrorInfo(), CellRangeV, CA_DEFAULT_VAL, &rangeHandle));

    __caErrChk (CA_GetDispatchFromObjHandle (rangeHandle, &dispatchPtr));

    __caErrChk (CA_GetInterfaceFromObjHandle (rangeHandle, &IID_IDispatch, 1, &dispatchPtr, NULL));

    CA_VariantSetDispatch (&SourceV, dispatchPtr);
    dispatchPtr = NULL;

    if(title != NULL)
        __caErrChk (CA_VariantSetCString (&TitleV, title));

    if(categoryAxisTitle != NULL)
        __caErrChk (CA_VariantSetCString (&CategoryAxisTitleV, categoryAxisTitle));

    if(valueAxisTitle != NULL)
        __caErrChk (CA_VariantSetCString (&ValueAxisTitleV, valueAxisTitle));

    if(extraAxisTitle != NULL)
        __caErrChk (CA_VariantSetCString (&ExtraAxisTitleV, extraAxisTitle));

    __caErrChk (Excel_ChartChartWizard (chartHandle, ExcelRpt_GetErrorInfo(), SourceV,
    								    (chartGallery == ExRConst_DefaultChartType) ? CA_DEFAULT_VAL : CA_VariantInt(chartGallery),
    								    (format == 0) ? CA_DEFAULT_VAL : CA_VariantInt(format),
    								    (plotBy ==  ExRConst_DefaultRowCol) ? CA_DEFAULT_VAL : CA_VariantInt(plotBy),
    								    (categoryLabels == -1) ? CA_DEFAULT_VAL : CA_VariantInt(categoryLabels),
    								    (seriesLabels == -1) ? CA_DEFAULT_VAL : CA_VariantInt(seriesLabels),
    								    CA_VariantBool(hasLegend ? VTRUE : VFALSE),
    								    (title != NULL) ? TitleV : CA_DEFAULT_VAL,
    								    (categoryAxisTitle != NULL) ? CategoryAxisTitleV : CA_DEFAULT_VAL,
    								    (valueAxisTitle != NULL) ? ValueAxisTitleV : CA_DEFAULT_VAL,
    								    (extraAxisTitle != NULL) ? ExtraAxisTitleV : CA_DEFAULT_VAL));

Error:
    CA_DiscardObjHandle(rangeHandle);
    CA_VariantClear(&SourceV);
    CA_VariantClear(&CellRangeV);
    CA_VariantClear(&TitleV);
    CA_VariantClear(&CategoryAxisTitleV);
    CA_VariantClear(&ValueAxisTitleV);
    CA_VariantClear(&ExtraAxisTitleV);
    if (dispatchPtr)
		dispatchPtr->lpVtbl->Release(dispatchPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_SetChartAttribute
//
// Purpose      : Set chart attributes
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC_C ExcelRpt_SetChartAttribute(CAObjHandle chartHandle, int attribute, ...)
{
    HRESULT __result = S_OK;
    CAObjHandle chartTitleHandle = 0;
    CAObjHandle pageSetupHandle = 0;
    CAObjHandle chartAreaHandle = 0;
    CAObjHandle interiorHandle = 0;
    CAObjHandle borderHandle = 0;
    CAObjHandle plotAreaHandle = 0;
    CAObjHandle dataTableHandle = 0;
    CAObjHandle legendHandle = 0;
    CAObjHandle chartObjHandle = 0;
    va_list pargs;
    int boolVal;
	int isEmbedded;
    long longVal;
    long bgr;
    char *charPtr;
    enum ExREnum_ExChartType chartType;
    enum ExREnum_ExDisplayBlanksAs displayBlankAs;
    enum ExREnum_ExRowCol plotby;
    enum ExREnum_ExObjectSize chartsize;
    enum ExREnum_ExPageOrientation orientation;
    enum ExREnum_ExLineStyle lineStyle;
    enum ExREnum_ExBorderWeight weight;
    enum ExREnum_ExLegendPosition position;

    va_start (pargs, attribute);

    switch(attribute)
    {
        case ER_CH_ATTR_CHART_NAME:
            charPtr = va_arg(pargs, char*);
			__caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));
			if(isEmbedded)
			{
				__caErrChk (Excel_SetProperty (chartObjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectName, CAVT_CSTRING, charPtr));
			}
			else
			{
				__caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartName, CAVT_CSTRING, charPtr));
			}
            break;
        case ER_CH_ATTR_CHART_TYPE:
            chartType = va_arg(pargs, enum ExREnum_ExChartType);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartType, CAVT_LONG, chartType));
            break;
        case ER_CH_ATTR_CHART_DEPTH_PERCENT:
            longVal = va_arg(pargs, long);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDepthPercent, CAVT_LONG, longVal));
            break;
        case ER_CH_ATTR_CHART_ELEVATION:
            longVal = va_arg(pargs, long);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartElevation, CAVT_LONG, longVal));
            break;
        case ER_CH_ATTR_CHART_ROTATION:
            longVal = va_arg(pargs, long);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartRotation, CAVT_VARIANT,
            							   CA_VariantLong(longVal)));
            break;
        case ER_CH_ATTR_CHART_DISPLAY_BLANK_AS:
            displayBlankAs = va_arg(pargs, enum ExREnum_ExDisplayBlanksAs);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDisplayBlanksAs, CAVT_LONG, displayBlankAs));
            break;
        case ER_CH_ATTR_CHART_PLOT_BY:
            plotby = va_arg(pargs, enum ExREnum_ExRowCol);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPlotBy, CAVT_LONG, plotby));
            break;
        case ER_CH_ATTR_CHART_PLOT_VISIBLE_ONLY:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPlotVisibleOnly, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_CHART_SIZE_WITH_WINDOW:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartSizeWithWindow, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_CHART_HAS_DATATABLE:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasDataTable, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_CHART_HAS_LEGEND:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasLegend, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_CHART_TITLE:
            charPtr = va_arg(pargs, char*);
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartTitle, CAVT_OBJHANDLE,
            							   &chartTitleHandle));
            __caErrChk (Excel_SetProperty (chartTitleHandle, ExcelRpt_GetErrorInfo(), Excel_ChartTitleText, CAVT_CSTRING, charPtr));
            break;
        case  ER_CH_ATTR_PRINTING_CHART_SIZE:
            chartsize = va_arg(pargs, enum ExREnum_ExObjectSize);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPageSetup, CAVT_OBJHANDLE,
            							   &pageSetupHandle));
            __caErrChk (Excel_SetProperty (pageSetupHandle, ExcelRpt_GetErrorInfo(), Excel_PageSetupChartSize, CAVT_LONG, chartsize));
            break;
        case ER_CH_ATTR_PAGE_ORIENTATION:
            orientation = va_arg(pargs, enum ExREnum_ExPageOrientation);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPageSetup, CAVT_OBJHANDLE,
            							   &pageSetupHandle));
            __caErrChk (Excel_SetProperty (pageSetupHandle, ExcelRpt_GetErrorInfo(), Excel_PageSetupOrientation, CAVT_LONG, orientation));
            break;
        case ER_CH_ATTR_CHARTAREA_COLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR(longVal);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaInterior, CAVT_OBJHANDLE,
            							   &interiorHandle));
            __caErrChk (Excel_SetProperty (interiorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT,
            							   CA_VariantLong(bgr)));
            break;
        case ER_CH_ATTR_CHARTAREA_BORDER_LINESTYLE:
            lineStyle = va_arg(pargs, enum ExREnum_ExLineStyle);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_SetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
            							   CA_VariantInt(lineStyle)));
            break;
        case ER_CH_ATTR_CHARTAREA_BORDER_COLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR(longVal);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_SetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderColor, CAVT_VARIANT, CA_VariantInt(bgr)));
            break;
        case ER_CH_ATTR_CHARTAREA_BORDER_WEIGHT:
            weight = va_arg(pargs, enum ExREnum_ExBorderWeight);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_SetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderWeight, CAVT_VARIANT,
            							   CA_VariantInt(weight)));
            break;
        case ER_CH_ATTR_PLOTAREA_COLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR(longVal);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPlotArea, CAVT_OBJHANDLE,
            							   &plotAreaHandle));
            __caErrChk (Excel_GetProperty (plotAreaHandle, ExcelRpt_GetErrorInfo(), Excel_PlotAreaInterior, CAVT_OBJHANDLE,
            							   &interiorHandle));
            __caErrChk (Excel_SetProperty (interiorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT,
            							   CA_VariantLong(bgr)));
            break;
        case ER_CH_ATTR_DATATABLE_HAS_BORDER_HORIZ:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_SetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableHasBorderHorizontal, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_DATATABLE_HAS_BORDER_VERT:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_SetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableHasBorderVertical, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_DATATABLE_HAS_BORDER_OUTLINE:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_SetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableHasBorderOutline, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_DATATABLE_BORDER_LINESTYLE:
            lineStyle = va_arg(pargs, enum ExREnum_ExLineStyle);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_SetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT,
            							   CA_VariantInt(lineStyle)));
            break;
        case ER_CH_ATTR_DATATABLE_BORDER_COLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR(longVal);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_SetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderColor, CAVT_VARIANT, CA_VariantLong(bgr)));
            break;
        case ER_CH_ATTR_DATATABLE_BORDER_WEIGHT:
            weight = va_arg(pargs, enum ExREnum_ExBorderWeight);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_SetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderWeight, CAVT_VARIANT,
            							   CA_VariantInt(weight)));
        case ER_CH_ATTR_LEGEND_POSITION:
            position = va_arg(pargs, enum ExREnum_ExLegendPosition);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartLegend, CAVT_OBJHANDLE, &legendHandle));
            __caErrChk (Excel_SetProperty (legendHandle, ExcelRpt_GetErrorInfo(), Excel_LegendPosition, CAVT_LONG, position));
            break;
        case ER_CH_ATTR_LEGEND_COLOR:
            longVal = va_arg(pargs, long);
            bgr = RGBtoBGR(longVal);
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartLegend, CAVT_OBJHANDLE, &legendHandle));
            __caErrChk (Excel_GetProperty (legendHandle, ExcelRpt_GetErrorInfo(), Excel_LegendInterior, CAVT_OBJHANDLE, &interiorHandle));
            __caErrChk (Excel_SetProperty (interiorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT,
            							   CA_VariantLong(bgr)));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
	}

Error:
    va_end(pargs);
    CA_DiscardObjHandle(chartTitleHandle);
    CA_DiscardObjHandle(pageSetupHandle);
    CA_DiscardObjHandle(chartAreaHandle);
    CA_DiscardObjHandle(interiorHandle);
    CA_DiscardObjHandle(borderHandle);
    CA_DiscardObjHandle(plotAreaHandle);
    CA_DiscardObjHandle(dataTableHandle);
    CA_DiscardObjHandle(legendHandle);
    CA_DiscardObjHandle(chartObjHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_GetChartAttribute
//
// Purpose      : Get chart attributes
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetChartAttribute(CAObjHandle chartHandle, int attribute, void *attributeValue)
{
    HRESULT __result = S_OK;
    CAObjHandle chartTitleHandle = 0;
    CAObjHandle pageSetupHandle = 0;
    CAObjHandle chartAreaHandle = 0;
    CAObjHandle interiorHandle = 0;
    CAObjHandle borderHandle = 0;
    CAObjHandle plotAreaHandle = 0;
    CAObjHandle dataTableHandle = 0;
    CAObjHandle legendHandle = 0;
    CAObjHandle chartObjHandle  = 0;
    VARIANT GetValueV;
	int isEmbedded;
    double doubleVal;
    VBOOL boolVal;
    char *charPtr = NULL;

    CA_VariantSetEmpty(&GetValueV);

    if(!attributeValue)
        __caErrChk (E_INVALIDARG);

    switch(attribute)
    {
        case ER_CH_ATTR_CHART_NAME:
			__caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));
			if(isEmbedded)
			{
				__caErrChk (Excel_GetProperty (chartObjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectName, CAVT_CSTRING, &charPtr));
			}
			else
			{
				__caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartName, CAVT_CSTRING, &charPtr));
			}
            strcpy(attributeValue, charPtr);
            break;
        case ER_CH_ATTR_CHART_NAME_LEN:
			__caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));
			if(isEmbedded)
			{
				__caErrChk (Excel_GetProperty (chartObjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectName, CAVT_CSTRING, &charPtr));
			}
			else
			{
				__caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartName, CAVT_CSTRING, &charPtr));
			}
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CH_ATTR_CHART_INDEX:
			__caErrChk (CheckForEmbeddedChart(chartHandle, &isEmbedded, &chartObjHandle));
			if(isEmbedded)
			{
				__caErrChk (Excel_GetProperty (chartObjHandle, ExcelRpt_GetErrorInfo(), Excel_ChartObjectIndex, CAVT_LONG, attributeValue));
			}
			else
			{
				__caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartIndex, CAVT_LONG, attributeValue));
			}
            break;
        case ER_CH_ATTR_CHART_TYPE:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartType, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_CHART_DEPTH_PERCENT:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDepthPercent, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_CHART_ELEVATION:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartElevation, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_CHART_ROTATION:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartRotation, CAVT_VARIANT, &GetValueV));
            __caErrChk (CA_VariantGetLong (&GetValueV, attributeValue));
            break;
        case ER_CH_ATTR_CHART_DISPLAY_BLANK_AS:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDisplayBlanksAs, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_CHART_PLOT_BY:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPlotBy, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_CHART_PLOT_VISIBLE_ONLY:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPlotVisibleOnly, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_CHART_SIZE_WITH_WINDOW:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartSizeWithWindow, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_CHART_HAS_DATATABLE:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasDataTable, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_CHART_HAS_LEGEND:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasLegend, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_CHART_TITLE:
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartTitle, CAVT_OBJHANDLE,
            							   &chartTitleHandle));
            __caErrChk (Excel_GetProperty (chartTitleHandle, ExcelRpt_GetErrorInfo(), Excel_ChartTitleText, CAVT_CSTRING, &charPtr));
            strcpy(attributeValue, charPtr);
            break;
        case ER_CH_ATTR_CHART_TITLE_LEN:
            __caErrChk (Excel_SetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartTitle, CAVT_OBJHANDLE,
            							   &chartTitleHandle));
            __caErrChk (Excel_GetProperty (chartTitleHandle, ExcelRpt_GetErrorInfo(), Excel_ChartTitleText, CAVT_CSTRING, &charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case  ER_CH_ATTR_PRINTING_CHART_SIZE:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPageSetup, CAVT_OBJHANDLE, &pageSetupHandle));
            __caErrChk (Excel_GetProperty (pageSetupHandle, ExcelRpt_GetErrorInfo(), Excel_PageSetupChartSize, CAVT_LONG,
           								   attributeValue));
            break;
        case ER_CH_ATTR_PAGE_ORIENTATION:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPageSetup, CAVT_OBJHANDLE, &pageSetupHandle));
            __caErrChk (Excel_GetProperty (pageSetupHandle, ExcelRpt_GetErrorInfo(), Excel_PageSetupOrientation, CAVT_LONG,
            							   attributeValue));
            break;
        case ER_CH_ATTR_CHARTAREA_COLOR:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaInterior, CAVT_OBJHANDLE,
            							   &interiorHandle));
            __caErrChk (Excel_GetProperty (interiorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT, &GetValueV));
            CA_VariantGetDouble (&GetValueV, &doubleVal);
            *((long*)attributeValue) = BGRtoRGB((long)doubleVal);
            break;
        case ER_CH_ATTR_CHARTAREA_BORDER_LINESTYLE:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_GetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT, &GetValueV));
            CA_VariantGetInt(&GetValueV, attributeValue);
            break;
        case ER_CH_ATTR_CHARTAREA_BORDER_COLOR:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_GetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderColor, CAVT_VARIANT, &GetValueV));
            CA_VariantGetDouble(&GetValueV, &doubleVal);
            *((long*)attributeValue) = BGRtoRGB((long)doubleVal);
            break;
        case ER_CH_ATTR_CHARTAREA_BORDER_WEIGHT:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartChartArea, CAVT_OBJHANDLE, &chartAreaHandle));
            __caErrChk (Excel_GetProperty (chartAreaHandle, ExcelRpt_GetErrorInfo(), Excel_ChartAreaBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_GetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderWeight, CAVT_VARIANT, &GetValueV));
            CA_VariantGetInt(&GetValueV, attributeValue);
            break;
        case ER_CH_ATTR_PLOTAREA_COLOR:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartPlotArea, CAVT_OBJHANDLE, &plotAreaHandle));
            __caErrChk (Excel_GetProperty (plotAreaHandle, ExcelRpt_GetErrorInfo(), Excel_PlotAreaInterior, CAVT_OBJHANDLE,
            							   &interiorHandle));
            __caErrChk (Excel_GetProperty (interiorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT, &GetValueV));
            CA_VariantGetDouble(&GetValueV, &doubleVal);
            *((long*)attributeValue) =BGRtoRGB((long)doubleVal);
            break;
        case ER_CH_ATTR_DATATABLE_HAS_BORDER_HORIZ:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableHasBorderHorizontal, CAVT_BOOL,
            							   &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_DATATABLE_HAS_BORDER_VERT:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableHasBorderVertical, CAVT_BOOL,
            							   &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_DATATABLE_HAS_BORDER_OUTLINE:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableHasBorderOutline, CAVT_BOOL,
            							   &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_DATATABLE_BORDER_LINESTYLE:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_GetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderLineStyle, CAVT_VARIANT, &GetValueV));
            CA_VariantGetInt(&GetValueV, attributeValue);
            break;
        case ER_CH_ATTR_DATATABLE_BORDER_COLOR:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_GetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderColor, CAVT_VARIANT, &GetValueV));
            CA_VariantGetDouble(&GetValueV, &doubleVal);
            *((long*)attributeValue) = BGRtoRGB((long)doubleVal);
            break;
        case ER_CH_ATTR_DATATABLE_BORDER_WEIGHT:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartDataTable, CAVT_OBJHANDLE, &dataTableHandle));
            __caErrChk (Excel_GetProperty (dataTableHandle, ExcelRpt_GetErrorInfo(), Excel_DataTableBorder, CAVT_OBJHANDLE,
            							   &borderHandle));
            __caErrChk (Excel_GetProperty (borderHandle, ExcelRpt_GetErrorInfo(), Excel_BorderWeight, CAVT_VARIANT, &GetValueV));
            CA_VariantGetInt(&GetValueV, attributeValue);
            break;
        case ER_CH_ATTR_LEGEND_POSITION:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartLegend, CAVT_OBJHANDLE, &legendHandle));
            __caErrChk (Excel_GetProperty (legendHandle, ExcelRpt_GetErrorInfo(), Excel_LegendPosition, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_LEGEND_COLOR:
            __caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartLegend, CAVT_OBJHANDLE, &legendHandle));
            __caErrChk (Excel_GetProperty (legendHandle, ExcelRpt_GetErrorInfo(), Excel_LegendInterior, CAVT_OBJHANDLE, &interiorHandle));
            __caErrChk (Excel_GetProperty (interiorHandle, ExcelRpt_GetErrorInfo(), Excel_InteriorColor, CAVT_VARIANT, &GetValueV));
            CA_VariantGetDouble(&GetValueV, &doubleVal);
            *((long*)attributeValue) = BGRtoRGB((long)doubleVal);
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
	}

Error:
    CA_DiscardObjHandle(chartTitleHandle);
    CA_DiscardObjHandle(pageSetupHandle);
    CA_DiscardObjHandle(chartAreaHandle);
    CA_DiscardObjHandle(interiorHandle);
    CA_DiscardObjHandle(borderHandle);
    CA_DiscardObjHandle(plotAreaHandle);
    CA_DiscardObjHandle(dataTableHandle);
    CA_DiscardObjHandle(legendHandle);
    CA_DiscardObjHandle(chartObjHandle);
    CA_VariantClear(&GetValueV);
	CA_FreeMemory(charPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_SetChartAxistAttribute
//
// Purpose      : Set chart axis attributes
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC_C ExcelRpt_SetChartAxisAttribute(CAObjHandle chartHandle, enum ExREnum_ExAxisType axisType,
											   enum ExREnum_ExAxisGroup axisGroup,  int attribute, ...)
{
    HRESULT __result = S_OK;
    CAObjHandle axesHandle = 0;
    CAObjHandle axisHandle = 0;
    CAObjHandle axisTitleHandle = 0;
    CAObjHandle unitLabelHandle = 0;
    va_list pargs;
    int boolVal;
    double doubleVal;
    long longVal;
    char *charPtr;
    enum ExREnum_ExCategoryType categoryType;
    enum ExREnum_ExDisplayUnit  displayUnit;
    enum ExREnum_ExTickMark tickMark;
    enum ExREnum_ExScaleType scaleType;
    enum ExREnum_ExOrientation titleOrientation;
	enum ExcelEnum_XlAxisType xlAxisType = (enum ExcelEnum_XlAxisType)axisType;
	enum ExcelEnum_XlAxisGroup xlAxisGroup = (enum ExcelEnum_XlAxisGroup)axisGroup;

    va_start (pargs, attribute);

	__caErrChk (Excel_ChartAxes (chartHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, xlAxisGroup, &axesHandle));

	switch(attribute)
    {
        case ER_CH_ATTR_CATEGORY_TYPE:
            categoryType = va_arg(pargs, enum ExREnum_ExCategoryType);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlCategory, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisCategoryType, CAVT_LONG, categoryType));
            break;
        case ER_CH_ATTR_DISPLAY_UNIT:
            displayUnit = va_arg(pargs, enum ExREnum_ExDisplayUnit);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisDisplayUnit, CAVT_LONG, displayUnit));
            break;
        case ER_CH_ATTR_HAS_DISPLAY_UNIT_LABEL:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasDisplayUnitLabel, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_DISPLAY_UNIT_LABEL_CAPTION:
            charPtr = va_arg(pargs, char*);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisDisplayUnitLabel, CAVT_OBJHANDLE,
            							   &unitLabelHandle));
            __caErrChk (Excel_SetProperty (unitLabelHandle, ExcelRpt_GetErrorInfo(), Excel_DisplayUnitLabelCaption, CAVT_CSTRING,
            							   charPtr));
            break;
        case ER_CH_ATTR_HAS_MAJOR_GRIDLINES:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasMajorGridlines, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_HAS_MINOR_GRIDLINES:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasMinorGridlines, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_MAJOR_TICK_MARK:
            tickMark = va_arg(pargs, enum ExREnum_ExTickMark);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMajorTickMark, CAVT_LONG, tickMark));
            break;
        case ER_CH_ATTR_MINOR_TICK_MARK:
            tickMark = va_arg(pargs, enum ExREnum_ExTickMark);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinorTickMark, CAVT_LONG, tickMark));
            break;
        case ER_CH_ATTR_MAJOR_UNIT:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMajorUnit, CAVT_DOUBLE, doubleVal));
            break;
        case ER_CH_ATTR_MAJOR_UNIT_IS_AUTO:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMajorUnitIsAuto, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_MINOR_UNIT:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinorUnit, CAVT_DOUBLE, doubleVal));
            break;
        case ER_CH_ATTR_MINOR_UNIT_IS_AUTO:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinorUnitIsAuto, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_MAXIMUM_SCALE:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMaximumScale, CAVT_DOUBLE, doubleVal));
            break;
        case ER_CH_ATTR_MAXIMUM_SCALE_IS_AUTO:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMaximumScaleIsAuto, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_MINIMUM_SCALE:
            doubleVal = va_arg(pargs, double);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinimumScale, CAVT_DOUBLE, doubleVal));
            break;
        case ER_CH_ATTR_MINIMUM_SCALE_IS_AUTO:
            boolVal = va_arg(pargs, int);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinimumScaleIsAuto, CAVT_BOOL,
            							   boolVal ? VTRUE : VFALSE));
            break;
        case ER_CH_ATTR_TICK_MARK_SPACING:
            longVal = va_arg(pargs, long);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTickMarkSpacing, CAVT_LONG, longVal));
            break;
        case ER_CH_ATTR_SCALE_TYPE:
            scaleType = va_arg(pargs, enum ExREnum_ExScaleType);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisScaleType, CAVT_LONG, scaleType));
            break;
        case ER_CH_ATTR_AXIS_TITLE:
            charPtr = va_arg(pargs, char*);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisAxisTitle, CAVT_OBJHANDLE, &axisTitleHandle));
            __caErrChk (Excel_SetProperty (axisTitleHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTitleText, CAVT_CSTRING, charPtr));
            break;
        case ER_CH_ATTR_AXIS_TITLE_ORIENTATION:
            titleOrientation = va_arg(pargs, enum ExREnum_ExOrientation);
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisAxisTitle, CAVT_OBJHANDLE, &axisTitleHandle));
            __caErrChk (Excel_SetProperty (axisTitleHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTitleOrientation, CAVT_VARIANT,
            							   CA_VariantInt(titleOrientation)));
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
    }

Error:
    va_end(pargs);
    CA_DiscardObjHandle(axesHandle);
    CA_DiscardObjHandle(axisHandle);
    CA_DiscardObjHandle(axisTitleHandle);
    CA_DiscardObjHandle(unitLabelHandle);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Function     : ExcelRpt_GetChartAxistAttribute
//
// Purpose      : Get chart axis attributes
//
// Result       : ActiveX error code
//------------------------------------------------------------------------------------------------------------------
HRESULT CVIFUNC ExcelRpt_GetChartAxisAttribute(CAObjHandle chartHandle, enum ExREnum_ExAxisType axisType,
                                               enum ExREnum_ExAxisGroup axisGroup,  int attribute,
                                               void *attributeValue)
{
    HRESULT __result = S_OK;
    CAObjHandle axesHandle = 0;
    CAObjHandle axisHandle = 0;
    CAObjHandle axisTitleHandle = 0;
    CAObjHandle displayUnitlabelHandle = 0;
    VARIANT GetValueV;
    VBOOL boolVal;
    char *charPtr = NULL;
	enum ExcelEnum_XlAxisType xlAxisType = (enum ExcelEnum_XlAxisType)axisType;
	enum ExcelEnum_XlAxisGroup xlAxisGroup = (enum ExcelEnum_XlAxisGroup)axisGroup;
				
    CA_VariantSetEmpty(&GetValueV);

    if(!attributeValue)
        __caErrChk (E_INVALIDARG);

    __caErrChk (Excel_ChartAxes (chartHandle, ExcelRpt_GetErrorInfo(), CA_DEFAULT_VAL, xlAxisGroup, &axesHandle));

    switch(attribute)
    {
        case ER_CH_ATTR_CATEGORY_TYPE:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), ExcelConst_xlCategory, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisCategoryType, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_DISPLAY_UNIT:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisDisplayUnit, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_HAS_DISPLAY_UNIT_LABEL:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasDisplayUnitLabel, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_DISPLAY_UNIT_LABEL_CAPTION:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisDisplayUnitLabel, CAVT_OBJHANDLE,
            							   &displayUnitlabelHandle));
            __caErrChk (Excel_GetProperty (displayUnitlabelHandle, ExcelRpt_GetErrorInfo(), Excel_DisplayUnitLabelText, CAVT_CSTRING,
            							   &charPtr));
            strcpy(attributeValue, charPtr);
            break;
        case ER_CH_ATTR_DISPLAY_UNIT_LABEL_CAPTION_LEN:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisDisplayUnitLabel, CAVT_OBJHANDLE,
            							   &displayUnitlabelHandle));
            __caErrChk (Excel_GetProperty (displayUnitlabelHandle, ExcelRpt_GetErrorInfo(), Excel_DisplayUnitLabelText, CAVT_CSTRING,
            							   &charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CH_ATTR_HAS_MAJOR_GRIDLINES:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasMajorGridlines, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_HAS_MINOR_GRIDLINES:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasMinorGridlines, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_MAJOR_TICK_MARK:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMajorTickMark, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_MAJOR_UNIT:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMajorUnit, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_MAJOR_UNIT_IS_AUTO:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMajorUnitIsAuto, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_MINOR_UNIT:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinorUnit, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_MINOR_UNIT_IS_AUTO:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinorUnitIsAuto, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_MAXIMUM_SCALE:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMaximumScale, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_MAXIMUM_SCALE_IS_AUTO:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMaximumScaleIsAuto, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_MINIMUM_SCALE:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinimumScale, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_MINIMUM_SCALE_IS_AUTO:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisMinimumScaleIsAuto, CAVT_BOOL, &boolVal));
            *((int*)attributeValue) = (boolVal == VFALSE) ? 0 : 1;
            break;
        case ER_CH_ATTR_TICK_MARK_SPACING:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTickMarkSpacing, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_SCALE_TYPE:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisScaleType, CAVT_LONG, attributeValue));
            break;
        case ER_CH_ATTR_TOP:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTop, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_LEFT:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisLeft, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_WIDTH:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisWidth, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_HEIGHT:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHeight, CAVT_DOUBLE, attributeValue));
            break;
        case ER_CH_ATTR_AXIS_TITLE:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisAxisTitle, CAVT_OBJHANDLE, &axisTitleHandle));
            __caErrChk (Excel_GetProperty (axisTitleHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTitleText, CAVT_CSTRING, charPtr));
            strcpy(attributeValue, charPtr);
            CA_FreeMemory(charPtr);
            break;
        case ER_CH_ATTR_AXIS_TITLE_LEN:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisAxisTitle, CAVT_OBJHANDLE, &axisTitleHandle));
            __caErrChk (Excel_GetProperty (axisTitleHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTitleText, CAVT_CSTRING, charPtr));
            *((int*)attributeValue) = (int)strlen(charPtr);
            break;
        case ER_CH_ATTR_AXIS_TITLE_ORIENTATION:
            __caErrChk (Excel_AxesItem (axesHandle, ExcelRpt_GetErrorInfo(), xlAxisType, xlAxisGroup, &axisHandle));
            __caErrChk (Excel_SetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisHasTitle, CAVT_BOOL, VTRUE));
            __caErrChk (Excel_GetProperty (axisHandle, ExcelRpt_GetErrorInfo(), Excel_AxisAxisTitle, CAVT_OBJHANDLE, &axisTitleHandle));
            __caErrChk (Excel_GetProperty (axisTitleHandle, ExcelRpt_GetErrorInfo(), Excel_AxisTitleOrientation, CAVT_VARIANT,
            							   &GetValueV));
            CA_VariantGetInt(&GetValueV, attributeValue);
            break;
        default:
        	__caErrChk (E_INVALIDARG);
        	break;
	}

Error:
    CA_DiscardObjHandle(axesHandle);
    CA_DiscardObjHandle(axisHandle);
    CA_DiscardObjHandle(axisTitleHandle);
    CA_DiscardObjHandle(displayUnitlabelHandle);
    CA_VariantClear(&GetValueV);
	CA_FreeMemory(charPtr);
    return __result;
}

//------------------------------------------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------------------------------------------

static HRESULT CheckForEmbeddedChart(CAObjHandle chartHandle, int *isEmbedded, CAObjHandle *chartObjHandle)
{
	HRESULT __result = S_OK;
	LPDISPATCH chartDisp = NULL;
	ITypeInfo *chartTypeInfo = NULL;
	LPTYPEATTR chartTypeAttr = NULL;
	CAObjHandle chartObjHandleL = 0;
	int isEmbeddedL = FALSE;

	GUID workbookIID = {0x000208DA, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
	GUID chartObjIID1 = {0x000208CF, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
	GUID chartObjIID2 = {0x000208CF, 0x0001, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};

	if (chartObjHandle)
		*chartObjHandle = 0;

	__caErrChk (Excel_GetProperty (chartHandle, ExcelRpt_GetErrorInfo(), Excel_ChartParent, CAVT_OBJHANDLE, &chartObjHandleL));

	__caErrChk (CA_GetInterfaceFromObjHandle (chartObjHandleL, &IID_IDispatch, 1, &chartDisp, NULL));
	__caErrChk (chartDisp->lpVtbl->GetTypeInfo (chartDisp, 0, LOCALE_NEUTRAL, &chartTypeInfo));
	__caErrChk (chartTypeInfo->lpVtbl->GetTypeAttr (chartTypeInfo, &chartTypeAttr));

	if (IsEqualGUID((void*)&(chartTypeAttr->guid), (void*)&workbookIID))
	{
	   // this is a chart
	   isEmbeddedL = FALSE;
	}
	else if (IsEqualGUID((void*)&(chartTypeAttr->guid),(void*)&chartObjIID1) ||
			 IsEqualGUID((void*)&(chartTypeAttr->guid), (void*)&chartObjIID2))
	{
		// this is an embedded chart
	   isEmbeddedL = TRUE;
	}
	else
	{
		__caErrChk(E_INVALIDARG);
	}

Error:
	if(!FAILED(__result))
	{
		if (isEmbeddedL && chartObjHandle)
		{
			*chartObjHandle = chartObjHandleL;
			chartObjHandleL = 0;
		}
		*isEmbedded = isEmbeddedL;
	}
	if (chartDisp)
		chartDisp->lpVtbl->Release (chartDisp);
	if (chartTypeAttr)
		chartTypeInfo->lpVtbl->ReleaseTypeAttr (chartTypeInfo, chartTypeAttr);
	if (chartTypeInfo)
		chartTypeInfo->lpVtbl->Release (chartTypeInfo);
	CA_DiscardObjHandle (chartObjHandleL);
	return __result;
}

// The RGB color in cvi is implemented as BGR with the first two hex digits for blue and the second two for
// green and the last two for red.
static long BGRtoRGB (long bgr)
{
	long blueval = 0;
	long greenval = 0;
	long redval = 0;
	long rgb = 0;

	long bluemask = 0xff0000;
	long greenmask = 0x00ff00;
	long redmask = 0x0000ff;

	blueval = bluemask & bgr;
	greenval = greenmask & bgr;
	redval = redmask & bgr;

	blueval = blueval >> 16;
	redval = redval <<   16;

	rgb = blueval | redval | greenval ;

	return rgb;
}

static long RGBtoBGR (long rgb)
{
	long blueval = 0;
	long greenval = 0;
	long redval = 0;
	long bgr = 0;

	long bluemask = 0x0000ff;
	long greenmask = 0x00ff00;
	long redmask = 0xff0000;

	blueval = bluemask & rgb;
	greenval = greenmask & rgb;
	redval = redmask & rgb;

	blueval = blueval << 16;
	redval = redval >> 16;

	bgr = blueval | greenval | redval;

	return bgr;
}

static void AtExitHandler(void)
{
	if (gThreadLocalErrorInfo)
	{
		CmtDiscardThreadLocalVar(gThreadLocalErrorInfo);
		gThreadLocalErrorInfo = 0;
	}
}
