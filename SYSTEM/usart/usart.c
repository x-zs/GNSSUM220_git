#include "usart.h"
#include "delay.h"
#include "signal.h"
#include "exti.h"
#include "dma.h"
#include "string.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
void UART2_send(u8 *p,u16 lenth)
{u16 i=0;
for(;i<lenth;i++)
	{	
		while((USART2->SR&0X40)==0);//ѭ������,ֱ���������    
	USART2->DR = *(p+i);
	}   
}
UART_HandleTypeDef UART1_Handler; //UART���
UART_HandleTypeDef UART2_Handler;
UART_HandleTypeDef UART3_Handler;

//��ʼ��IO ����1 
//bound:������
void UART1_Init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    					//USART1
	UART1_Handler.Init.BaudRate=bound;				    				//������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   	//�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    		//һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    		//����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    			//�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    					//HAL_UART_Init()��ʹ��UART1
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

	//�򿪿����ж� ��ʼ��DMA����
	//UsartType1.dmaSend_flag = USART_DMA_SENDOVER;
	//HAL_UART_Receive_DMA(&UART1_Handler, UsartType1.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&UART1_Handler, UART_IT_IDLE);
	
}

void UART2_Init(u32 bound)
{
	//UART ��ʼ������
	UART2_Handler.Instance=USART2;					    					//UART2
	UART2_Handler.Init.BaudRate=bound;				    				//������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   	//�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    		//һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    		//����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    			//�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    					//HAL_UART_Init()��ʹ��UART2
	
	//HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

	//�򿪿����ж� ��ʼ��DMA����
	//UsartType2.dmaSend_flag = USART_DMA_SENDOVER;
	__HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_IDLE);
}

void UART3_Init(u32 bound)
{
	//UART ��ʼ������
	UART3_Handler.Instance=USART3;					    					//UART3
	UART3_Handler.Init.BaudRate=bound;				    				//������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   	//�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    		//һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    		//����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   	//��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    			//�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					    					//HAL_UART_Init()��ʹ��UART3
	
	//HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

	//�򿪿����ж� ��ʼ��DMA����
	//UsartType3.dmaSend_flag = USART_DMA_SENDOVER;
	__HAL_UART_ENABLE_IT(&UART3_Handler, UART_IT_IDLE);
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)						//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();				//ʹ��GPIOBʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;				//PB6,7
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;						//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;								//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;						//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;				//����ΪUSART1
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   			//��ʼ��PB6,7
		HAL_NVIC_EnableIRQ(USART1_IRQn);						//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,0);			//��ռ���ȼ�3�������ȼ�3
	}
	else if (huart->Instance==USART3)				//����Ǵ���3�����д���3 MSP��ʼ��
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();					//ʹ��GPIODʱ��
		__HAL_RCC_USART3_CLK_ENABLE();				//ʹ��USART3ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;			//PD8,9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;					//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;							//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;					//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;			//����ΪUSART3
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   			//��ʼ��

		HAL_NVIC_EnableIRQ(USART3_IRQn);						//ʹ��USART3�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�0
	}
	else if (huart->Instance==USART2)					//����Ǵ���2�����д���2 MSP��ʼ��
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();						//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();					//ʹ��USART2ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;			//PD5,6
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;					//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;							//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;					//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;			//����ΪUSART2
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   			//��ʼ��

		HAL_NVIC_EnableIRQ(USART2_IRQn);						//ʹ��USART2�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,3,1);			//��ռ���ȼ�3�������ȼ�1
	}

}
//u8 flag=0;
u16 rx_len1=0;
//u16 rx_len2=0;
//���ڽ��տ����ж�
void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{
	uint32_t temp;
	uint32_t rx_len;
	OS_ERR err;	

	if (huart->Instance == USART1)
	{
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&UART1_Handler);
//			HAL_UART_DMAStop(&UART2_Handler);

			
			//printf("11");
		}
	}
	
	if (huart->Instance == USART2)
	{
		
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&UART2_Handler);
			UART2_Handler.Instance->CR3 &= ~USART_CR3_DMAR;
			HAL_DMA_Abort(UART2_Handler.hdmarx);
			UART2_Handler.State = HAL_UART_STATE_READY;
			temp = UART2_Handler.hdmarx->Instance->NDTR;
			rx_len =  RECEIVELEN - temp;
			rx_len1=rx_len1+rx_len;	
		 if(UART2_rxBuf[0]==0xd3&&(UART2_rxBuf[3]==0x43||UART2_rxBuf[3]==0x46))
			{	//rx_len=rx_len1+rx_len;	
				if(UART2_rxBuf[rx_len1-2]==0x0d&&UART2_rxBuf[rx_len1-1]==0x0a)
				{
	
					OSQPost(&OSQ_UART1RxMsgQ, &UART2_rxBuf[0], rx_len1-2, OS_OPT_POST_FIFO, &err);								
					HAL_UART_Receive_DMA(&UART2_Handler,&UART2_rxBuf[0],RECEIVELEN);					//������������
					rx_len1=0;
				}
				else
				{
		  	HAL_UART_Receive_DMA(&UART2_Handler,&UART2_rxBuf[rx_len1],RECEIVELEN);					
				}
				
			}
			else 
			{ rx_len1=0;
			 UART2_rxBuf[rx_len]=0;
			 HAL_UART_Receive_DMA(&UART2_Handler,&UART2_rxBuf[0],RECEIVELEN);
			}
		}
	}
}



//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	UsartReceive_IDLE(&UART1_Handler);		//���ý��տ����ж�
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif


}

void USART2_IRQHandler(void)                	
{ 
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	UsartReceive_IDLE(&UART2_Handler);		//���ý��տ����ж�
	HAL_UART_IRQHandler(&UART2_Handler);	//����HAL���жϴ����ú���
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
	
}

void USART3_IRQHandler(void)                	
{ 
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	UsartReceive_IDLE(&UART3_Handler);		//���ý��տ����ж�
	HAL_UART_IRQHandler(&UART3_Handler);	//����HAL���жϴ����ú���
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}

