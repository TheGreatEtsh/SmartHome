/*
 * PushButton.c
 *
 * Created: 6/7/2022 1:38:24 PM
 *  Author: elwady
 */ 

#include "PushButton.h"

void H_PushButton_Init(u8 Local_u8_Push_Button_Number)
{
	switch(Local_u8_Push_Button_Number)
	{
		case PUSH_BUTTON_0: M_Dio_PinMode(PUSH_BUTTON_0_PIN,INPUT);		break;
		case PUSH_BUTTON_1: M_Dio_PinMode(PUSH_BUTTON_1_PIN,INPUT);		break;
		case PUSH_BUTTON_2: M_Dio_PinMode(PUSH_BUTTON_2_PIN,INPUT);		break;
		default:														break;
	}
}
u8	H_PushButton_Read(u8 Local_u8_Push_Button_Number)
{
	u8 Local_u8_Push_Button_Value = 0;
	switch(Local_u8_Push_Button_Number)
	{
		case PUSH_BUTTON_0: Local_u8_Push_Button_Value = M_Dio_PinRead(PUSH_BUTTON_0);  /*if( M_Dio_PinRead(PUSH_BUTTON_0_PIN) == PRESSED ) { _delay_ms(80); if( M_Dio_PinRead(PUSH_BUTTON_0_PIN) == PRESSED ) {while(M_Dio_PinRead(PUSH_BUTTON_0 == PRESSED));Local_u8_Push_Button_Value = PRESSED;}else Local_u8_Push_Button_Value = RELEASED;} else Local_u8_Push_Button_Value = RELEASED;*/break;
		case PUSH_BUTTON_1: Local_u8_Push_Button_Value = M_Dio_PinRead(PUSH_BUTTON_1);  /*if( M_Dio_PinRead(PUSH_BUTTON_1_PIN) == PRESSED ) { _delay_ms(80); if( M_Dio_PinRead(PUSH_BUTTON_1_PIN) == PRESSED ) {while(M_Dio_PinRead(PUSH_BUTTON_1 == PRESSED));Local_u8_Push_Button_Value = PRESSED;}else Local_u8_Push_Button_Value = RELEASED;} else Local_u8_Push_Button_Value = RELEASED;*/break;
		case PUSH_BUTTON_2: Local_u8_Push_Button_Value = M_Dio_PinRead(PUSH_BUTTON_2); /*if( M_Dio_PinRead(PUSH_BUTTON_2_PIN) == PRESSED ) { _delay_ms(80); if( M_Dio_PinRead(PUSH_BUTTON_2_PIN) == PRESSED ) {while(M_Dio_PinRead(PUSH_BUTTON_2 == PRESSED));Local_u8_Push_Button_Value = PRESSED;}else Local_u8_Push_Button_Value = RELEASED;} else Local_u8_Push_Button_Value = RELEASED;*/break;
		default:														break;
	}
	
	
	return Local_u8_Push_Button_Value ;
}
