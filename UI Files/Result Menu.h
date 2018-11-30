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

#define  CHPANEL                          1
#define  CHPANEL_CHECKBOX                 2       /* control type: radioButton, callback function: ChoseCallback */

#define  REMINDSAVE                       2
#define  REMINDSAVE_TEXTMSG               2       /* control type: textMsg, callback function: (none) */

#define  RESULTMENU                       3
#define  RESULTMENU_TABLE                 2       /* control type: picture, callback function: TableCallback */
#define  RESULTMENU_SAVE                  3       /* control type: picture, callback function: SaveDataCallback */
#define  RESULTMENU_GRAPH                 4       /* control type: picture, callback function: GraphCallback */

#define  SAVEDATA                         4       /* callback function: SaveDataPanelCallbck */
#define  SAVEDATA_SHEETPATH               2       /* control type: string, callback function: (none) */
#define  SAVEDATA_GRAPH1PATH              3       /* control type: string, callback function: (none) */
#define  SAVEDATA_GRAPH2PATH              4       /* control type: string, callback function: (none) */
#define  SAVEDATA_TEXTMSG                 5       /* control type: textMsg, callback function: (none) */
#define  SAVEDATA_TEXTMSG_3               6       /* control type: textMsg, callback function: (none) */
#define  SAVEDATA_TEXTMSG_2               7       /* control type: textMsg, callback function: (none) */
#define  SAVEDATA_SAVEGRAPH2              8       /* control type: command, callback function: SaveGraph2Callback */
#define  SAVEDATA_COMMANDBUTTON_2         9       /* control type: command, callback function: SaveGraph1Callback */
#define  SAVEDATA_COMMANDBUTTON           10      /* control type: command, callback function: SaveSheetCallback */
#define  SAVEDATA_COMMANDBUTTON_4         11      /* control type: command, callback function: SaveAllCallback */
#define  SAVEDATA_COMMANDBUTTON_5         12      /* control type: command, callback function: ExitCallback */
#define  SAVEDATA_COMMANDBUTTON_8         13      /* control type: command, callback function: BrowseSheetCallback */
#define  SAVEDATA_COMMANDBUTTON_7         14      /* control type: command, callback function: BrowseGraph1Callback */
#define  SAVEDATA_BROWSEGRAPH2            15      /* control type: command, callback function: BrowseGraph2Callback */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_UNNAMEDM                 2


     /* Callback Prototypes: */

int  CVICALLBACK BrowseGraph1Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BrowseGraph2Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BrowseSheetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ChoseCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GraphCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveAllCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveDataCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveDataPanelCallbck(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveGraph1Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveGraph2Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveSheetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TableCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
