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
#define  SAMPLE_ADV_SAMPLERATE            4       /* control type: numeric, callback function: (none) */
#define  SAMPLE_ADV_SAMPLENUMBER          5       /* control type: numeric, callback function: (none) */
#define  SAMPLE_ADV_BSCSET                6       /* control type: textMsg, callback function: BasicSetCallback */
#define  SAMPLE_ADV_OutputRelay           7       /* control type: radioButton, callback function: (none) */
#define  SAMPLE_ADV_RING_2                8       /* control type: ring, callback function: (none) */
#define  SAMPLE_ADV_CURRENTMODE           9       /* control type: ring, callback function: (none) */
#define  SAMPLE_ADV_MAXRANGE_B            10      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_LINEAR_B              11      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_SAMPLERATE_B          12      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_SAMPLENUMBER_B        13      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_OUTPUTRELAY_B         14      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_DIRECTION_B           15      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_MINRANGE_B            16      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_ADV_SAMPLERATEHZ_T        17      /* control type: textMsg, callback function: (none) */

#define  SAMPLE_CFG                       2
#define  SAMPLE_CFG_SAMPLE_DELAY          2       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_QUIETTIME             3       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_TIMESTEP              4       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_RANGESETTING          5       /* control type: ring, callback function: (none) */
#define  SAMPLE_CFG_RUNTIME               6       /* control type: numeric, callback function: (none) */
#define  SAMPLE_CFG_ADVCSET               7       /* control type: textMsg, callback function: AdvancSetCallback */
#define  SAMPLE_CFG_SWEEOSPEED            8       /* control type: ring, callback function: RampleRateCallback */
#define  SAMPLE_CFG_RUNTIME_B             9       /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TIMTINTERVAL_B        10      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TIMTINTERVAL_T        11      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_RUNTIME_T             12      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_TEXTMSG               13      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_QUIETTIME_T           14      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_SAMPLEDELAY_B         15      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_RANGESETTINT_B        16      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_QUIETTIME_B           17      /* control type: textMsg, callback function: (none) */
#define  SAMPLE_CFG_SAMPLERATE_B          18      /* control type: textMsg, callback function: (none) */


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
