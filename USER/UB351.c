#include "UB351.h"
#include "dma.h"
#include "string.h"
extern u8 flag;
extern u16 rx_len1;
extern u16 rx_len2;					
u8 Frame_Type=0;
u16 Frame_Len=0;
u8 Frame_LenH=0;
u8 Frame_LenL=0;
u8 Frame_number=0;
u8 Frame_Head[12]={0xaa,0x55,0xa5,0x5a,Station_Type,Station_Number_L,Station_Number_H,0,0,0,0,0};//帧头=开始帧+ 节点类型+节点编号+帧类型+长度低字节+长度高字节
u8 Status_Msg[Reser_Len]={0,0xff,0xff,0xff};  //保留字 帧序号+3字节保留
void UB351_Transmit(u8 *message,u16 length,u8 Frame_type)
{	u8 Frame_check=0;
	u16 i=0;

	Frame_Len=length+Reser_Len;   //观测量载荷+状态信息载荷+校验位
	Frame_Head[11]=(Frame_Len& 0xFF00)>>8; //长度高字节
	Frame_Head[10]=(u8)(Frame_Len&0xFF);    //长度低字节
	Frame_Len=0;	
	Frame_Head[9]=Frame_type;
	Status_Msg[0]=Frame_number;  //发送帧序号
	Frame_number+=1;		

	for(i=0;i<12;i++)  //帧头
	{
	 Frame_check^=Frame_Head[i];
		printf("%c",Frame_Head[i]);
	}		
	for(i=0;i<Reser_Len;i++)  //状态信息
	{
	 Frame_check^=Status_Msg[i];
		printf("%c",Status_Msg[i]);
	}	
	for(i=0;i<length;i++)
	{
	 printf("%c",message[i]);
	 Frame_check^=message[i];
	}
	printf("%c",Frame_check);		 //异或校验位
	

}
