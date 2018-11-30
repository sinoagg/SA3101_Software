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

#define  ENVIRONMEN                       1
#define  ENVIRONMEN_PRESSURE              2       /* control type: radioButton, callback function: PressCallback */
#define  ENVIRONMEN_HUMIDITY              3       /* control type: radioButton, callback function: HumidityCallback */
#define  ENVIRONMEN_TEMPTER               4       /* control type: radioButton, callback function: TempCallback */
#define  ENVIRONMEN_MEASURE               5       /* control type: radioButton, callback function: MesaureCallback */
#define  ENVIRONMEN_TEX_MEAS              6       /* control type: textMsg, callback function: (none) */
#define  ENVIRONMEN_TXT_HUM               7       /* control type: textMsg, callback function: (none) */
#define  ENVIRONMEN_TXT_TEMP              8       /* control type: textMsg, callback function: (none) */
#define  ENVIRONMEN_TXT_PRESS             9       /* control type: textMsg, callback function: (none) */

#define  ENVIRPANEL                       2
#define  ENVIRPANEL_TXT_HUM               2       /* control type: textMsg, callback function: (none) */
#define  ENVIRPANEL_TXT_PRESS             3       /* control type: textMsg, callback function: (none) */
#define  ENVIRPANEL_TEXTMSG_2             4       /* control type: textMsg, callback function: (none) */
#define  ENVIRPANEL_TEXTMSG               5       /* control type: textMsg, callback function: (none) */
#define  ENVIRPANEL_CHECKBOX_3            6       /* control type: radioButton, callback function: Pressure_displayCB */
#define  ENVIRPANEL_CHECKBOX_2            7       /* control type: radioButton, callback function: Humidity_displayCB */
#define  ENVIRPANEL_CHECKBOX              8       /* control type: radioButton, callback function: Temp_displayCB */
#define  ENVIRPANEL_STR_PRES              9       /* control type: numeric, callback function: (none) */
#define  ENVIRPANEL_STR_HUM               10      /* control type: numeric, callback function: (none) */
#define  ENVIRPANEL_STR_TEMP              11      /* control type: numeric, callback function: (none) */
#define  ENVIRPANEL_TEXTMSG_3             12      /* control type: textMsg, callback function: (none) */
#define  ENVIRPANEL_LEGENDCHECK           13      /* control type: radioButton, callback function: Legend_display_CB */
#define  ENVIRPANEL_TXT_TEMP              14      /* control type: textMsg, callback function: (none) */
#define  ENVIRPANEL_TXT_LEGEND            15      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Humidity_displayCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HumidityCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Legend_display_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MesaureCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PressCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Pressure_displayCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Temp_displayCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TempCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
