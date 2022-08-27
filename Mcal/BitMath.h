/*
 * BitMath.h
 *
 * Created: 22/04/2022 02:45:30 م
 *  Author: dell
 */ 


#ifndef BITMATH_H_
#define BITMATH_H_

#define SetBit(reg,bit)      reg |=   ( 1 << bit  )
#define ClrBit(reg,bit)      reg &= ~ ( 1 << bit  )
#define TogBit(reg,bit)      reg ^=   ( 1 << bit  )
#define GetBit(reg,bit)          (1 & (reg >> bit ))

#endif /* BITMATH_H_ */