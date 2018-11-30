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

#define  PANEL_V_I                        1
#define  PANEL_V_I_SMU1MODE               2       /* control type: ring, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_START1                 3       /* control type: numeric, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_START1UNIT             4       /* control type: ring, callback function: Start1UnitCB */
#define  PANEL_V_I_STOP1                  5       /* control type: numeric, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_GAP1                   6       /* control type: numeric, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_SMU2MODE               7       /* control type: ring, callback function: VISMU2DecoCallback */
#define  PANEL_V_I_START2                 8       /* control type: numeric, callback function: VISMU2DecoCallback */
#define  PANEL_V_I_START2UNIT             9       /* control type: ring, callback function: Start2UnitCB */
#define  PANEL_V_I_STOP2                  10      /* control type: numeric, callback function: VISMU2DecoCallback */
#define  PANEL_V_I_GAP2                   11      /* control type: numeric, callback function: VISMU2DecoCallback */
#define  PANEL_V_I_STOP_1                 12      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_STEP_1                 13      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_START_1                14      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_PICTURE_4              15      /* control type: picture, callback function: (none) */
#define  PANEL_V_I_PICTURE_2              16      /* control type: picture, callback function: (none) */
#define  PANEL_V_I_ST_TEXT_2              17      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_ST_TEXT_1              18      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_START_2                19      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_STOP_2                 20      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_DECORATION_SMU2        21      /* control type: deco, callback function: VISMU2DecoCallback */
#define  PANEL_V_I_TXT_SMU2               22      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_TXT_SMU1               23      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_BG_SMU1                24      /* control type: canvas, callback function: (none) */
#define  PANEL_V_I_DECORATION_SMU1        25      /* control type: deco, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_BG_SMU2                26      /* control type: canvas, callback function: (none) */
#define  PANEL_V_I_TEXTMSG                27      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_PICTURE_7              28      /* control type: picture, callback function: (none) */
#define  PANEL_V_I_PICTURE_6              29      /* control type: picture, callback function: (none) */
#define  PANEL_V_I_STEP_2                 30      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_I_GAP1UNIT               31      /* control type: textMsg, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_STOP2UNIT              32      /* control type: textMsg, callback function: VISMU2DecoCallback */
#define  PANEL_V_I_STOP1UNIT              33      /* control type: textMsg, callback function: VISMU1DecoCallback */
#define  PANEL_V_I_GAP2UNIT               34      /* control type: textMsg, callback function: VISMU2DecoCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Start1UnitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start2UnitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VISMU1DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VISMU2DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
