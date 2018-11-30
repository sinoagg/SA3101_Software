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

#define  TABLE                            1
#define  TABLE_TABLE1                     2       /* control type: table, callback function: (none) */
#define  TABLE_COMMANDBUTTON              3       /* control type: command, callback function: COMMANDBUTTON_Back */
#define  TABLE_COMMANDBUTTON_2            4       /* control type: command, callback function: COMMANDBUTTON_2_Callback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK COMMANDBUTTON_2_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK COMMANDBUTTON_Back(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
