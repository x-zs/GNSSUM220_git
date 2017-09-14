#ifndef _UB351_H
#define _UB351_H
#include "sys.h"
#include "stdio.h"	
#define Station_Number_H (512& 0xFF00)>>8
#define Station_Number_L (u8)(512&0xFF)
#define Station_Number 512
#define Station_Type 3
#define Reser_Len 4  //±£Áô×Ö³¤¶È
extern u8 IR_500ms;
void UB351_Receive(u8 *message,u16 length);
void UB351_Transmit(u8 *message,u16 length,u8 Frame_type);
#endif
