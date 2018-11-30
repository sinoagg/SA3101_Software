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

#define  DEFPANEL                         1       /* callback function: PrjSelectCallback */
#define  DEFPANEL_NAME                    2       /* control type: textMsg, callback function: (none) */
#define  DEFPANEL_CANVAS                  3       /* control type: canvas, callback function: (none) */
#define  DEFPANEL_DESC                    4       /* control type: textMsg, callback function: (none) */
#define  DEFPANEL_PICS                    5       /* control type: picture, callback function: (none) */
#define  DEFPANEL_TIME                    6       /* control type: textMsg, callback function: (none) */
#define  DEFPANEL_DATE                    7       /* control type: textMsg, callback function: (none) */

#define  PROJECT                          2

#define  PROPANEL                         3       /* callback function: PIC_ExitPrjCallback */
#define  PROPANEL_STRING                  2       /* control type: string, callback function: (none) */
#define  PROPANEL_SEARCH                  3       /* control type: command, callback function: SearchCallback */
#define  PROPANEL_PIC_EXIT                4       /* control type: picture, callback function: PIC_ExitPrjCallback2 */
#define  PROPANEL_TXT_EXIT                5       /* control type: textMsg, callback function: PIC_ExitPrjCallback2 */
#define  PROPANEL_TXT_OPENPRJ             6       /* control type: textMsg, callback function: TXT_OpenPrjCallback */
#define  PROPANEL_PIC_OPENPRJ             7       /* control type: picture, callback function: TXT_OpenPrjCallback */
#define  PROPANEL_CANVAS_2                8       /* control type: canvas, callback function: (none) */
#define  PROPANEL_CANVAS                  9       /* control type: canvas, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK PIC_ExitPrjCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PIC_ExitPrjCallback2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PrjSelectCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SearchCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TXT_OpenPrjCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
