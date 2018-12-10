#include "MainPanel.h"
#include "asynctmr.h"
#include "SetPanel.h"
#include "inifile.h"
#include <ansi_c.h>
#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include "main.h"
#include "Uart.h"
#include "MainPanelCb.h" 
#include "Table.h"
#include "LoadPanel.h" 
#include "Cgs_mt.h"
#include "Plot.h"
//==============================================================================
//
// Title:		Uart.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/9/27 at 11:44:29 by �� ��.
// Copyright:	�㽭���ȿƼ���չ���޹�˾. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//#include "Uart.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
RxDataTypeDef RxData1,RxData2;
Rx_CGS_DataTypeDef Rx_CGS_Data;
int logFlag;
int rowIndex = 2;
int rowIndex2 = 2;
int rowIndex3 = 2;
static char dirName[500];
static char pathName[500]; 
static IniText serialPortIni; 
char USART_RX_Buffer1[20];   //���յ��İ汾��
int stop_Flag = 0; 
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
void SubString(char* str1,char* str2,int start,int length)
{
	int i;
	for(i = start; i< start + length; i++ )
	{
		str1[i-start] = str2[i];
	}
}
void PrintEdition(char USART_RX_Buffer1[20])
{
	char Hard_version[6] = {"123"};
	char Firm_version[6] = {"456"};
	char Soft_version[6] = {"789"};
	SubString(Hard_version,USART_RX_Buffer1,6,3);
	SubString(Firm_version,USART_RX_Buffer1,9,3);
	SubString(Soft_version,USART_RX_Buffer1,12,3);
  	SetCtrlVal (aboutPanel, ABOUT_HARDWAREVERSION, Hard_version); 
  	SetCtrlVal (aboutPanel, ABOUT_FIRMWAREVERSION, Firm_version); 
  	SetCtrlVal (aboutPanel, ABOUT_SOFTWAREVERSION, Soft_version); 
}
static int CheckPortStatus(unsigned char portNumber, unsigned char uartRxLen, void (*pFunc)(int, int, void*))
{
	int status;
	status = OpenComConfig(portNumber, "", 115200, 0, 8, 1, 1024, 1024);	   			//Config and Connect serial port
	if(status != 0) 
	{
		MessagePopup("Error","Device unconnected.");
		return -1;
	}
	else
	{
		InstallComCallback (portNumber, LWRS_RECEIVE, uartRxLen, 0, pFunc, 0);   		//binding Callback function to serial input data		18 bytes received will calling for an interrupt
		SetCTSMode(portNumber, LWRS_HWHANDSHAKE_OFF);
		FlushInQ(portNumber);	   														//Clear input and output buffer
		FlushOutQ(portNumber);
		return 0;
	}
}
void CheckCloseCom(int SA3102_UART_Status,unsigned char measureComPort,unsigned char controlComPort)
{
	if(SA3102_UART_Status==1)				//��������豸�򿪵Ļ����ر����ʱ��Ҫ�رն�Ӧ�Ĵ��ڡ��û���ʹ�û��������豸ʱ��û�д򿪣����ԾͲ��ùرա�
	{
		CloseCom(measureComPort);
	}
	if(SA3102_UART_Status==2)				//�����ѧ�豸�򿪵Ļ����ر����ʱ��Ҫ�رն�Ӧ�Ĵ��ڡ��û���ʹ�õ�ѧ�����豸ʱ��û�д򿪣����ԾͲ��ùرա�
	{
		CloseCom(controlComPort);
	}
	if(SA3102_UART_Status==3)				//�����ѧ�ͻ����Ĳ����豸���򿪵Ļ�������ʱ��Ҫͬʱ�ر�
	{
		CloseCom(controlComPort);
		CloseCom(measureComPort);
	}
}
int CheckOpenCom(unsigned char* measureComPort,unsigned char* controlComPort,unsigned char* measure_Uart_Flag,unsigned char* control_Uart_Flag)
{
	int Status = 0; 
	if(CheckPortStatus(*measureComPort, MEASURE_UART_RX_LEN, ComCallback)<0)
	{
		Status=0;															//���ڴ�ʧ��
		*measure_Uart_Flag = 0;
		MessagePopup("Error","Failed to open the serial port of electrical measuring equipment"); 
		//Ҫ����������Ҫ���ô˴��ڵĲ�������
	}
	else 
	{
		Status=1;															//���ڴ򿪳ɹ� 
		*measure_Uart_Flag = 1;
	}
	if(CheckPortStatus(*controlComPort, CONTROL_UART_RX_LEN, CtrlComCallback)<0)						//�жϻ��������豸��Ӧ�Ĵ����Ƿ����ӳɹ�
	{
		*control_Uart_Flag = 0;
		Status = Status + 0;     
		MessagePopup("Error","Failed to open the serial port of the environmental measuring equipment.Please check the communication line"); 
	}
	else
	{
		Status=Status + 2;
		*control_Uart_Flag = 1;
	}
	return Status;
}
//==================��ȡ�����ļ��д��ںŲ�ʶ��=============
void ReadSerialPort(unsigned char* measureComPort,unsigned char* controlComPort)
{
	char IDquery[] = {0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0xFF};
	char IDquery2[] = {0x11, 0x03, 0x00, 0x02, 0x00, 0x05, 0x26, 0x99}; 
	char USART_RX_Buffer[60];		//���յ�ѧ 
	char USART_RX_Buffer2[60];		//���ջ���
	int b, strLen;
	int measurePort;				//��ini�ļ��ж����Ĵ��ں�
	int controlPort;
	GetProjectDir(dirName);
	MakePathname (dirName, "serialPort.ini", pathName);
	if( !(serialPortIni = Ini_New (0)) )
	{
		MessagePopup("Error","There is not enough memory for inifile");
	}
	if( (Ini_ReadFromFile (serialPortIni, pathName)) < 0)
	{
		MessagePopup("Error","Cannot read inifile");
	}
	
	Ini_GetInt (serialPortIni, "SerialPort", "measureComPort", &measurePort);   //��ȡini�ļ��еĵ�ѧ���ں�

	SetBreakOnLibraryErrors (0);
	b = OpenComConfig(measurePort, "", 115200, 0, 8, 1, 50, 50);		 		//���ini�ļ��еĵ�ѧ���ں��Ƿ���ȷ
	if(b<0 )
	{
		MeasureDeviceQuery(measureComPort); 
		Ini_PutInt (serialPortIni, "SerialPort", "measureComPort", ((int)(*measureComPort)));
		Ini_WriteToFile(serialPortIni,pathName);
		Ini_Dispose(serialPortIni);
	}
	else
	{
		ComWrt(measurePort, IDquery, 32);//���Ͳ�ѯָ�� 
		Delay(5);
		strLen = GetInQLen (measurePort);
		ComRd (measurePort,USART_RX_Buffer,strLen);
		Delay(0.5);
		if((USART_RX_Buffer[1]== '3') && (USART_RX_Buffer[2] == '1') && (USART_RX_Buffer[3] == '0') && (USART_RX_Buffer[4]== '2'))  
		{
			*measureComPort = (unsigned char)measurePort;
			
			for(int i=0;i<19;i++)
			{
			USART_RX_Buffer1[i] = USART_RX_Buffer[i+1];
			}
			USART_RX_Buffer1[19] = '\0'; 
			FlushInQ(*measureComPort);	   														//Clear input and output buffer
			FlushOutQ(*measureComPort);
			CloseCom (*measureComPort);
		}
		else
		{
			CloseCom(measurePort);
			MeasureDeviceQuery(measureComPort); 		   //����ѧ����
			Ini_PutInt (serialPortIni, "SerialPort", "measureComPort", ((int)(*measureComPort)));	   //д����ȷ����
			Ini_WriteToFile(serialPortIni,pathName);
			Ini_Dispose(serialPortIni);
		}
		SetCTSMode (measurePort, LWRS_HWHANDSHAKE_OFF);   //��Ӳ������ 
	}
	
/*=============��ȡ�������ں�==================*/
	if( !(serialPortIni = Ini_New (0)) )
	{
		MessagePopup("Error","there is not enough memory for inifile");
	}
	if( (Ini_ReadFromFile (serialPortIni, pathName)) < 0)
	{
		MessagePopup("Error","Cannot read inifile");
	}
	Ini_GetInt (serialPortIni, "SerialPort", "controlComPort", &controlPort);   //��ȡini�ļ��еĻ������ں� 
	b = OpenComConfig(controlPort, "", 115200, 0, 8, 1, 30, 30);
	if(b<0 )
	{
		CtrlDeviceQuery(controlComPort);
		Ini_PutInt (serialPortIni, "SerialPort", "controlComPort", ((int)(*controlComPort)));
		Ini_WriteToFile(serialPortIni,pathName);
		Ini_Dispose(serialPortIni);
	}
	else
	{
		ComWrt(controlPort, IDquery2, 8);//���Ͳ�ѯָ�� 
		Delay(5);
		strLen = GetInQLen (controlPort);
		ComRd (controlPort,USART_RX_Buffer2,strLen);
		Delay(0.5);
		if((USART_RX_Buffer2[0]== 0x11) && (USART_RX_Buffer2[1] == 0x03) && (USART_RX_Buffer2[2] == 0x0a))    
		{
			*controlComPort = (unsigned char)controlPort;
			FlushInQ(*controlComPort);	   														//Clear input and output buffer
			FlushOutQ(*controlComPort);
			CloseCom (*controlComPort);

		}
		else
		{
			CloseCom(controlPort);
			CtrlDeviceQuery(controlComPort);
			Ini_PutInt (serialPortIni, "SerialPort", "controlComPort", ((int)(*controlComPort)));
			Ini_WriteToFile(serialPortIni,pathName);
			Ini_Dispose(serialPortIni);
			
		}
		SetCTSMode (controlPort, LWRS_HWHANDSHAKE_OFF);   //��Ӳ������ 
	}
	serialPortIni=0; 
	
}
void MeasureDeviceQuery(unsigned char* measureComPort)																					//��ѯ�豸���ں�
{
	char IDquery[] = {0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
	unsigned char i;
	char USART_RX_Buffer[60];   //����
	int b, strLen;
	
		for(i = 1; i<=10; i++)
		{
			SetBreakOnLibraryErrors (0);
			b = OpenComConfig(i, "", 115200, 0, 8, 1, 50, 50);											//���úʹ򿪴��� 
		    if(b<0 )
			{
		
			}else
			{	
			    ComWrt(i, IDquery, 32);//���Ͳ�ѯָ��
				Delay(3);
				strLen = GetInQLen (i);
				ComRd (i,USART_RX_Buffer,strLen);
				Delay(0.5);
				if((USART_RX_Buffer[1]== '3') && (USART_RX_Buffer[2] == '1') && (USART_RX_Buffer[3] == '0') && (USART_RX_Buffer[4]== '2'))
				{
					*measureComPort = i;																//�����ѯ������ȷ����
					for(int i=0;i<19;i++)
					{
					USART_RX_Buffer1[i] = USART_RX_Buffer[i+1];
					}
					USART_RX_Buffer1[19] = '\0';
					FlushInQ(*measureComPort);	   														//Clear input and output buffer
					FlushOutQ(*measureComPort);
					CloseCom (*measureComPort);
					break;
				}
				else
				{
					CloseCom(i);
				}
				SetCTSMode (i, LWRS_HWHANDSHAKE_OFF);   //��Ӳ������
			}
		}
	
}
void CtrlDeviceQuery(unsigned char* controlComPort)
{
	char IDquery2[] = {0x11, 0x03, 0x00, 0x02, 0x00, 0x05, 0x26, 0x99};
	char USART_RX_Buffer2[60];   //����
	unsigned char i;
	int b, strLen;
	for(i = 1; i<=10; i++)
		{
			SetBreakOnLibraryErrors (0);
			b = OpenComConfig(i, "", 115200, 0, 8, 1, 30, 30);  //���úʹ򿪴��� 
		    if(b<0 )
			{
		
			}else
			{	
			    ComWrt(i, IDquery2, 8);							//���Ͳ�ѯָ��
				Delay(5);
				strLen = GetInQLen (i);
				ComRd (i,USART_RX_Buffer2,strLen);
				Delay(0.5);
				if((USART_RX_Buffer2[0]== 0x11) && (USART_RX_Buffer2[1] == 0x03) && (USART_RX_Buffer2[2] == 0x0a))
				{
					*controlComPort = i;							
					FlushInQ(*controlComPort);	   				//Clear input and output buffer
					FlushOutQ(*controlComPort);
					CloseCom (*controlComPort);
					break;
				}
				else
				{
					CloseCom(i);
				}
				SetCTSMode (i, LWRS_HWHANDSHAKE_OFF);			//��Ӳ������
			}
		}
}
int  CheckMeasurePort(unsigned char* measureComPort,unsigned char i)	   //�������ĵ�ѧ���ں��Ƿ���ȷ
{
	char IDquery[] = {0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
	char USART_RX_Buffer1[60];   //����
	int b, strLen;
	
	SetBreakOnLibraryErrors (0);
	b = OpenComConfig(i, "", 115200, 0, 8, 1, 50, 50);										//���úʹ򿪴��� 
    if(b<0 )
	{
		MessagePopup("Error","Incorrect serial port!");
		return -1;
	}
	else
	{	
	    ComWrt(i, IDquery, 32);//���Ͳ�ѯָ��
		Delay(5);
		strLen = GetInQLen (i);
		ComRd (i,USART_RX_Buffer1,strLen);
		Delay(0.5); 
		if((USART_RX_Buffer1[1]== '3') && (USART_RX_Buffer1[2] == '1') && (USART_RX_Buffer1[3] == '0') && (USART_RX_Buffer1[4]== '2'))
		{
			*measureComPort = i;
			FlushInQ(*measureComPort);	   														//Clear input and output buffer
			FlushOutQ(*measureComPort);
			CloseCom (*measureComPort);
			return 0;
		}
		else
		{
			CloseCom(i);
			MessagePopup("Error","Incorrect serial port!"); 
			return -1;
		}
		SetCTSMode (i, LWRS_HWHANDSHAKE_OFF);   //��Ӳ������
	}
}
int  CheckControlPort(unsigned char* controlComPort,unsigned char i) 			 //�������Ļ������ں��Ƿ���ȷ 
{
	char IDquery2[] = {0x11, 0x03, 0x00, 0x02, 0x00, 0x05, 0x26, 0x99};
	char USART_RX_Buffer2[60];   //����
	int b, strLen;

	SetBreakOnLibraryErrors (0);
	b = OpenComConfig(i, "", 115200, 0, 8, 1, 30, 30);  //���úʹ򿪴��� 
    if(b<0 )
	{
		MessagePopup("Error","Incorrect serial port!"); 
		return -1;
	}
	else
	{	
	    ComWrt(i, IDquery2, 8);							//���Ͳ�ѯָ��
		Delay(5);
		strLen = GetInQLen (i);
		ComRd (i,USART_RX_Buffer2,strLen);
		Delay(0.5); 
		if((USART_RX_Buffer2[0]== 0x11) && (USART_RX_Buffer2[1] == 0x03) && (USART_RX_Buffer2[2] == 0x0a))
		{
			*controlComPort = i;							
			FlushInQ(*controlComPort);	   				//Clear input and output buffer
			FlushOutQ(*controlComPort);
			CloseCom (*controlComPort);
			return 0;
		}
		else
		{
			CloseCom(i);
			MessagePopup("Error","Incorrect serial port!"); 
			return -1;
		}
		SetCTSMode (i, LWRS_HWHANDSHAKE_OFF);			//��Ӳ������
	}
}
void TestStop(enum TestMode testMode)
{
	if((RxData1.rxStopSign == 0x01) || (RxData2.rxStopSign == 0x01))  //�κ�һ��ֹͣ�¼�����
	{
		switch(TestPara1.testMode)
		{
			case CALIBRATION:
				
				break;
				
			case NO_SWEEP_IV:
			case NO_SWEEP_VI:
			case NO_SWEEP_IT:
			case NO_SWEEP_RT:
			case NO_SWEEP_VT: 
				
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))													//�ж��û�ѡ������һ��������
					{
						if((RxData1.rxStopSign == 0x01) && (RxData2.rxStopSign == 0x01))								//�ж��Ƿ��������Ӷ��������ˡ�
						{
							RxData1.rxStopSign = 0x00;
							RxData2.rxStopSign = 0x00;
							StopKeyAction(select_Addr1,select_Addr2);													//ֹͣ��ť���º������һϵ�ж��� 
						}
						//StopKeyAction(select_Addr1,select_Addr2);														//ֹͣ��ť���º������һϵ�ж���
					}
				else if(select_Addr1 == 0x01)																			//ֹͣ1�Ű���
					{
						RxData1.rxStopSign = 0x00;																		//������߽�����־λ��������һ�����ݳ�ͻ
						StopKeyAction(0x01,select_Addr2);
					}
				else if(select_Addr2 == 0x02)																			//ֹͣ2�Ű���
					{
						RxData2.rxStopSign = 0x00;																		//������߽�����־λ��������һ�����ݳ�ͻ
						StopKeyAction(0x00,select_Addr2);
					}
				break;
		}
	}
}
void Getxy(unsigned char *measUartRxBuf, RxDataTypeDef* RxData1, RxDataTypeDef* RxData2) 							//�жϽ���Դ��1 Դ��2������
{
		if(*measUartRxBuf  == 0x01) //�ж��Ƿ��� 01 ��ַ���� ������    
		{
			if(TestPara1.testMode == NO_SWEEP_IV)																	//���ݲ�ͬģʽ ѡ��ͬ��X ������
			{
				if(logFlag)
				{
					*(Graph.pCurveArray->pDotY++) = log10((double)(RxData1->rx_current.num_float));					//get y, set pointer to the next data 
				}
				else
				{
					*(Graph.pCurveArray->pDotY++) = RxData1->rx_current.num_float;									//get y, set pointer to the next data 
				}
				*(Graph.pCurveArray->pDotX++) = RxData1->rx_Theory_voltaget;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex), *(Graph.pCurveArray->pDotY-1)); 
				rowIndex++; 
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_voltage.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
			}
			else if(TestPara1.testMode == NO_SWEEP_VI )
			{
				*(Graph.pCurveArray->pDotX++) = RxData1->rx_Theory_current;
				*(Graph.pCurveArray->pDotY++) = RxData1->rx_voltage.num_float;										//get y, set pointer to the next data
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex), *(Graph.pCurveArray->pDotY-1)); 
				rowIndex++; 
				
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_current.num_float);							//��ص���
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
				
			}
			else if(TestPara1.testMode == NO_SWEEP_IT )
			{
				if(logFlag)
				{
					*(Graph.pCurveArray->pDotY++) = log10((double)(RxData1->rx_current.num_float));					//get y, set pointer to the next data 
				}
				else
				{
					*(Graph.pCurveArray->pDotY++) = RxData1->rx_current.num_float;									//get y, set pointer to the next data 
				}
				
				//*(Graph.pCurveArray->pDotX++) = (Graph.X_Axis1++)*TestPara1.timeStep*0.001;
				
				*(Graph.pCurveArray->pDotX++) = RxData1->realTime * 0.001;													//��ʵʱ�� 
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex),*(Graph.pCurveArray->pDotY-1)); 
				rowIndex++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_voltage.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
			}
			if(TestPara1.testMode == NO_SWEEP_VT )
			{
				*(Graph.pCurveArray->pDotX++) = RxData1->realTime * 0.001;													//��ʵʱ��
				*(Graph.pCurveArray->pDotY++) = RxData1->rx_voltage.num_float;										//get y, set pointer to the next data 
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex),*(Graph.pCurveArray->pDotY-1)); 
				rowIndex++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_current.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
				
			}
			if(TestPara1.testMode == NO_SWEEP_RT )
			{
				*(Graph.pCurveArray->pDotX++) = RxData1->realTime * 0.001;													//��ʵʱ��		
				*(Graph.pCurveArray->pDotY++) = (TestPara1.Voltage_Start*0.001)/RxData1->rx_current.num_float;		//get y, set pointer to the next data
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex, 1, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (1,rowIndex), *(Graph.pCurveArray->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (2,rowIndex),*(Graph.pCurveArray->pDotY-1)); 
				rowIndex++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV,RxData1->rx_voltage.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT1 ,RxData1->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
			}
			
			
			Graph.pCurveArray->numOfDotsToPlot++;				//number of dots to plot increase 
			Graph.pCurveArray->numofSmu1RxDots++;
			
		}
		//else if((*measUartRxBuf  == 0x02) && ((Graph.pCurveArray + 1)->numofSmu2RxDots <= (Graph.pCurveArray + 1)->numOfTotalDots-1)) //�ж��Ƿ��� 02 ��ַ���� ������ 
		else if(*measUartRxBuf  == 0x02) //�ж��Ƿ��� 02 ��ַ���� ������
		{
			if(TestPara2.testMode == NO_SWEEP_IV )
			{
				if(logFlag)
				{
					*((Graph.pCurveArray + 1)->pDotY++) = log10((double)(RxData2->rx_current.num_float));			//get y, set pointer to the next data 
				}
				else
				{
					*((Graph.pCurveArray + 1)->pDotY++) = RxData2->rx_current.num_float;  							//get y, set pointer to the next data 
				}
				*((Graph.pCurveArray + 1)->pDotX++) = RxData2->rx_Theory_voltaget;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_voltage.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
				
			}
			if(TestPara2.testMode == NO_SWEEP_VI)
			{
				*((Graph.pCurveArray +1)->pDotX++) = RxData2->rx_Theory_current;
				*((Graph.pCurveArray +1)->pDotY++) = RxData2->rx_voltage.num_float;										//get y, set pointer to the next data
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++; 
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_current.num_float);							//��ص���
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);						    //���ֵ�ǰ��λ��Ϣ
				
			}
			if(TestPara2.testMode == NO_SWEEP_IT )
			{
				if(logFlag)
				{
					*((Graph.pCurveArray + 1)->pDotY++) = log10((double)(RxData2->rx_current.num_float));
				}
				else
				{
					*((Graph.pCurveArray + 1)->pDotY++) = RxData2->rx_current.num_float; 
				}
				//*((Graph.pCurveArray + 1)->pDotX++) = (Graph.X_Axis2++)*TestPara2.timeStep*0.001;
				
				*((Graph.pCurveArray + 1)->pDotX++) = RxData2->realTime * 0.001;													//��ʵʱ�� 
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);    
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++; 
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_voltage.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);							//���ֵ�ǰ��λ��Ϣ 
			}
			if(TestPara2.testMode == NO_SWEEP_VT )
			{
				*((Graph.pCurveArray +1)->pDotX++) = RxData2->realTime * 0.001;													//��ʵʱ��
				*((Graph.pCurveArray +1)->pDotY++) = RxData2->rx_voltage.num_float;										//get y, set pointer to the next data 
				
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++;
				
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_current.num_float);							//��ص���
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);						    //���ֵ�ǰ��λ��Ϣ
			}
			if(TestPara2.testMode == NO_SWEEP_RT )
			{
				*((Graph.pCurveArray + 1)->pDotX++) = RxData2->realTime * 0.001;													//��ʵʱ��
				*((Graph.pCurveArray + 1)->pDotY++) = (TestPara2.Voltage_Start*0.001)/RxData2->rx_current.num_float;
				SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex2, 3, 1, 2), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);    
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (3,rowIndex2), *((Graph.pCurveArray + 1)->pDotX-1));
				SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (4,rowIndex2), *((Graph.pCurveArray + 1)->pDotY-1)); 
				rowIndex2++;
				SetCtrlVal (hResultDispPanel, SAMPLE_VVV_2,RxData2->rx_voltage.num_float);							//��ص�ѹ
				SetCtrlVal (hResultDispPanel, SAMPLE_RANGESELECT2 ,RxData2->rangeSelect);							//���ֵ�ǰ��λ��Ϣ
				
			}
			(Graph.pCurveArray+1)->numOfDotsToPlot++;				 													//number of dots to plot increase
			(Graph.pCurveArray+1)->numofSmu2RxDots++;
		}
		//**********************************************************    �жϵ�һ����Y�����ݵ���ȷ����ʼY�������᷶Χ  ***************************************************************************//	
			if(select_Addr1 == 0x01 && select_Addr2 == 0x02)
			{
				if(rowIndex2 == 3)
				{
					if(*((Graph.pCurveArray + 1)->pDotY-1) > 0)																										//�ж�Y������λ Ϊ��ֵ
					{
						Graph.pGraphAttr->yAxisTail = (*((Graph.pCurveArray + 1)->pDotY-1))+(*((Graph.pCurveArray + 1)->pDotY-1)*0.0009);
						Graph.pGraphAttr->yAxisHead = (*((Graph.pCurveArray + 1)->pDotY-1))-(*((Graph.pCurveArray + 1)->pDotY-1)*0.0009);
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
					}
					else if(*((Graph.pCurveArray + 1)->pDotY-1) < 0)																									//�ж�Y������Ϊ��ֵ
					{
						Graph.pGraphAttr->yAxisTail = *((Graph.pCurveArray + 1)->pDotY-1) - *((Graph.pCurveArray + 1)->pDotY-1)*0.0009;
						Graph.pGraphAttr->yAxisHead = *((Graph.pCurveArray + 1)->pDotY-1) + *((Graph.pCurveArray + 1)->pDotY-1)*0.0009;
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
					}
				}
				else if(rowIndex == 3)
				{
					if(*(Graph.pCurveArray->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
					{
						Graph.pGraphAttr->yAxisTail = (*(Graph.pCurveArray->pDotY-1))+(*(Graph.pCurveArray->pDotY-1)*0.0009);
						Graph.pGraphAttr->yAxisHead = (*(Graph.pCurveArray->pDotY-1))-(*(Graph.pCurveArray->pDotY-1)*0.0009);
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
					}
					else if(*(Graph.pCurveArray->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
					{
						Graph.pGraphAttr->yAxisTail = *(Graph.pCurveArray->pDotY-1) - *(Graph.pCurveArray->pDotY-1)*0.0009;
						Graph.pGraphAttr->yAxisHead = *(Graph.pCurveArray->pDotY-1) + *(Graph.pCurveArray->pDotY-1)*0.0009;
						SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
					}
				}
			}
			else if(select_Addr2 == 0x02 && rowIndex2 == 3 )								//�жϵ����������ǵ�һ����   && (*((Graph.pCurveArray + 1)->pDotY) != 0)
			{
				if(*((Graph.pCurveArray + 1)->pDotY-1) > 0)																											//�ж�Y������λ Ϊ��ֵ
				{
					Graph.pGraphAttr->yAxisTail = (*((Graph.pCurveArray + 1)->pDotY-1))+(*((Graph.pCurveArray + 1)->pDotY-1)*0.0009);
					Graph.pGraphAttr->yAxisHead = (*((Graph.pCurveArray + 1)->pDotY-1))-(*((Graph.pCurveArray + 1)->pDotY-1)*0.0009);
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
				}
				else if(*((Graph.pCurveArray + 1)->pDotY-1) < 0)																									//�ж�Y������Ϊ��ֵ
				{
					Graph.pGraphAttr->yAxisTail = *((Graph.pCurveArray + 1)->pDotY-1) - *((Graph.pCurveArray + 1)->pDotY-1)*0.0009;
					Graph.pGraphAttr->yAxisHead = *((Graph.pCurveArray + 1)->pDotY-1) + *((Graph.pCurveArray + 1)->pDotY-1)*0.0009;
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
				}
			}
			else if(select_Addr1 == 0x01 && rowIndex == 3)																							
			{
				if(*(Graph.pCurveArray->pDotY-1) > 0)																												//�ж�Y������λ Ϊ��ֵ
				{
					Graph.pGraphAttr->yAxisTail = (*(Graph.pCurveArray->pDotY-1))+(*(Graph.pCurveArray->pDotY-1)*0.0009);
					Graph.pGraphAttr->yAxisHead = (*(Graph.pCurveArray->pDotY-1))-(*(Graph.pCurveArray->pDotY-1)*0.0009);
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);		//����  X  ��ķ�Χ
				}
				else if(*(Graph.pCurveArray->pDotY-1) < 0)																											//�ж�Y������Ϊ��ֵ
				{
					Graph.pGraphAttr->yAxisTail = *(Graph.pCurveArray->pDotY-1) - *(Graph.pCurveArray->pDotY-1)*0.0009;
					Graph.pGraphAttr->yAxisHead = *(Graph.pCurveArray->pDotY-1) + *(Graph.pCurveArray->pDotY-1)*0.0009;
					SetAxisScalingMode(graphDispPanel, GRAPHDISP_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL,Graph.pGraphAttr->yAxisHead, Graph.pGraphAttr->yAxisTail);					//����  X  ��ķ�Χ
				}
			}
		//**********************************************************    �жϵ�һ����Y�����ݵ���ȷ����ʼY�������᷶Χ  ***************************************************************************//	
}
void CVICALLBACK ComCallback(int portNumber, int eventMask, void * callbackData)
{
	measure_Uart_Flag = 0;																						//����ʱ���β�ѯ���ر����ݲ�ѯ
	SetAsyncTimerAttribute (TimerID, ASYNC_ATTR_ENABLED, 0); 													//����ʱ���β�ѯ���ر����ݲ�ѯ//����ʱ���β�ѯ�����ڲ�ѯ����
		int rxNum = 0;																							  
		int i=0;																								
		rxNum = GetInQLen(measureComPort);  																	//��ȡ�����з���������������
		if(rxNum>1024) rxNum=1024;																				//��ֹ�����ڴ淶Χ
		rxNum-= rxNum%MEASURE_UART_RX_LEN;																		//ֻ��ȡ�����ַ��������������ݣ�������ɢ����
		ComRd(measureComPort, (char *)measUartRxBuf1, rxNum); 
		while(rxNum>=MEASURE_UART_RX_LEN)
		{	 
								
				ProtocolGetData(measUartRxBuf1+i*MEASURE_UART_RX_LEN, &RxData1,&RxData2);						//get data from uart buffer ,�����ж��Ƿ���Դ��1��2 ���ݣ��ֱ������Ӧ�Ļ�����
				Getxy(&measUartRxBuf1[i*MEASURE_UART_RX_LEN], &RxData1, &RxData2);								//�Ӵ��ڴ�����������ȡ��  X��Y�� ������
				rxNum -=MEASURE_UART_RX_LEN;
				i++;
		}
		PlotCurve(&Graph, graphDispPanel, GRAPHDISP_GRAPH1);									 		 
		TestStop(TestPara1.testMode);
	measure_Uart_Flag = 1; 																						//������ѯ����
	SetAsyncTimerAttribute (TimerID, ASYNC_ATTR_ENABLED, 1); 													//����ʱ���β�ѯ���ر����ݲ�ѯ//����ʱ���β�ѯ�����ڲ�ѯ����

}
//void CVICALLBACK CtrlComCallback(int portNumber, int eventMask, void * callbackData)
//{
//	control_Uart_Flag = 0;																						//����ʱ���β�ѯ�����ڲ�ѯ����     
//		int rxNum;
//		int i,j = 0;
//		Rx_CGS_DataTypeDef Rx_CGS_Data;
//		Rx_CGS_Data.heating_stage_temp = 0;
//		Rx_CGS_Data.environmental_humidity = 0;
//		Rx_CGS_Data.pressure = 0;
//		rxNum = GetInQLen(controlComPort);  															//��ȡ�����з���������������
//		if(rxNum>500) rxNum=500;																		//��ֹ�����ڴ淶Χ
//		while(rxNum>=15)
//		{
//			if((Graph.pCurveArray->numOfPlotDots >0) || ((Graph.pCurveArray + 1)->numOfPlotDots >0))
//			{
//				ComRd(controlComPort, (char *)meas_CGS_UartRxBuf, 15);
//				ProtocolGet_CGS_Data(meas_CGS_UartRxBuf, &Rx_CGS_Data);									//�Ӵ�����ȡ��������������
//				if(Rx_CGS_Data.rxDevAddr == 0x11)
//				{
//					
//								if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))								//�жϻ��������������Ǹ��� Դ��1  ����Դ��2 �ĺ�����
//								{
//									j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
//								}
//								if(select_Addr1 == 0x01)
//								{
//									j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
//								}
//								if(select_Addr2 == 0x02)
//								{
//									j = (Graph.pCurveArray + 1)->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots; 
//								}
//						for(i = 0; i<j; i++)				  
//						{
//							if((TestPara1.testMode == NO_SWEEP_VT) || (TestPara1.testMode == NO_SWEEP_RT) || (TestPara1.testMode == NO_SWEEP_IT) || (TestPara2.testMode == NO_SWEEP_RT) || (TestPara2.testMode == NO_SWEEP_VT) || (TestPara2.testMode == NO_SWEEP_IT))
//							{
//								*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.timeStep*0.001;
//								*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.timeStep*0.001; 
//								*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.timeStep*0.001;
//								Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + 1;										//1*TestPara1.Current_Step;
//							}
//							if((TestPara1.testMode == NO_SWEEP_IV) || (TestPara1.testMode == NO_SWEEP_VI) || (TestPara2.testMode == NO_SWEEP_IV) || (TestPara2.testMode == NO_SWEEP_VI))
//							{
//								*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1;
//								*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1; 
//								*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1;
//								Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + TestPara2.Current_Step;					//1*TestPara1.Current_Step;  
//							}
//							
//							
//						    *(Graph_Temp.pCurveArray->pDotY++)		 = Rx_CGS_Data.heating_stage_temp;				//��̨�¶�      
//							*((Graph_Temp.pCurveArray + 1)->pDotY++) = Rx_CGS_Data.environmental_humidity;			//����ʪ��
//							
//							if(Rx_CGS_Data.pressure > 10500)														//����ѹǿ��ʾ����׼����ѹ��
//							{
//								Rx_CGS_Data.pressure = 10500;		  
//							}
//							*((Graph_Temp.pCurveArray + 2)->pDotY++) = Rx_CGS_Data.pressure * 0.001;				//ѹǿ
//					
//							if(tempVal)			  //DLP
//							{
//								SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 5, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
//								SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (5,rowIndex3), *(Graph_Temp.pCurveArray->pDotY-1)); 
//							}
//							if(humidityVal)
//							{
//								SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 6, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
//								SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (6,rowIndex3), *((Graph_Temp.pCurveArray + 1)->pDotY-1));
//							}
//							if(pressureVal)
//							{
//								SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 7, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
//								SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (7,rowIndex3), Rx_CGS_Data.pressure*1.000);
//							}
//							/*SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 5, 1, 3), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
//							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (5,rowIndex3), *(Graph_Temp.pCurveArray->pDotY-1));
//							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (6,rowIndex3), *((Graph_Temp.pCurveArray + 1)->pDotY-1));
//							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (7,rowIndex3), Rx_CGS_Data.pressure*1.000);*/							//ѹǿ
//							rowIndex3++;
//				
//							Graph_Temp.pCurveArray->numOfDotsToPlot++;  
//							(Graph_Temp.pCurveArray +1)->numOfDotsToPlot++;
//							(Graph_Temp.pCurveArray +2)->numOfDotsToPlot++;
//						}
//				}
//			}
//			else
//			{
//			FlushInQ(controlComPort);	   											
//			FlushOutQ(controlComPort);
//			}
//			rxNum -=15;  
//		}
//		PlotCurve_Temp(&Graph_Temp, graphDispPanel, GRAPHDISP_GRAPH2, &Rx_CGS_Data);					//������ͼ
//		control_Uart_Flag = 1;																				//����ʱ���β�ѯ�����ڲ�ѯ����
//}
void CVICALLBACK CtrlComCallback(int portNumber, int eventMask, void * callbackData)
{
	SetAsyncTimerAttribute (TimerID, ASYNC_ATTR_ENABLED, 0); 													//����ʱ���β�ѯ���ر����ݲ�ѯ//����ʱ���β�ѯ�����ڲ�ѯ����
		int rxNum;
		int i,j = 0;
		rxNum = GetInQLen(controlComPort);  															//��ȡ�����з���������������
		if(rxNum>500) rxNum=500;																		//��ֹ�����ڴ淶Χ
		while(rxNum>=15)
		{
			if((Graph.pCurveArray->numOfPlotDots >0) || ((Graph.pCurveArray + 1)->numOfPlotDots >0))
			{
				ComRd(controlComPort, (char *)meas_CGS_UartRxBuf, 15);
				ProtocolGet_CGS_Data(meas_CGS_UartRxBuf, &Rx_CGS_Data);									//�Ӵ�����ȡ��������������
				if(Rx_CGS_Data.rxDevAddr == 0x11)
				{
					
								if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))								//�жϻ��������������Ǹ��� Դ��1  ����Դ��2 �ĺ�����
								{
									j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
								}
								else if(select_Addr1 == 0x01)
								{
									j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
								}
								else if(select_Addr2 == 0x02)
								{
									j = (Graph.pCurveArray + 1)->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots; 
								}
						for(i = 0; i<j; i++)				  
						{
							if((TestPara1.testMode == NO_SWEEP_VT) || (TestPara1.testMode == NO_SWEEP_RT) || (TestPara1.testMode == NO_SWEEP_IT) || (TestPara2.testMode == NO_SWEEP_RT) || (TestPara2.testMode == NO_SWEEP_VT) || (TestPara2.testMode == NO_SWEEP_IT))
							{
								*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.TotalDelay;
								*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.TotalDelay; 
								*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.TotalDelay;
								Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + 1;										//1*TestPara1.Current_Step;
							
							 /*   *(Graph_Temp.pCurveArray->pDotX++) = *(Graph.pCurveArray->pDotX + Graph_Temp.X_Axis1);
								*((Graph_Temp.pCurveArray + 1)->pDotX++) = *(Graph.pCurveArray->pDotX + Graph_Temp.X_Axis1); 
								*((Graph_Temp.pCurveArray + 2)->pDotX++) = *(Graph.pCurveArray->pDotX + Graph_Temp.X_Axis1);
								
								Graph_Temp.X_Axis1++;*/
							
							}
							if((TestPara1.testMode == NO_SWEEP_IV) || (TestPara1.testMode == NO_SWEEP_VI) || (TestPara2.testMode == NO_SWEEP_IV) || (TestPara2.testMode == NO_SWEEP_VI))
							{
								*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1;
								*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1; 
								*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1;
								
								if((TestPara1.testMode == NO_SWEEP_IV) || (TestPara2.testMode == NO_SWEEP_IV))
								{
									Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + TestPara1.Current_Step;				//1*TestPara1.Current_Step;
								}
								if((TestPara1.testMode == NO_SWEEP_VI) || (TestPara2.testMode == NO_SWEEP_VI))
								{
								   Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + TestPara1.Voltage_Step;				//1*TestPara1.Current_Step;
								}
								  
							}
						    *(Graph_Temp.pCurveArray->pDotY++)		 = Rx_CGS_Data.heating_stage_temp;				//��̨�¶�      
							*((Graph_Temp.pCurveArray + 1)->pDotY++) = Rx_CGS_Data.environmental_humidity;			//����ʪ��
							
							if(Rx_CGS_Data.pressure > 10500)														//����ѹǿ��ʾ����׼����ѹ��
							{
								Rx_CGS_Data.pressure = 10500;		  
							}
							*((Graph_Temp.pCurveArray + 2)->pDotY++) = Rx_CGS_Data.pressure * 0.001;				//ѹǿ
					
							if(tempVal)			  //DLP
							{
								SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 5, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
								SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (5,rowIndex3), *(Graph_Temp.pCurveArray->pDotY-1)); 
							}
							if(humidityVal)
							{
								SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 6, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
								SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (6,rowIndex3), *((Graph_Temp.pCurveArray + 1)->pDotY-1));
							}
							if(pressureVal)
							{
								SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 7, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
								SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (7,rowIndex3), Rx_CGS_Data.pressure*1.000);
							}
							rowIndex3++;
				
							Graph_Temp.pCurveArray->numOfDotsToPlot++;  
							(Graph_Temp.pCurveArray +1)->numOfDotsToPlot++;
							(Graph_Temp.pCurveArray +2)->numOfDotsToPlot++;
						}
				}
			}
			else
			{
			FlushInQ(controlComPort);	   											
			FlushOutQ(controlComPort);
			}
			rxNum -=15;  
		}
		PlotCurve_Temp(&Graph_Temp, graphDispPanel, GRAPHDISP_GRAPH2, &Rx_CGS_Data);							//������ͼ
	SetAsyncTimerAttribute (TimerID, ASYNC_ATTR_ENABLED, 1); 													//����ʱ���β�ѯ���ر����ݲ�ѯ//����ʱ���β�ѯ�����ڲ�ѯ����

}

//===========================================С���д=================================================================================  
int CVICALLBACK AbnmDCThreadFunction (void *functionData)
{ 
    //int n = 1;   	 //ÿʮ��֮n����һ�λ���ȥ������ 
	int i,j = 0;
	while(threadFlag == 1)
	{
		//if(Graph.pCurveArray->numOfPlotDots > 0)  //ÿʮ��֮һ���ܵ���ʱ����һ�λ�����
		//{
		// /*   WriteAndSaveExcel(tablePanel, TABLE_TABLE1);
		//	n +=1;
		//	if(n > 10)
		//	{
		//		n = 1;
		//	}*/
		//}
		
		if((control_Uart_Flag > 0) && (stop_Flag == 1))  													//�жϻ��������Ƿ��û�ѡ���ж����һ�黷�������Ƿ�ȱ�㡣����ȱ�ٵĵ�����
		{
				if((select_Addr1 == 0x01) && (select_Addr2 == 0x02))										//�жϻ��������������Ǹ��� Դ��1  ����Դ��2 �ĺ�����
				{
					j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;   		//�������ӵ�ʱ�����1�Ű��ӵĺ�����
				}
				if(select_Addr1 == 0x01)
				{
					j = Graph.pCurveArray->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots;
				}
				if(select_Addr2 == 0x02)
				{
					j = (Graph.pCurveArray + 1)->numOfPlotDots - Graph_Temp.pCurveArray->numOfPlotDots; 
				}
					//ȱ�����㲹������
					for(i = 0; i<j; i++)				  
					{
						if((TestPara1.testMode == NO_SWEEP_VT) || (TestPara1.testMode == NO_SWEEP_RT) || (TestPara1.testMode == NO_SWEEP_IT) || (TestPara2.testMode == NO_SWEEP_RT) || (TestPara2.testMode == NO_SWEEP_VT) || (TestPara2.testMode == NO_SWEEP_IT))
						{
							*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.TotalDelay;
							*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.TotalDelay; 
							*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1 * TestPara1.TotalDelay;
							Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + 1;											//1*TestPara1.Current_Step;
						}
						if((TestPara1.testMode == NO_SWEEP_IV) || (TestPara1.testMode == NO_SWEEP_VI) || (TestPara2.testMode == NO_SWEEP_IV) || (TestPara2.testMode == NO_SWEEP_VI))
						{
							*(Graph_Temp.pCurveArray->pDotX++) = Graph_Temp.X_Axis1;
							*((Graph_Temp.pCurveArray + 1)->pDotX++) = Graph_Temp.X_Axis1; 
							*((Graph_Temp.pCurveArray + 2)->pDotX++) = Graph_Temp.X_Axis1;
							Graph_Temp.X_Axis1 = Graph_Temp.X_Axis1 + TestPara2.Current_Step;						//1*TestPara1.Current_Step;  
						}
						    *(Graph_Temp.pCurveArray->pDotY++)		 = Rx_CGS_Data.heating_stage_temp;				//��̨�¶�      
							*((Graph_Temp.pCurveArray + 1)->pDotY++) = Rx_CGS_Data.environmental_humidity;			//����ʪ��
						
						if(Rx_CGS_Data.pressure > 10500)															//����ѹǿ��ʾ����׼����ѹ��
						{
							Rx_CGS_Data.pressure = 10500;		  
						}
							*((Graph_Temp.pCurveArray + 2)->pDotY++) = Rx_CGS_Data.pressure * 0.001;				//ѹǿ
				
						if(tempVal)			  //DLP
						{
							SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 5, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (5,rowIndex3), *(Graph_Temp.pCurveArray->pDotY-1)); 
						}
						if(humidityVal)
						{
							SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 6, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (6,rowIndex3), *((Graph_Temp.pCurveArray + 1)->pDotY-1));
						}
						if(pressureVal)
						{
							SetTableCellRangeAttribute (tablePanel, TABLE_TABLE1, MakeRect (rowIndex3, 7, 1, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);
							SetTableCellVal (tablePanel, TABLE_TABLE1, MakePoint (7,rowIndex3), Rx_CGS_Data.pressure*1.000);
						}
						rowIndex3++;
						Graph_Temp.pCurveArray->numOfDotsToPlot++;  
						(Graph_Temp.pCurveArray +1)->numOfDotsToPlot++;
						(Graph_Temp.pCurveArray +2)->numOfDotsToPlot++;
					}
				PlotCurve_Temp(&Graph_Temp, graphDispPanel, GRAPHDISP_GRAPH2,&Rx_CGS_Data);										//������ͼ
				//stop_Flag = 0;																								//��������־λ
		}
		if(stop_Flag == 1)
		{
		
			SetGraphX_Axis(GRAPHDISP_GRAPH1,&Graph);		
			SetGraphX_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp);
			SetGraphY_Axis_TEMP(GRAPHDISP_GRAPH2,&Graph_Temp);
		
			stop_Flag = 0;
		}
	}
	return 0;
}

int CVICALLBACK QINGCHU_CB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			
			
			
			//����������Ϳ���   *(Graph.pCurveArray->pDotX++) = RxData1->rx_Theory_voltaget;
			Graph.pGraphAttr->xAxisHead = *(Graph.pCurveArray->pDotX-1);
			Graph.pGraphAttr->xAxisTail = (*(Graph.pCurveArray->pDotX-1))*1.5;
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH1,VAL_BOTTOM_XAXIS,VAL_MANUAL,Graph.pGraphAttr->xAxisHead,Graph.pGraphAttr->xAxisTail);
			

			break;
	}
	return 0;
}

int CVICALLBACK QUANTU_CB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			SetAxisScalingMode(graphDispPanel,GRAPHDISP_GRAPH1,VAL_BOTTOM_XAXIS,VAL_MANUAL,0,Graph.pGraphAttr->xAxisTail);								
			/*PlotXY(graphDispPanel, control, Graph.pCurveArray->pDotXHead, Graph.pCurveArray->pDotYHead, Graph.pCurveArray->numOfPlotDots, VAL_FLOAT, VAL_FLOAT, 
										CurveAttr.plotStyle, 
									  	CurveAttr.pointStyle, 
									  	CurveAttr.lineStyle, 1, 
									  	CurveAttr.smu1lineColor);*/

			break;
	}
	return 0;
}
