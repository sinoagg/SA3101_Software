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

#define  PANEL_I_T                        1
#define  PANEL_I_T_SMU1MODE               2       /* control type: ring, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_START1                 3       /* control type: numeric, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_SMU2MODE               4       /* control type: ring, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_START2                 5       /* control type: numeric, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_DECORATION_SMU1        6       /* control type: deco, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_START_1                7       /* control type: textMsg, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_PICTURE_2              8       /* control type: picture, callback function: (none) */
#define  PANEL_I_T_PICTURE_4              9       /* control type: picture, callback function: (none) */
#define  PANEL_I_T_ST_TEXT_3              10      /* control type: textMsg, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_ST_TEXT_1              11      /* control type: textMsg, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_START_2                12      /* control type: textMsg, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_DECORATION_SMU2        13      /* control type: deco, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_TXT_SMU2               14      /* control type: textMsg, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_TXT_SMU1               15      /* control type: textMsg, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_TEXTMSG                16      /* control type: textMsg, callback function: (none) */
#define  PANEL_I_T_BG_SMU1                17      /* control type: canvas, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_PICTURE                18      /* control type: picture, callback function: (none) */
#define  PANEL_I_T_PICTURE_6              19      /* control type: picture, callback function: (none) */
#define  PANEL_I_T_TEXTMSG_2              20      /* control type: textMsg, callback function: ITSMU1DecoCallback */
#define  PANEL_I_T_TEXTMSG_3              21      /* control type: textMsg, callback function: ITSMU2DecoCallback */
#define  PANEL_I_T_BG_SMU2                22      /* control type: canvas, callback function: ITSMU2DecoCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ITSMU1DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ITSMU2DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
