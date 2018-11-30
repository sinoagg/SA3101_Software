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

#define  PANEL_V_T                        1
#define  PANEL_V_T_SMU1MODE               2       /* control type: ring, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_START1                 3       /* control type: numeric, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_VTSTART2UNIT           4       /* control type: ring, callback function: VtSmu2Callback */
#define  PANEL_V_T_VTSTART1UNIT           5       /* control type: ring, callback function: VtSmu1Callback */
#define  PANEL_V_T_SMU2MODE               6       /* control type: ring, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_START2                 7       /* control type: numeric, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_DECORATION_SMU2        8       /* control type: deco, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_START_1                9       /* control type: textMsg, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_PICTURE_4              10      /* control type: picture, callback function: (none) */
#define  PANEL_V_T_PICTURE_2              11      /* control type: picture, callback function: (none) */
#define  PANEL_V_T_ST_TEXT_3              12      /* control type: textMsg, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_ST_TEXT_1              13      /* control type: textMsg, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_START_2                14      /* control type: textMsg, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_DECORATION_SMU1        15      /* control type: deco, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_TXT_SMU2               16      /* control type: textMsg, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_TXT_SMU1               17      /* control type: textMsg, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_BG_SMU2                18      /* control type: canvas, callback function: VTSMU2DecoCallback */
#define  PANEL_V_T_TEXTMSG                19      /* control type: textMsg, callback function: (none) */
#define  PANEL_V_T_BG_SMU1                20      /* control type: canvas, callback function: VTSMU1DecoCallback */
#define  PANEL_V_T_PICTURE_6              21      /* control type: picture, callback function: (none) */
#define  PANEL_V_T_PICTURE_7              22      /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK VtSmu1Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VTSMU1DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VtSmu2Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VTSMU2DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
