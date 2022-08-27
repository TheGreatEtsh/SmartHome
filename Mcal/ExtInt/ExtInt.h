/*
 * ExtInt.h
 *
 * Created: 07/05/2022 05:01:12 م
 *  Author: dell
 */ 


#ifndef EXTINT_H_
#define EXTINT_H_

/*********************************_INCLUDES_******************************/
#include "BitMath.h"
#include "Reg.h"
#include <avr/interrupt.h>
/***********************************_MACOS_*******************************/
#define RISING_E              1
#define FAILING_E             2
#define ANY_L_CHANGE          3
#define LOW_LEVEL             4
#define NULL_                '\0'
/********************************_PROTOTYPES_*****************************/
void M_ExtInt_Init(void);
void M_ExtInt_SetCallBack(void(*)(void));
#endif /* EXTINT_H_ */