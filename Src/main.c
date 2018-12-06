#include <utility.h>
#include <cvirte.h>		
#include "LoadPanel.h"
#include "Uart.h"
#include "main.h" 
/*

	改为真实时间的软件版本

*/
int main (int argc, char *argv[])
{
	int SA3102_UART_Status=0;
	int abnmDCThreadId; 
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, AbnmDCThreadFunction, NULL, &abnmDCThreadId);					//开辟新的线程
	ReadSerialPort(&measureComPort,&controlComPort);
	//DeviceQuery(&measureComPort,&controlComPort);																				//自动查询设备串口号
	SA3102_UART_Status = CheckOpenCom(&measureComPort,&controlComPort,&measure_Uart_Flag,&control_Uart_Flag);					//打开检查到的串口号
	LoadInitPanel();
	//*********************************************//
		if(controlComPort==0)
		{							
			SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_MEASURE, ATTR_DIMMED, 1);												//禁用环境选择按钮，没有检测到环境的串口号就禁用环境测量按钮。
		}
	//*********************************************//
	PrintEdition(USART_RX_Buffer1);																								//打印版本号
	RunUserInterface();
	CheckCloseCom(SA3102_UART_Status,measureComPort,controlComPort);//关闭打开的串口号
	
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,abnmDCThreadId,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);		//等待线程函数完成执行  
	CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, abnmDCThreadId);
	QuitUserInterface(0); 	  
	return 0;
}

