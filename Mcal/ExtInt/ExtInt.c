/*
 * ExtInt.c
 *
 * Created: 07/05/2022 05:01:01 م
 *  Author: dell
 */ 

#include "ExtInt.h"

void(*ExtInt_void_CallBack)(void) = NULL_;

void M_ExtInt_Init(void)
{
	#if   SENSE_CONTROL       ==      RISING_E
	SetBit(MCUCR,0);
	SetBit(MCUCR,1);
	#elif SENSE_CONTROL       ==      FAILING_E
	ClrBit(MCUCR,0);
	SetBit(MCUCR,1);
	#elif SENSE_CONTROL       ==      ANY_L_CHANGE
	SetBit(MCUCR,0);
	ClrBit(MCUCR,1);
	#elif SENSE_CONTROL       ==      LOW_LEVEL
	ClrBit(MCUCR,0);
	ClrBit(MCUCR,1);
	#endif
	// to enable INT0 interrupt
	SetBit(GICR,6);
	// to enable global interrupt
	SetBit(SREG,7);
}
void M_ExtInt_SetCallBack(void(*Local_void_Ptr)(void))
{
	ExtInt_void_CallBack = Local_void_Ptr;
}
ISR(INT0_vect)
{
	ExtInt_void_CallBack();	
}

