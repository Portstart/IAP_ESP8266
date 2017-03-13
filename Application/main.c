#include "action_sys.h"
#include "action_uart.h"
#include "action_gpio.h"
#include "iap.h"

int main(void)
{
	uint16_t oldcount=0;	//�ϵĴ��ڽ�������ֵ
	uint32_t applenth=0;	//���յ���app���볤��
	
	Device_Init();
	Delay_Ms(1000);
	//USART1_Init(115200);
	USART3_Init(115200);
	
	while(1)
	{
		if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				USART_printf(USART3,"APP received completely\r\n");
				USART_printf(USART3,"Code:%dBytes\r\n",applenth);
				
				if(applenth)
			{
				USART_printf(USART3,"��ʼ���¹̼�...\r\n");	
 				if(((*(__IO uint32_t *)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF_BIN,applenth);//����FLASH����   
					USART_printf(USART3,"�̼��������!\r\n");	
				}
				else USART_printf(USART3,"��FLASHӦ�ó���!\r\n");
 			}
			else 
			{
				USART_printf(USART3,"û�п��Ը��µĹ̼�!\r\n");
				
			}
			
			USART_printf(USART3,"��ʼִ��FLASH�û�����!!\r\n");
			Delay_Ms(500);
			if(((*(__IO uint32_t *)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.	 
					 iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			else USART_printf(USART3,"��FLASHӦ�ó���,�޷�ִ��!\r\n");							
			
			
			}
			else oldcount=USART_RX_CNT;			
		}
		Delay_Ms(10);	
		
	}

}