#include <utility.h>
#include <cvirte.h>		
#include "LoadPanel.h"
#include "Uart.h"
#include "main.h" 
/*

	��Ϊ��ʵʱ�������汾

*/
int main (int argc, char *argv[])
{
	int SA3102_UART_Status=0;
	int abnmDCThreadId; 
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, AbnmDCThreadFunction, NULL, &abnmDCThreadId);					//�����µ��߳�
	ReadSerialPort(&measureComPort,&controlComPort);
	//DeviceQuery(&measureComPort,&controlComPort);																				//�Զ���ѯ�豸���ں�
	SA3102_UART_Status = CheckOpenCom(&measureComPort,&controlComPort,&measure_Uart_Flag,&control_Uart_Flag);					//�򿪼�鵽�Ĵ��ں�
	LoadInitPanel();
	//*********************************************//
		if(controlComPort==0)
		{							
			SetCtrlAttribute (hEnvCfgPanel,  ENVIRONMEN_MEASURE, ATTR_DIMMED, 1);												//���û���ѡ��ť��û�м�⵽�����Ĵ��ںžͽ��û���������ť��
		}
	//*********************************************//
	PrintEdition(USART_RX_Buffer1);																								//��ӡ�汾��
	RunUserInterface();
	CheckCloseCom(SA3102_UART_Status,measureComPort,controlComPort);//�رմ򿪵Ĵ��ں�
	
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,abnmDCThreadId,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);		//�ȴ��̺߳������ִ��  
	CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, abnmDCThreadId);
	QuitUserInterface(0); 	  
	return 0;
}

