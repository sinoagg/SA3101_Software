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

#define  PANEL_R_T                        1
#define  PANEL_R_T_SMU1MODE               2       /* control type: ring, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_START1                 3       /* control type: numeric, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_SMU2MODE               4       /* control type: ring, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_START2                 5       /* control type: numeric, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_START_1                6       /* control type: textMsg, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_PICTURE_4              7       /* control type: picture, callback function: (none) */
#define  PANEL_R_T_PICTURE_2              8       /* control type: picture, callback function: (none) */
#define  PANEL_R_T_ST_TEXT_3              9       /* control type: textMsg, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_ST_TEXT_1              10      /* control type: textMsg, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_START_2                11      /* control type: textMsg, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_DECORATION_SMU1        12      /* control type: deco, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_DECORATION_SMU2        13      /* control type: deco, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_TXT_SMU2               14      /* control type: textMsg, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_TXT_SMU1               15      /* control type: textMsg, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_BG_SMU2                16      /* control type: canvas, callback function: RTSMU2DecoCallback */
#define  PANEL_R_T_TEXTMSG                17      /* control type: textMsg, callback function: (none) */
#define  PANEL_R_T_BG_SMU1                18      /* control type: canvas, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_PICTURE_6              19      /* control type: picture, callback function: (none) */
#define  PANEL_R_T_PICTURE_7              20      /* control type: picture, callback function: (none) */
#define  PANEL_R_T_TEXTMSG_3              21      /* control type: textMsg, callback function: RTSMU1DecoCallback */
#define  PANEL_R_T_TEXTMSG_2              22      /* control type: textMsg, callback function: RTSMU2DecoCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK RTSMU1DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RTSMU2DecoCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
