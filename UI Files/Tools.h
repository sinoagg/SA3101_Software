/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CALIPANEL                        1       /* callback function: CaliPanelCallback */
#define  CALIPANEL_TEXTMSG                2       /* control type: textMsg, callback function: (none) */
#define  CALIPANEL_RANGESELECT            3       /* control type: ring, callback function: RangeSelectCallback */
#define  CALIPANEL_TEXTMSG_2              4       /* control type: textMsg, callback function: (none) */
#define  CALIPANEL_EXIT                   5       /* control type: textMsg, callback function: ExitCaliCallback */
#define  CALIPANEL_OK_PIC_2               6       /* control type: picture, callback function: ExitCaliCallback */
#define  CALIPANEL_SAVE                   7       /* control type: textMsg, callback function: SaveCaliCallback */
#define  CALIPANEL_OK_PIC                 8       /* control type: picture, callback function: SaveCaliCallback */
#define  CALIPANEL_OUTVOLCALI             9       /* control type: textMsg, callback function: OutputVoltageCaliCallback */
#define  CALIPANEL_CURRENTCALI            10      /* control type: textMsg, callback function: CurrentCaliCallback */
#define  CALIPANEL_ZEROCURCALI            11      /* control type: textMsg, callback function: ZeroCurrentCaliCallback */

#define  CHECKPORT                        2       /* callback function: CheckPanelCallback */
#define  CHECKPORT_MANUAL                 2       /* control type: command, callback function: ManualCallback */
#define  CHECKPORT_AUTOCHECK              3       /* control type: command, callback function: AutoCheckCallback */

#define  READPORT                         3       /* callback function: ReadPanelCallback */
#define  READPORT_TEXTMSG                 2       /* control type: textMsg, callback function: (none) */
#define  READPORT_CHECK_2                 3       /* control type: command, callback function: ReadCtrlPortCallback */
#define  READPORT_CHECK                   4       /* control type: command, callback function: ReadMeasurePortCallback */
#define  READPORT_CONTROLPORT             5       /* control type: numeric, callback function: (none) */
#define  READPORT_MEASUREPORT             6       /* control type: numeric, callback function: (none) */
#define  READPORT_TEXTMSG_2               7       /* control type: textMsg, callback function: (none) */

#define  TOOLSPANEL                       4       /* callback function: ToolsPanelCallback */
#define  TOOLSPANEL_PICTURE               2       /* control type: picture, callback function: CalibrationCallback */
#define  TOOLSPANEL_CALIBRATION           3       /* control type: textMsg, callback function: CalibrationCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AutoCheckCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CalibrationCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CaliPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CheckPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CurrentCaliCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitCaliCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OutputVoltageCaliCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RangeSelectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadCtrlPortCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadMeasurePortCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveCaliCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ToolsPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ZeroCurrentCaliCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
