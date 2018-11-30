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

#define  SAMPLE_ADV                       1
#define  SAMPLE_ADV_MINRANGE              2       /* control type: ring, callback function: (none) */
#define  SAMPLE_ADV_MAXRANGE              3       /* control type: ring, callback function: (none) */
#define  SAMPLE_ADV_TEXTMSG               4       /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_SAMPLERATE            5       /* control type: numeric, callback function: (none) */
#define  SAMPLE_ADV_SAMPLENUMBER          6       /* control type: numeric, callback function: (none) */
#define  SAMPLE_ADV_TEXTMSG_2             7       /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_BSCSET                8       /* control type: textMsg, callback function: BasicSetCallback */
#define  SAMPLE_ADV_OutputRelay           9       /* control type: radioButton, callback function: (none) */
#define  SAMPLE_ADV_TEXTMSG_3             10      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_RING_2                11      /* control type: ring, callback function: (none) */
#define  SAMPLE_ADV_TEXTMSG_6             12      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_CURRENTMODE           13      /* control type: ring, callback function: (none) */
#define  SAMPLE_ADV_TEXTMSG_7             14      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_TEXTMSG_12            15      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_LINEAR                16      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_DIRECTION             17      /* control type: textMsg, callback function: (none) */

#define  SAMPLE_CFG                       2
#define  SAMPLE_CFG_SAMPLE_DELAY          2       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_QUIETTIME             3       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_TIMESTEP              4       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_RANGESETTING          5       /* control type: ring, callback function: (none) */
#define  SAMPLE_CFG_RUNTIME               6       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_ADVCSET               7       /* control type: textMsg, callback function: AdvancSetCallback */
#define  SAMPLE_CFG_TXT_RUNTIME           8       /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TXT_S                 9       /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG_3             10      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG_5             11      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG_4             12      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG_14            13      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_SAMPLEDELAY           14      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_SWEEOSPEED            15      /* control type: ring, callback function: RampleRateCallback */
#define  SAMPLE_CFG_INTERVAL              16      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG               17      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG_11            18      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AdvancSetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BasicSetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RampleRateCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
