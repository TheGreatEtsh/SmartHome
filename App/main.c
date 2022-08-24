/*
 * SmartHome.c
 *
 * Created: 7/5/2022 12:34:53 AM
 * Author : Ahmed Hesham
 */ 

#include "Led.h"
#include "Buzzer.h"
#include "Lcd.h"
#include "KeyPad.h"
#include "Servo.h"
#include "DcMotor.h"
#include "Lm35.h"
#include "Eeprom.h"
#define F_CPU 16000000UL
#include <util/delay.h>

void InputID		(u8* ID);
void InputPassword	(u8* Password);
u8 AdminLogin (u8* ID, u8* Password);
void AddUser (u8* ID, u8* Password);
u8 UserLogin ( u8* ID, u8* Password);
void CheckEEPROM(void);
void DeleteUser (u8 NumberOfUsers, u8* ID);
void DeleteAll (void);

/************************************************************************/
/*	Admin Password is 1234
	Admin UserName is 123							
	Admin Location is @ 101 byte address page 1							*/
/************************************************************************/


/************************************************************************/
/*                                                                      */
/************************************************************************/

int main ()
{
	H_Lcd_Init();
	H_KeyPad_Init();
	H_Eeprom_Init();
	char NumberOfUsers = 0;
<<<<<<< Updated upstream
	NumberOfUsers = H_Eeprom_Read(0,100);

	u8 UserID		[3]	= {0,0,0};
	u8 UserPassword [4] = {0,0,0,0};	
	H_Lcd_Clear();
	H_Lcd_WriteString("Enter ID");
	InputID(UserID);
	
	H_Lcd_Clear();
	H_Lcd_WriteString("Enter Password");
	InputPassword(UserPassword);	
	H_Lcd_Clear();
	UserLogin(UserID,UserPassword);	
=======
// 	NumberOfUsers = H_Eeprom_Read(0,100);
// 
// 	u8 UserID		[3]	= {0,0,0};
// 	u8 UserPassword [4] = {0,0,0,0};	
// 	H_Lcd_Clear();
// 	H_Lcd_WriteString("Enter ID");
// 	InputID(UserID);
// 	
// 	H_Lcd_Clear();
// 	H_Lcd_WriteString("Enter Password");
// 	InputPassword(UserPassword);	
// 	H_Lcd_Clear();
// 	UserLogin(UserID,UserPassword);	
	H_DcMotor_Init();
	H_DcMotor_SetDirection(CW);
	H_DcMotor_Speed(100);
	H_DcMotor_Start();
	_delay_ms(2000);
	H_DcMotor_Stop();
	
>>>>>>> Stashed changes

	while (1)
	{
	}
	
	return 0;
}


void AddUser (u8* ID, u8* Password)
{
	u8 NumberOfUsers = H_Eeprom_Read(0,100);
	if (NumberOfUsers <10)
	{
		u8 Counter = 0, UserLocation  = 0; 
	
		if (NumberOfUsers > 0)
		{
			u8 WrongEntry = 0;
		
			for (; Counter < NumberOfUsers; Counter++)
			{
				
				UserLocation = (Counter*7);
				u8 Check = 0, EEPROMData = 0, EEPROMDataLocation = 0, ValidEntries = 3;
			
				for (; Check < 3; Check++)
				{
					EEPROMDataLocation = UserLocation+Check;
					EEPROMData = H_Eeprom_Read(0, EEPROMDataLocation);
					if (ID[Check] == EEPROMData )
					{
						ValidEntries--;
					}
				
				}
			
				if (ValidEntries == 0)
				{
					/*print on LCD invalid username*/
					WrongEntry ++;
					H_Lcd_WriteString("Invalid Username");
				
				}
			}
		
		
			if (WrongEntry == 0)
			{
				/*print on LCD valid entry*/
				H_Lcd_WriteString("Valid Entry");
				_delay_ms(100);
				u8 Check = 0, EEPROMDataLocation = 0;
				UserLocation = NumberOfUsers * 7;
				for (; Check < 3; Check++)
				{
					EEPROMDataLocation = UserLocation+Check;
					H_Eeprom_Write(ID[Check],0, EEPROMDataLocation);
				}
		
				for (Check = 0; Check < 4; Check++)
				{
					EEPROMDataLocation = UserLocation+Check+3;
					H_Eeprom_Write(Password[Check],0, EEPROMDataLocation);
				}
				NumberOfUsers++;
				H_Eeprom_Write(NumberOfUsers,0,100);
				
			}
		}
		else /*There isn't any user yet*/
		{
			H_Lcd_WriteString("Valid Entry");
			u8 Check = 0, EEPROMDataLocation = 0;
			UserLocation = NumberOfUsers * 7;			/*UserLocation = 0*/
			for (; Check < 3; Check++)
			{
				EEPROMDataLocation = UserLocation+Check;
				H_Eeprom_Write(ID[Check],0, EEPROMDataLocation);
			}
		
			for (Check = 0; Check < 4; Check++)
			{
				EEPROMDataLocation = UserLocation+Check+3;
				H_Eeprom_Write(Password[Check],0, EEPROMDataLocation);
			}
			NumberOfUsers++;
			H_Eeprom_Write(NumberOfUsers,0,100);
			
			H_Lcd_WriteString("User Added");
			H_Lcd_GoTo(1,0);
			H_Lcd_WriteString("No of Users");
			H_Lcd_WriteNumber(H_Eeprom_Read(0,100));
		}
	}
	else /*Number of users is equal or more than 10 users */
	{
		/*Print on LCD you reached max number of users*/
		H_Lcd_WriteString("Reached max");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Number Of Users");
	}
}

void DeleteUser (u8 NumberOfUsers, u8* ID)
{
	if (NumberOfUsers > 0)
	{
		u8 Counter = 0, UserLocation  = 0, PerfectMatch = 0;
	
		for (; Counter < NumberOfUsers; Counter++)
		{
			UserLocation = (Counter*7);
			u8 Check = 0, EEPROMData = 0, EEPROMDataLocation = 0, ValidEntries = 0;
			
			for (; Check < 3; Check++)
			{
				EEPROMDataLocation = UserLocation+Check;
				EEPROMData = H_Eeprom_Read(0, EEPROMDataLocation);
				if (ID[Check] == EEPROMData )
				{
					ValidEntries++;
				}
				
			}
			
			if (ValidEntries == 3)
			{
				/*print on LCD valid username*/
				H_Lcd_WriteString("Valid Username");
				_delay_ms(200);
				PerfectMatch ++;
				
			}
			if (PerfectMatch == 1)
			{
				u8 Check = 0, EEPROMDataLocation = 0;
				for (; Check < 3; Check++)
				{
					EEPROMDataLocation = UserLocation+Check;
					EEPROMData = H_Eeprom_Read(0,(EEPROMDataLocation+7));
					H_Eeprom_Write(EEPROMData,0, EEPROMDataLocation);
				}
				Check = 0;
				for (; Check < 4; Check++)
				{
					EEPROMDataLocation = UserLocation+Check+3;
					EEPROMData = H_Eeprom_Read(0,(EEPROMDataLocation+7));
					H_Eeprom_Write(EEPROMData,0, EEPROMDataLocation);
				}
				
				
			}
		}
		if (PerfectMatch == 1)
		{	
			NumberOfUsers--;
			H_Eeprom_Write(NumberOfUsers,0,100);
		}
		
	}
	
	else
	{
		/*print on LCD There is no any users yet*/
		
	}
	
}

u8 AdminLogin (u8* ID, u8* Password)
{
	H_Lcd_Clear();
	u8 ByteAddress = 0, ValidUsername = 0, ValidPassword = 0, Counter = 0, EEPROMData = 0, WrongEntry = 0;
	for (; Counter < 3; Counter++)
	{
		ByteAddress = 101+Counter;
		EEPROMData = H_Eeprom_Read(0,ByteAddress);
		if (ID[Counter] == EEPROMData)
		{
			ValidUsername++;

		}
		else
		{
			/*Do Nothing*/
			
// 			H_Lcd_WriteNumber(ValidUsername);
// 			_delay_ms(500);
		}
	//	ValidUsername++;
	}
	
	for (Counter = 0; Counter < 4; Counter++)
	{
		ByteAddress = 101+Counter+3;
		EEPROMData = H_Eeprom_Read(0,ByteAddress);
		if (Password[Counter] == EEPROMData)
		{
			ValidPassword++;
		}
		else
		{
			/*Do Nothing*/
		}
		
	}
	if ((ValidUsername == 3) && (ValidPassword == 4))
	{
		/*Print on LCD Login Successful*/
		H_Lcd_WriteString("Login Successful");
	}
	else if ((ValidPassword == 4) && (ValidUsername != 3)  )
	{
		/*Print on LCD invalid Username*/
		H_Lcd_WriteString("Invalid Username");
		WrongEntry++;
	}
	else if ((ValidUsername == 3) && (ValidPassword != 4))
	{
		/*Print on LCD Invalid Password*/
		H_Lcd_WriteString("Invalid Password");
		WrongEntry++;
	}
	else if ((ValidUsername != 3) && (ValidPassword != 4))
	{
		H_Lcd_WriteString("Invalid Username");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Invalid Password");
		/*Print on LCD Invalid Username & Invalid Password*/
		WrongEntry++;
	} 
	else
	{
		/*Do Nothing*/
	}
	return WrongEntry;
}

u8 UserLogin (u8* ID, u8* Password)
{
	u8 NumberOfUsers = H_Eeprom_Read(0,100);
	u8 UserLocation = 0, ValidUsername = 0, ValidPassword = 0, Counter = 0, EEPROMData = 0, WrongEntry = 0, EEPROMDataLocation = 0;
	if (NumberOfUsers > 0)
	{
		for (;Counter < NumberOfUsers; Counter++)
		{
			ValidPassword = 0;
			ValidUsername = 0;
			UserLocation = Counter*7;
			u8 Check = 0;
			
			for(;Check < 3; Check++)
			{	
				EEPROMDataLocation = UserLocation+ Check;
				EEPROMData = H_Eeprom_Read(0,(EEPROMDataLocation));
				if (ID[Check] == EEPROMData)
				{
					ValidUsername++;
				}

			}
			
			for (Check = 0;Check < 4; Check++)
			{
				EEPROMDataLocation = UserLocation+Check+3;
				EEPROMData = H_Eeprom_Read(0,(EEPROMDataLocation));
				if (Password[Check] == EEPROMData)
				{
					ValidPassword++;
				}

			}
			
			if (ValidUsername == 3 && ValidPassword == 4)
			{
				break;
			}
			else
			{
				ValidUsername = 0;
				ValidPassword = 0;
			}	
		}
		if (ValidUsername == 3 && ValidPassword == 4)
		{
			/*print on LCD Login Successful*/
			H_Lcd_WriteString("Login Successful");
		}
		else
		{
			/*print on LCD Invalid Username Or Password*/
			H_Lcd_WriteString("Invalid Username");
			H_Lcd_GoTo(1,0);
			H_Lcd_WriteString("Or Password");
			WrongEntry++;
		}
	}
	else
	{
		/*print on LCD There is no users added yet*/
		H_Lcd_WriteString("There is no users");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("added yet");
	}
	
	return WrongEntry;
}

/************************************************************************/
/*Parameters input array to save in it the input from keypad			*/
/*for the ID															*/
/************************************************************************/
void InputID (u8* ID)
{
	u8 KeyPressed = 0, DigitsEntered = 0;
	H_Lcd_GoTo(1,0);
	while (DigitsEntered < 3)
	{
		KeyPressed = H_KeyPad_Read();
		if (KeyPressed)
		{
			ID[DigitsEntered] = KeyPressed;
			/*Print on LCD Numbers Entered*/
			H_Lcd_WriteCharacter(ID[DigitsEntered]);
			DigitsEntered++;
		}
	}	
	_delay_ms(300);
}

/************************************************************************/
/*Parameters input array to save in it the input from keypad			*/
/*for the password														*/
/************************************************************************/
void InputPassword (u8* Password)
{
	H_Lcd_GoTo(1,0);
	u8 KeyPressed = 0, DigitsEntered = 0;
	while (DigitsEntered < 4)
	{
		KeyPressed = H_KeyPad_Read();
		if (KeyPressed)
		{
			Password[DigitsEntered] = KeyPressed; 
			/*Print on LCD Stars for every character entered*/
			H_Lcd_WriteCharacter('*');
			DigitsEntered++;
		}
	}
	_delay_ms(300);
}

u8 SwitchDoor (u8 DoorOpened)
{
	if (DoorOpened == 0)
	{
		H_Servo_SetAngel(120);
		DoorOpened = 1;
	}
	else
	{
		H_Servo_SetAngel(0);
		DoorOpened = 0;
	}
	return DoorOpened;
}

u8 ACSwitch (u8 ACOpened)
{
	if (ACOpened == 0)
	{
		u8 Temperature = 0;
		Temperature = H_Lm35_Read();
		if (Temperature >= 26)
		{
			H_Led_On(LED_0);
			H_Led_Off(LED_1);
			H_DcMotor_SetDirection(CW);
			H_DcMotor_Speed(100);
			H_DcMotor_Start();
		}
		else if (Temperature <= 21)
		{
			H_Led_On(LED_1);
			H_Led_Off(LED_0);
			H_DcMotor_Stop();
		}
		
		ACOpened = 1;
	}
	else
	{
		H_DcMotor_Stop();
		ACOpened = 0;
	}
		
	return ACOpened;
}

void CheckEEPROM (void)
{
	//char NumberOfUsers = 0;
	//NumberOfUsers = H_Eeprom_Read(0,100);
	//H_Lcd_WriteNumber(NumberOfUsers);
	//char counter = NumberOfUsers* 7;
	for(char i = 0; i<=16; i++)
	{
		H_Lcd_WriteCharacter(H_Eeprom_Read(0,i));
		//_delay_ms(250);
	//	H_Lcd_Clear();
	}
	H_Lcd_GoTo(1,0);
	for(char i = 17; i<33; i++)
	{
		H_Lcd_WriteCharacter(H_Eeprom_Read(0,i));
		//_delay_ms(250);
		//	H_Lcd_Clear();
	}
}

void DeleteAll (void)
{
	for (u8 i = 0; i<100 ; i++)
	{
	 	H_Eeprom_Write(0xFF,0,i);
	}
	H_Eeprom_Write(0,0,100);
}