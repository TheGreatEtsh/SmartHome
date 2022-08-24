/*
 * LcdCfg.h
 *
 * Created: 6/8/2022 1:23:13 AM
 *  Author: Ahmed Hesham
 */ 


#ifndef LCDCFG_H_
#define LCDCFG_H_

/*******************************_SELECT_LCD_MODE********************************/
/*
 * OPTION 1-> [ _4_BIT_MODE ]
 * OPTION 1-> [ _8_BIT_MODE ]
 */
#define LCD_MODE			_4_BIT_MODE

/****************************_SELECT_DATA_REGISTER********************************/

#define LCD_DATA_PORT		PORTA

/*******************************_SELECT_MC_PINS********************************/

#define LCD_RS_PIN			PB1
#define LCD_RW_PIN			PB2
#define LCD_EN_PIN			PB3
#define LCD_DATA_4_PIN		PA4
#define LCD_DATA_5_PIN		PA5
#define LCD_DATA_6_PIN		PA6
#define LCD_DATA_7_PIN		PA7

#endif /* LCDCFG_H_ */