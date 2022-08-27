/*
 * Hc05.c
 *
 * Created: 28/05/2022 04:12:14 م
 *  Author: dell
 */ 
#include "Hc05.h"

void H_Hc05_Init(void)
{
	M_Uart_Init();
}
void H_Hc05_Send(u8 Local_u8_Data)
{
	M_Uart_Write(Local_u8_Data);
}
u8   H_Hc05_Rec(void)
{
	u8 Local_u8_Data = 0;
	Local_u8_Data = M_Uart_Read();
	return Local_u8_Data;
}