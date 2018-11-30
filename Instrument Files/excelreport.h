/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 2000-2017.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       ExcelReport.h                                                 */
/* Purpose:     Header file for CVI Excel Report Generation Instrument Driver */
/* Notes: 		This library communicates with the Microsoft Excel Automation */
/*				Server via a generated ActiveX controller instrument driver   */
/*				(Excel2000.fp).  This generated driver must be present to use */
/*				this library.												  */
/*                                                                            */
/*============================================================================*/

#include <cvidef.h>

#ifndef _EXCELREPORT_H_
#define _EXCELREPORT_H_


#include <cviauto.h>
#include <userint.h>
#ifdef __cplusplus
	extern "C" {
#endif

#define MAX_FOUND_CELLRANGE_LENGTH 				10

#define ER_AP_ATTR_VISIBLE						1
#define ER_AP_ATTR_WINDOW_STATE 				2
#define ER_AP_ATTR_TOP							3
#define ER_AP_ATTR_LEFT							4
#define ER_AP_ATTR_WIDTH						5
#define ER_AP_ATTR_HEIGHT						6


#define ER_WB_ATTR_NUM_WORKSHEETS   			1
#define ER_WB_ATTR_NUM_CHARTS       			2
#define ER_WB_ATTR_NUM_TOTAL_SHEETS 			3

#define ER_WS_ATTR_USED_RANGE					1
#define ER_WS_ATTR_USED_RANGE_LEN				2
#define ER_WS_ATTR_PRINT_PAGE_ORIENTATION		3
#define ER_WS_ATTR_NAME							4
#define ER_WS_ATTR_NAME_LEN						5
#define ER_WS_ATTR_NUM_EMBEDDED_CHARTS			6
#define ER_WS_ATTR_INDEX						7

#define ER_CR_ATTR_VALUE						1
#define ER_CR_ATTR_TEXT							2
#define ER_CR_ATTR_TEXT_LEN						3
#define ER_CR_ATTR_COMMENT						4
#define ER_CR_ATTR_COMMENT_LEN					5
#define ER_CR_ATTR_FORMULA						6
#define ER_CR_ATTR_FORMULA_LEN					7
#define ER_CR_ATTR_NUMBER_FORMAT				8
#define ER_CR_ATTR_NUMBER_FORMAT_LEN			9
#define ER_CR_ATTR_HORIZ_ALIGN					10
#define ER_CR_ATTR_VERT_ALIGN					11
#define ER_CR_ATTR_BGCOLOR						12
#define ER_CR_ATTR_BGCOLOR_INDEX				13
#define ER_CR_ATTR_FONT_COLOR					14
#define ER_CR_ATTR_FONT_COLOR_INDEX				15
#define ER_CR_ATTR_FONT_BOLD					16
#define ER_CR_ATTR_FONT_NAME					17
#define ER_CR_ATTR_FONT_NAME_LEN				18
#define ER_CR_ATTR_FONT_ITALIC					19
#define ER_CR_ATTR_FONT_SIZE					20
#define ER_CR_ATTR_FONT_STRIKETHROUGH			21
#define ER_CR_ATTR_FONT_UNDERLINE       		22
#define ER_CR_ATTR_WRAP_TEXT					23
#define ER_CR_ATTR_ROW_HEIGHT					24
#define ER_CR_ATTR_COLUMN_WIDTH					25

#define ER_CH_ATTR_CHART_NAME					1
#define ER_CH_ATTR_CHART_NAME_LEN				2
#define ER_CH_ATTR_CHART_INDEX					3
#define ER_CH_ATTR_CHART_TYPE					4
#define ER_CH_ATTR_CHART_DEPTH_PERCENT			5
#define ER_CH_ATTR_CHART_ELEVATION				6
#define ER_CH_ATTR_CHART_ROTATION				7
#define ER_CH_ATTR_CHART_DISPLAY_BLANK_AS		8
#define ER_CH_ATTR_CHART_PLOT_BY				9
#define ER_CH_ATTR_CHART_PLOT_VISIBLE_ONLY		10
#define ER_CH_ATTR_CHART_SIZE_WITH_WINDOW		11
#define ER_CH_ATTR_CHART_HAS_DATATABLE			12
#define ER_CH_ATTR_CHART_HAS_LEGEND				13
#define ER_CH_ATTR_CHART_TITLE					14
#define ER_CH_ATTR_CHART_TITLE_LEN				15
#define ER_CH_ATTR_PRINTING_CHART_SIZE			16
#define ER_CH_ATTR_PAGE_ORIENTATION				17
#define ER_CH_ATTR_CHARTAREA_COLOR				18
#define ER_CH_ATTR_CHARTAREA_COLOR_INDEX		19
#define ER_CH_ATTR_CHARTAREA_BORDER_LINESTYLE	20
#define ER_CH_ATTR_CHARTAREA_BORDER_COLOR		21
#define ER_CH_ATTR_CHARTAREA_BORDER_COLOR_INDEX	22
#define ER_CH_ATTR_CHARTAREA_BORDER_WEIGHT		23
#define ER_CH_ATTR_PLOTAREA_COLOR				24
#define ER_CH_ATTR_PLOTAREA_COLOR_INDEX			25
#define ER_CH_ATTR_DATATABLE_HAS_BORDER_HORIZ	26
#define ER_CH_ATTR_DATATABLE_HAS_BORDER_VERT	27
#define ER_CH_ATTR_DATATABLE_HAS_BORDER_OUTLINE 28
#define ER_CH_ATTR_DATATABLE_BORDER_LINESTYLE	29
#define ER_CH_ATTR_DATATABLE_BORDER_COLOR		30
#define ER_CH_ATTR_DATATABLE_BORDER_COLOR_INDEX	31
#define ER_CH_ATTR_DATATABLE_BORDER_WEIGHT		32
#define ER_CH_ATTR_LEGEND_POSITION				33
#define ER_CH_ATTR_LEGEND_COLOR					34
#define ER_CH_ATTR_LEGEND_COLOR_INDEX			35

#define ER_CH_ATTR_CATEGORY_TYPE				36
#define ER_CH_ATTR_DISPLAY_UNIT					37
#define ER_CH_ATTR_HAS_DISPLAY_UNIT_LABEL		38
#define ER_CH_ATTR_HAS_MAJOR_GRIDLINES			39
#define ER_CH_ATTR_HAS_MINOR_GRIDLINES			40
#define ER_CH_ATTR_MAJOR_TICK_MARK				41
#define ER_CH_ATTR_MINOR_TICK_MARK				42
#define ER_CH_ATTR_MAJOR_UNIT					43
#define ER_CH_ATTR_MAJOR_UNIT_IS_AUTO			44
#define ER_CH_ATTR_MINOR_UNIT					45
#define ER_CH_ATTR_MINOR_UNIT_IS_AUTO			46
#define ER_CH_ATTR_MAXIMUM_SCALE				47
#define ER_CH_ATTR_MAXIMUM_SCALE_IS_AUTO		48
#define ER_CH_ATTR_MINIMUM_SCALE				49
#define ER_CH_ATTR_MINIMUM_SCALE_IS_AUTO		50
#define ER_CH_ATTR_TICK_MARK_SPACING			51
#define ER_CH_ATTR_SCALE_TYPE					52
#define ER_CH_ATTR_AXIS_TITLE					53
#define ER_CH_ATTR_AXIS_TITLE_ORIENTATION		54
#define ER_CH_ATTR_DISPLAY_UNIT_LABEL_CAPTION			55
#define ER_CH_ATTR_TOP							56
#define ER_CH_ATTR_LEFT							57
#define ER_CH_ATTR_WIDTH						58
#define ER_CH_ATTR_HEIGHT						59
#define ER_CH_ATTR_DISPLAY_UNIT_LABEL_CAPTION_LEN		60
#define ER_CH_ATTR_AXIS_TITLE_LEN				61

enum ExREnum_ExBool
{
	ExRConst_False = 0,
	ExRConst_True = 1,
	_Excel_ExBoolForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExDataType
{
	ExRConst_dataNotInitialized = 0,
	ExRConst_dataShort = CAVT_SHORT,
	ExRConst_dataLong =  CAVT_LONG,
	ExRConst_dataInt =  CAVT_INT,
	ExRConst_dataFloat = CAVT_FLOAT,
	ExRConst_dataDouble = CAVT_DOUBLE,
	ExRConst_dataString = CAVT_CSTRING,
	ExRConst_dataEmpty = CAVT_EMPTY,
	ExRConst_dataCurrency = CAVT_CY,
	ExRConst_dataDate = CAVT_DATE,
	_Excel_ExDataTypeForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExSaveOptions
{

	ExRConst_DoNotSaveChanges = 0,
	ExRConst_SaveChanges     = 1,
	_Excel_ExSaveOptionsForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExFillOptions
{
   ExRConst_UP = 1,
   ExRConst_DOWN = 2,
   ExRConst_LEFT = 3,
   ExRConst_RIGHT = 4,
	_Excel_ExFillOptionsForceSizeToFourBytes = 0xFFFFFFFF
 };

enum ExREnum_ExInsertShiftDirection
{
	ExRConst_ShiftDown = -4121,
	ExRConst_ShiftToRight = -4161,
	_Excel_ExInsertShiftDirectionForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExDeleteShiftDirection
{
	ExRConst_ShiftToLeft = -4159,
	ExRConst_ShiftUp = -4162,
	_Excel_ExDeleteShiftDirectionForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExSortOrder
{
	ExRConst_Ascending = 1,
	ExRConst_Descending = 2,
	_Excel_ExSortOrderForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExSortOrientation
{
	ExRConst_SortRows = 2,
	ExRConst_SortColumns = 1,
	_Excel_ExSortOrientationForceSizeToFourBytes = 0xFFFFFFFF
};


enum ExREnum_ExYesNo
{
	ExRConst_No = 2,
	ExRConst_Yes = 1,
	_Excel_ExYesNoForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExBordersIndex
{
	ExRConst_InsideHorizontal = 1,
	ExRConst_InsideVertical = 2,
	ExRConst_DiagonalDown = 4,
	ExRConst_DiagonalUp = 8,
	ExRConst_EdgeBottom = 16,
	ExRConst_EdgeLeft = 32,
	ExRConst_EdgeRight = 64,
	ExRConst_EdgeTop = 128,
	_Excel_ExBordersIndexForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExLineStyle
{
	ExRConst_Continuous = 1,
	ExRConst_Dash = -4115,
	ExRConst_DashDot = 4,
	ExRConst_DashDotDot = 5,
	ExRConst_Dot = -4118,
	ExRConst_Double = -4119,
	ExRConst_SlantDashDot = 13,
	ExRConst_LineStyleNone = -4142,
	_Excel_ExLineStyleForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExBorderWeight
{
	ExRConst_Hairline = 1,
	ExRConst_Medium = -4138,
	ExRConst_Thick = 4,
	ExRConst_Thin = 2,
	_Excel_ExBorderWeightForceSizeToFourBytes = 0xFFFFFFFF
};


enum ExREnum_ExWindowState
{
	ExRConst_Maximized = -4137,
	ExRConst_Minimized = -4140,
	ExRConst_Normal = -4143,
	_Excel_ExWindowStateForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExFileFormat
{
	ExRConst_DefaultFileFormat = 0,
	ExRConst_AddIn = 18,
	ExRConst_CSV = 6,
	ExRConst_CSVMac = 22,
	ExRConst_CSVMSDOS = 24,
	ExRConst_CSVWindows = 23,
	ExRConst_DBF2 = 7,
	ExRConst_DBF3 = 8,
	ExRConst_DBF4 = 11,
	ExRConst_DIF = 9,
	ExRConst_Excel2 = 16,
	ExRConst_Excel2FarEast = 27,
	ExRConst_Excel3 = 29,
	ExRConst_Excel4 = 33,
	ExRConst_Excel5 = 39,
	ExRConst_Excel7 = 39,
	ExRConst_Excel9795 = 43,
	ExRConst_Excel4Workbook = 35,
	ExRConst_IntlAddIn = 26,
	ExRConst_IntlMacro = 25,
	ExRConst_WorkbookNormal = -4143,
	ExRConst_SYLK = 2,
	ExRConst_Template = 17,
	ExRConst_CurrentPlatformText = -4158,
	ExRConst_TextMac = 19,
	ExRConst_TextMSDOS = 21,
	ExRConst_TextPrinter = 36,
	ExRConst_TextWindows = 20,
	ExRConst_WJ2WD1 = 14,
	ExRConst_WK1 = 5,
	ExRConst_WK1ALL = 31,
	ExRConst_WK1FMT = 30,
	ExRConst_WK3 = 15,
	ExRConst_WK4 = 38,
	ExRConst_WK3FM3 = 32,
	ExRConst_WKS = 4,
	ExRConst_Works2FarEast = 28,
	ExRConst_WQ1 = 34,
	ExRConst_WJ3 = 40,
	ExRConst_WJ3FJ3 = 41,
	ExRConst_UnicodeText = 42,
	ExRConst_Html = 44,
	_Excel_ExFileFormatForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExSheetType
{
	ExRConst_Chart = -4109,
	ExRConst_Worksheet = -4167,
	_Excel_ExSheetTypeForceSizeToFourBytes = 0xFFFFFFFF
};


enum ExREnum_ExChartWizardGallery
{
	ExRConst_GalleryArea = 1,
	ExRConst_GalleryBar = 2,
	ExRConst_GalleryColumn = 3,
	ExRConst_GalleryLine = 4,
	ExRConst_GalleryPie = 5,
	ExRConst_GalleryRadar = -4151,
	ExRConst_GalleryXYScatter = -4169,
	ExRConst_GalleryCombination = -4111,
	ExRConst_Gallery3DArea = -4098,
	ExRConst_Gallery3DBar = -4099,
	ExRConst_Gallery3DColumn = -4100,
	ExRConst_Gallery3DLine = -4101,
	ExRConst_Gallery3DPie = -4102,
	ExRConst_Gallery3DSurface = -4103,
	ExRConst_GalleryDoughnut = -4120,
	_Excel_ExChartWizardGalleryForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExChartType
{
	ExRConst_DefaultChartType = 0,
	ExRConst_ColumnClustered = 51,
	ExRConst_ColumnStacked = 52,
	ExRConst_ColumnStacked100 = 53,
	ExRConst_3DColumnClustered = 54,
	ExRConst_3DColumnStacked = 55,
	ExRConst_3DColumnStacked100 = 56,
	ExRConst_BarClustered = 57,
	ExRConst_BarStacked = 58,
	ExRConst_BarStacked100 = 59,
	ExRConst_3DBarClustered = 60,
	ExRConst_3DBarStacked = 61,
	ExRConst_3DBarStacked100 = 62,
	ExRConst_LineStacked = 63,
	ExRConst_LineStacked100 = 64,
	ExRConst_LineMarkers = 65,
	ExRConst_LineMarkersStacked = 66,
	ExRConst_LineMarkersStacked100 = 67,
	ExRConst_PieOfPie = 68,
	ExRConst_PieExploded = 69,
	ExRConst_3DPieExploded = 70,
	ExRConst_BarOfPie = 71,
	ExRConst_XYScatterSmooth = 72,
	ExRConst_XYScatterSmoothNoMarkers = 73,
	ExRConst_XYScatterLines = 74,
	ExRConst_XYScatterLinesNoMarkers = 75,
	ExRConst_AreaStacked = 76,
	ExRConst_AreaStacked100 = 77,
	ExRConst_3DAreaStacked = 78,
	ExRConst_3DAreaStacked100 = 79,
	ExRConst_DoughnutExploded = 80,
	ExRConst_RadarMarkers = 81,
	ExRConst_RadarFilled = 82,
	ExRConst_Surface = 83,
	ExRConst_SurfaceWireframe = 84,
	ExRConst_SurfaceTopView = 85,
	ExRConst_SurfaceTopViewWireframe = 86,
	ExRConst_Bubble = 15,
	ExRConst_Bubble3DEffect = 87,
	ExRConst_StockHLC = 88,
	ExRConst_StockOHLC = 89,
	ExRConst_StockVHLC = 90,
	ExRConst_StockVOHLC = 91,
	ExRConst_CylinderColClustered = 92,
	ExRConst_CylinderColStacked = 93,
	ExRConst_CylinderColStacked100 = 94,
	ExRConst_CylinderBarClustered = 95,
	ExRConst_CylinderBarStacked = 96,
	ExRConst_CylinderBarStacked100 = 97,
	ExRConst_CylinderCol = 98,
	ExRConst_ConeColClustered = 99,
	ExRConst_ConeColStacked = 100,
	ExRConst_ConeColStacked100 = 101,
	ExRConst_ConeBarClustered = 102,
	ExRConst_ConeBarStacked = 103,
	ExRConst_ConeBarStacked100 = 104,
	ExRConst_ConeCol = 105,
	ExRConst_PyramidColClustered = 106,
	ExRConst_PyramidColStacked = 107,
	ExRConst_PyramidColStacked100 = 108,
	ExRConst_PyramidBarClustered = 109,
	ExRConst_PyramidBarStacked = 110,
	ExRConst_PyramidBarStacked100 = 111,
	ExRConst_PyramidCol = 112,
	ExRConst_3DColumn = -4100,
	ExRConst_Line = 4,
	ExRConst_3DLine = -4101,
	ExRConst_3DPie = -4102,
	ExRConst_Pie = 5,
	ExRConst_XYScatter = -4169,
	ExRConst_3DArea = -4098,
	ExRConst_Area = 1,
	ExRConst_Doughnut = -4120,
	ExRConst_Radar = -4151,
	ExRConst_3DSurface = -4103,
	ExRConst_Bar = 2,
	ExRConst_Column = 3,
	ExRConst_Combination = -4111,
	ExRConst_3DBar = -4099,
	_Excel_ExChartTypeForceSizeToFourBytes = 0xFFFFFFFF
};


enum ExREnum_ExRowCol
{
	ExRConst_DefaultRowCol =0,
	ExRConst_Columns = 2,
	ExRConst_Rows = 1,
	_Excel_ExRowColForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExDisplayBlanksAs
{
	ExRConst_Interpolated = 3,
	ExRConst_NotPlotted = 1,
	ExRConst_Zero = 2,
	_Excel_ExDisplayBlanksAsForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExObjectSize
{
	ExRConst_FitToPage = 2,
	ExRConst_FullPage = 3,
	ExRConst_ScreenSize = 1,
	_Excel_ExObjectSizeForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExPageOrientation
{
	ExRConst_Landscape = 2,
	ExRConst_Portrait = 1,
	_Excel_ExPageOrientationForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExLegendPosition
{
	ExRConst_LegendPositionBottom = -4107,
	ExRConst_LegendPositionCorner = 2,
	ExRConst_LegendPositionLeft = -4131,
	ExRConst_LegendPositionRight = -4152,
	ExRConst_LegendPositionTop = -4160,
	_Excel_ExLegendPositionForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExAxisGroup
{
	ExRConst_Primary = 1,
	ExRConst_Secondary = 2,
	_Excel_ExAxisGroupForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExCategoryType
{
	ExRConst_CategoryScale = 2,
	ExRConst_TimeScale = 3,
	ExRConst_AutomaticScale = -4105,
	_Excel_ExCategoryTypeForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExAxisType
{
	ExRConst_Category = 1,
	ExRConst_SeriesAxis = 3,
	ExRConst_Value = 2,
	_Excel_ExAxisTypeForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExDisplayUnit
{
	ExRConst_Hundreds = -2,
	ExRConst_Thousands = -3,
	ExRConst_TenThousands = -4,
	ExRConst_HundredThousands = -5,
	ExRConst_Millions = -6,
	ExRConst_TenMillions = -7,
	ExRConst_HundredMillions = -8,
	ExRConst_ThousandMillions = -9,
	ExRConst_MillionMillions = -10,
	_Excel_ExDisplayUnitForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExTickMark
{
	ExRConst_TickMarkCross = 4,
	ExRConst_TickMarkInside = 2,
	ExRConst_TickMarkNone = -4142,
	ExRConst_TickMarkOutside = 3,
	_Excel_ExTickMarkForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExScaleType
{
	ExRConst_ScaleLinear = -4132,
	ExRConst_ScaleLogarithmic = -4133,
	_Excel_ExScaleTypeForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExOrientation
{
	ExRConst_Downward = -4170,
	ExRConst_Horizontal = -4128,
	ExRConst_Upward = -4171,
	ExRConst_Vertical = -4166,
	_Excel_ExOrientationForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExUnderlineStyle
{
	ExRConst_UnderlineStyleDouble = -4119,
	ExRConst_UnderlineStyleDoubleAccounting = 5,
	ExRConst_UnderlineStyleNone = -4142,
	ExRConst_UnderlineStyleSingle = 2,
	ExRConst_UnderlineStyleSingleAccounting = 4,
	_Excel_ExUnderlineStyleForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExColorIndex
{
	ExRConst_ColorIndexAutomatic = -4105,
	ExRConst_ColorIndexNone = -4142,
	_Excel_ExColorIndexForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExFindLookIn
{
	ExRConst_Formulas = -4123,
	ExRConst_Comments = -4144,
	ExRConst_Values = -4163,
	_Excel_ExFindLookInForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExLookAt
{
	ExRConst_Part = 2,
	ExRConst_Whole = 1,
	_Excel_ExLookAtForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExSearchOrder
{
	ExRConst_ByColumns = 2,
	ExRConst_ByRows = 1,
	_Excel_ExSearchOrderForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExSearchDirection
{
	ExRConst_Next = 1,
	ExRConst_Previous = 2,
	_Excel_ExSearchDirectionForceSizeToFourBytes = 0xFFFFFFFF
};


enum ExREnum_ExHAlign
{
	ExRConst_HAlignCenter = -4108,
	ExRConst_HAlignCenterAcrossSelection = 7,
	ExRConst_HAlignDistributed = -4117,
	ExRConst_HAlignFill = 5,
	ExRConst_HAlignGeneral = 1,
	ExRConst_HAlignJustify = -4130,
	ExRConst_HAlignLeft = -4131,
	ExRConst_HAlignRight = -4152,
	_Excel_ExHAlignForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExVAlign
{
	ExRConst_VAlignBottom = -4107,
	ExRConst_VAlignCenter = -4108,
	ExRConst_VAlignDistributed = -4117,
	ExRConst_VAlignJustify = -4130,
	ExRConst_VAlignTop = -4160,
	_Excel_ExVAlignForceSizeToFourBytes = 0xFFFFFFFF
};

enum ExREnum_ExInsertMode
{
	ExRConst_OverwriteGrow = 1,
	ExRConst_Overwrite = 2,
	ExRConst_InsertRow = 3,
	ExRConst_InsertColumn = 4,
	ExRConst_InsertRowAndColumn = 5,
	_Excel_ExInsertModeForceSizeToFourBytes = 0xFFFFFFFF
};

//*****************************************************************************************************************

ERRORINFO * CVIFUNC ExcelRpt_GetErrorInfo(void);

HRESULT CVIFUNC ExcelRpt_ApplicationNew (int makeVisible, CAObjHandle *applicationHandle);

HRESULT CVIFUNC ExcelRpt_ApplicationQuit (CAObjHandle applicationHandle);

HRESULT CVIFUNC ExcelRpt_RunMacro (CAObjHandle applicationHandle, const char *macroName);

HRESULT CVIFUNC_C ExcelRpt_SetApplicationAttribute (CAObjHandle applicationHandle, int attribute, ...);

HRESULT CVIFUNC ExcelRpt_GetApplicationAttribute (CAObjHandle applicationHandle, int attribute,
												  void *attributeValue);

HRESULT CVIFUNC ExcelRpt_WorkbookNew (CAObjHandle applicationHandle, CAObjHandle *workbookHandle);

HRESULT CVIFUNC ExcelRpt_WorkbookOpen (CAObjHandle applicationHandle, const char *fileName,
									   CAObjHandle *workbookHandle);

HRESULT CVIFUNC ExcelRpt_WorkbookClose (CAObjHandle workbookHandle, int saveChanges);

HRESULT CVIFUNC ExcelRpt_WorkbookSave (CAObjHandle workbookHandle, const char *fileName,
									   enum ExREnum_ExFileFormat format);

HRESULT CVIFUNC ExcelRpt_SetMinimumMonitor (CAObjHandle workbookHandle, long resolution, long pixelsPerInch);

HRESULT CVIFUNC ExcelRpt_AllowVMLSupport (CAObjHandle workbookHandle, int value);

HRESULT CVIFUNC ExcelRpt_AllowCSSSupport (CAObjHandle workbookHandle, int value);

HRESULT CVIFUNC ExcelRpt_AllowPNGSupport (CAObjHandle workbookHandle, int value);

HRESULT CVIFUNC ExcelRpt_SetEncoding (CAObjHandle workbookHandle, int encoding);

HRESULT CVIFUNC ExcelRpt_GetWorksheetFromIndex (CAObjHandle workbookHandle, int Index,
												CAObjHandle *worksheetHandle);

HRESULT CVIFUNC ExcelRpt_GetChartsheetFromIndex (CAObjHandle workbookHandle, int Index,
												 CAObjHandle *chartsheetHandle);

HRESULT CVIFUNC ExcelRpt_GetChartsheetFromName (CAObjHandle workbookHandle, const char *chartName,
											 	CAObjHandle *chartsheetHandle);

HRESULT CVIFUNC ExcelRpt_GetWorksheetFromName (CAObjHandle workbookHandle, const char *worksheetName,
											   CAObjHandle *worksheetHandle);

HRESULT CVIFUNC ExcelRpt_GetSheetTypeFromIndex (CAObjHandle workbookHandle, int Index, enum ExREnum_ExSheetType *sheetType);

HRESULT CVIFUNC ExcelRpt_GetWorkbookAttribute (CAObjHandle workbookHandle, int attribute, void *attributeValue);

HRESULT CVIFUNC ExcelRpt_WorksheetNew (CAObjHandle workbookHandle, int insertBeforeIndex,
									   CAObjHandle *worksheetHandle);

HRESULT CVIFUNC ExcelRpt_WorksheetDelete (CAObjHandle worksheetHandle);

HRESULT CVIFUNC ExcelRpt_InsertRow (CAObjHandle worksheetHandle, int insertBeforeRowIndex, int numRows);

HRESULT CVIFUNC ExcelRpt_InsertColumn (CAObjHandle worksheetHandle, int insertBeforeColumnIndex, int numColumns);

HRESULT CVIFUNC ExcelRpt_DeleteRow (CAObjHandle worksheetHandle, int rowIndex, int numRows);

HRESULT CVIFUNC ExcelRpt_DeleteColumn (CAObjHandle worksheetHandle, int columnIndex, int numColumns);

HRESULT CVIFUNC ExcelRpt_InsertPicture (CAObjHandle worksheetHandle, const char *fileName, int top , int left,
										int width, int hight);

HRESULT CVIFUNC ExcelRpt_ActivateWorksheet (CAObjHandle worksheetHandle);

HRESULT CVIFUNC ExcelRpt_Find (CAObjHandle worksheetHandle, enum ExREnum_ExDataType dataType, const void *data,
							   const char *afterCell, enum ExREnum_ExFindLookIn lookIn,
							   enum ExREnum_ExLookAt lookAt, enum ExREnum_ExSearchOrder searchOrder,
							   enum ExREnum_ExSearchDirection searchDirection, int matchCase,
							   int matchByte, char foundRange[]);

HRESULT CVIFUNC ExcelRpt_GetEmbeddedChartFromIndex (CAObjHandle worksheetHandle, int Index,
													CAObjHandle *chartHandle);

HRESULT CVIFUNC ExcelRpt_GetEmbeddedChartFromName (CAObjHandle worksheetHandle, const char *chartName,
												   CAObjHandle *chartHandle);

HRESULT CVIFUNC_C ExcelRpt_SetWorksheetAttribute (CAObjHandle worksheetHandle, int attribute, ...);

HRESULT CVIFUNC ExcelRpt_GetWorksheetAttribute (CAObjHandle worksheetHandle, int attribute, void *attributeValue);

HRESULT CVIFUNC ExcelRpt_RangeSelect (CAObjHandle worksheetHandle, const char *cellRange);

HRESULT CVIFUNC ExcelRpt_RangeBorder (CAObjHandle worksheetHandle, const char *cellRange,
									  enum ExREnum_ExLineStyle Linestyle, long color,
									  enum ExREnum_ExBorderWeight weight, unsigned char edges);

HRESULT CVIFUNC ExcelRpt_AddHyperlink (CAObjHandle worksheetHandle, const char *cellRange, const char *address,
									   const char *screenTip);

HRESULT CVIFUNC ExcelRpt_RangePrint (CAObjHandle worksheetHandle, const char *cellRange, int displayDialog);

HRESULT CVIFUNC ExcelRpt_Fill (CAObjHandle worksheetHandle, enum ExREnum_ExFillOptions direction,
							   const char *cellRange);

HRESULT CVIFUNC ExcelRpt_Sort (CAObjHandle worksheetHandle, const char *cellRange, const char *key1,
							   enum ExREnum_ExSortOrder sortOrder1, const char *key2,
							   enum ExREnum_ExSortOrder sortOrder2, enum ExREnum_ExSortOrientation sortOrientation,
							   enum ExREnum_ExYesNo header);

HRESULT CVIFUNC ExcelRpt_RangeInsert (CAObjHandle worksheetHandle, const char *cellRange,
									  enum ExREnum_ExInsertShiftDirection shiftDirection);

HRESULT CVIFUNC ExcelRpt_RangeDelete (CAObjHandle worksheetHandle, const char *cellRange,
									  enum ExREnum_ExDeleteShiftDirection shiftDirection);

HRESULT CVIFUNC ExcelRpt_Show (CAObjHandle worksheetHandle, const char *cellRange);

HRESULT CVIFUNC ExcelRpt_WriteData (CAObjHandle worksheetHandle, const char *cellRange,
									enum ExREnum_ExDataType dataType, size_t dim1, size_t dim2, const void *data);

HRESULT CVIFUNC ExcelRpt_ReadData (CAObjHandle worksheetHandle, const char *cellRange,
								   enum ExREnum_ExDataType dataType, void *data);

HRESULT CVIFUNC ExcelRpt_FreeStringData (char **data, size_t count);

HRESULT CVIFUNC ExcelRpt_GetNumCellsInRange (CAObjHandle worksheetHandle, const char *cellRange, int *numCells);

HRESULT CVIFUNC ExcelRpt_GetRangeDataType (CAObjHandle worksheetHandle, const char *cellRange,
										   enum ExREnum_ExDataType *dataType);

HRESULT CVIFUNC ExcelRpt_WriteDataFromTableControl (CAObjHandle worksheetHandle, const char *cellRange, int panel,
													int control);

HRESULT CVIFUNC ExcelRpt_ReadDataToTableControl (CAObjHandle worksheetHandle, const char *cellRange,
												 Point startpoint, enum ExREnum_ExInsertMode insertmode, int panel,
												 int control);

HRESULT CVIFUNC ExcelRpt_GetCellDataType (CAObjHandle worksheetHandle, const char *cellRange,
										  enum ExREnum_ExDataType *dataType);

HRESULT CVIFUNC ExcelRpt_GetCellValueStringLen (CAObjHandle worksheetHandle, const char *cellRange, int *length);

HRESULT CVIFUNC ExcelRpt_GetCellValue (CAObjHandle worksheetHandle, const char *cellRange,
									   enum ExREnum_ExDataType dataType, void *dataValue);

HRESULT CVIFUNC_C ExcelRpt_SetCellValue (CAObjHandle worksheetHandle, const char *cellRange,
									   enum ExREnum_ExDataType dataType, ...);

HRESULT CVIFUNC_C ExcelRpt_SetCellRangeAttribute (CAObjHandle worksheetHandle, const char* cellRange,
												int attribute, ...);

HRESULT CVIFUNC ExcelRpt_GetCellRangeAttribute (CAObjHandle worksheetHandle, const char* cellRange, int attribute,
												void* attributeValue);

HRESULT CVIFUNC ExcelRpt_ChartNew (CAObjHandle workbookHandle, int insertBeforeSheetIndex, CAObjHandle *chartHandle);

HRESULT CVIFUNC ExcelRpt_ChartAddtoWorksheet (CAObjHandle worksheetHandle, double left, double top, double width,
											  double height, CAObjHandle *chartHandle);

HRESULT CVIFUNC ExcelRpt_ChartDelete (CAObjHandle chartHandle) ;

HRESULT CVIFUNC ExcelRpt_ChartPrint (CAObjHandle chartHandle, int displayPrintDialog);

HRESULT CVIFUNC ExcelRpt_ChartActivate (CAObjHandle chartHandle);

HRESULT CVIFUNC ExcelRpt_ChartSetSourceRange (CAObjHandle chartHandle, CAObjHandle worksheetHandle,
											  const char *cellRange);

HRESULT CVIFUNC ExcelRpt_ChartWizard (CAObjHandle chartHandle, CAObjHandle worksheetHandle, const char *sourceRange,
									  enum ExREnum_ExChartWizardGallery chartGallery, int format, enum ExREnum_ExRowCol plotBy,
									  int categoryLabels, int seriesLabels, int hasLegend, const char *title,
									  const char *categoryAxisTiltle, const char *valueAxisTitle,
									  const char *extraAxisTitle);

HRESULT CVIFUNC_C ExcelRpt_SetChartAttribute (CAObjHandle chartHandle, int attribute,...);

HRESULT CVIFUNC ExcelRpt_GetChartAttribute (CAObjHandle chartHandle, int attribute, void *attributeValue);

HRESULT CVIFUNC_C ExcelRpt_SetChartAxisAttribute (CAObjHandle chartHandle, enum ExREnum_ExAxisType axisType,
												enum ExREnum_ExAxisGroup axisGroup,  int attribute, ...);

HRESULT CVIFUNC ExcelRpt_GetChartAxisAttribute (CAObjHandle chartHandle, enum ExREnum_ExAxisType axisType,
												enum ExREnum_ExAxisGroup axisGroup,  int attribute,
												void *attributeValue);

#ifdef  __cplusplus
       }
#endif

#endif   /*_EXCELREPORT_H_*/


