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
#include "Eeprom.h"
#include "Servo.h"
#include "DcMotor.h"
#include "Lm35.h"
// int main(void)
// {
// 	H_Lcd_Init();
// 	H_KeyPad_Init();
// 	H_Led_Init(LED_0);
// 	H_Buzzer_Init();
// 	u8 Welcoming	= 1;
// 	u8 Key			= 0;
// 	u8 MsgPrinted	= 0;
// 	u8 AdminLogin	= 0;
//     
// 	while (Welcoming)
// 	{
// 		if (!MsgPrinted)
// 		{
// 			H_Lcd_WriteString("Welcome to the  office");
// 			MsgPrinted = 1;
// 		}
// 		Key = H_KeyPad_Read();
// 		if (Key == '7')
// 		{
// 			Welcoming = 0;
// 			MsgPrinted = 0;
// 			AdminLogin = 1;
// 		}
// 		while (AdminLogin)
// 		{
// 			if (!MsgPrinted)
// 			{
// 				H_Lcd_Clear();
// 				H_Lcd_WriteString("Enter 8 Please");
// 				MsgPrinted = 1;
// 			}	
// 			Key = H_KeyPad_Read();
// 			if (Key == '8')
// 			{
// 				H_Buzzer_On();
// 				H_Led_On(LED_0);
// 			}
// 		}
// 	}
//     while (1) 
//     {
//     }
// 	return 0;
// }

int main (void)
{
	H_KeyPad_Init();
	H_Lcd_Init();
	
	u8 Key = 0;
	
	while(1)
	{
		Key = H_KeyPad_Read();
		if (Key)
		{
			H_Lcd_WriteCharacter(Key);
		}
				
	}
	return 0;
}

void AddUser (u8 NumberOfUsers, u8* ID, u8* Password)
{
	if (NumberOfUsers <10)
	{
		u8 Counter = 1, UserLocation  = 0; 
	
		if (NumberOfUsers > 0)
		{
			u8 WrongEntry = 0;
		
			for (; Counter <= NumberOfUsers; Counter++)
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
				
				}
			}
		
		
			if (WrongEntry == 0)
			{
				/*print on LCD valid entry*/
				NumberOfUsers++;
				u8 Check = 0, EEPROMDataLocation = 0;
				for (; Check < 3; Check++)
				{
					EEPROMDataLocation = UserLocation+Check;
					H_Eeprom_Write(ID[Check],0, EEPROMDataLocation);
				}
		
				for (; Check < 7; Check++)
				{
					EEPROMDataLocation = UserLocation+Check;
					H_Eeprom_Write(Password[Check],0, EEPROMDataLocation);
				}
				H_Eeprom_Write(NumberOfUsers,0,100);
			}
		}
		else /*There isn't any user yet*/
		{
			NumberOfUsers++;
			u8 Check = 0, EEPROMDataLocation = 0;
			for (; Check < 3; Check++)
			{
				EEPROMDataLocation = UserLocation+Check;
				H_Eeprom_Write(ID[Check],0, EEPROMDataLocation);
			}
		
			for (; Check < 7; Check++)
			{
				EEPROMDataLocation = UserLocation+Check;
				H_Eeprom_Write(Password[Check],0, EEPROMDataLocation);
			}
			H_Eeprom_Write(NumberOfUsers,0,100);
		}
	}
	else /*Number of users is equal or more than 10 users */
	{
		/*Print on LCD you reached max number of users*/
	}
}





void DeleteUser (u8 NumberOfUsers, u8* ID)
{
	if (NumberOfUsers > 0)
	{
		u8 Counter = 1, UserLocation  = 0, PerfectMatch = 0;
	
		for (; Counter <= NumberOfUsers; Counter++)
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
				PerfectMatch ++;
				
			}
		}
		if (PerfectMatch == 1)
		{
			NumberOfUsers--;
			u8 Check = 0, EEPROMDataLocation = 0;
			for (; Check < 3; Check++)
			{
				EEPROMDataLocation = UserLocation+Check;
				H_Eeprom_Write(0xFF,0, EEPROMDataLocation);
			}
		
			for (; Check < 7; Check++)
			{
				EEPROMDataLocation = UserLocation+Check;
				H_Eeprom_Write(0xFF,0, EEPROMDataLocation);
			}
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
		}
		
	}
	
	for (; Counter < 7; Counter++)
	{
		ByteAddress = 101+Counter;
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
	}
	else if ((ValidUsername != 3) && (ValidPassword == 4))
	{
		/*Print on LCD invalid Username*/
		WrongEntry++;
	}
	else if ((ValidUsername == 3) && (ValidPassword != 4))
	{
		/*Print on LCD Invalid Password*/
		WrongEntry++;
	}
	else if ((ValidUsername != 3) && (ValidPassword != 4))
	{
		/*Print on LCD Invalid Username & Invalid Password*/
		WrongEntry++;
	} 
	else
	{
		/*Do Nothing*/
	}
	return WrongEntry;
}


u8 UserLogin (u8 NumberOfUsers, u8* ID, u8* Password)
{
	u8 UserLocation = 0, ValidUsername = 0, ValidPassword = 0, Counter = 1, EEPROMData = 0, WrongEntry = 0;
	if (NumberOfUsers > 0)
	{
		for (;Counter <= NumberOfUsers; Counter++)
		{
			UserLocation = Counter*7;
			u8 EEPROMCounter = 0;
			
			for(;EEPROMCounter < 3; EEPROMCounter++)
			{	
				EEPROMData = H_Eeprom_Read(0,UserLocation);
				if (ID[EEPROMCounter] == EEPROMData)
				{
					ValidUsername++;
				}
			}
			
			for (;EEPROMCounter < 7; EEPROMCounter++)
			{
				EEPROMData = H_Eeprom_Read(0,UserLocation);
				if (Password[EEPROMCounter] == EEPROMData)
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
		}
		else
		{
			/*print on LCD Invalid Username Or Password*/
			WrongEntry++;
		}
	}
	else
	{
		/*print on LCD There is no users added yet*/
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
	
	while (DigitsEntered < 3)
	{
		KeyPressed = H_KeyPad_Read();
		if (KeyPressed)
		{
			KeyPressed = ID[DigitsEntered];
			/*Print on LCD Numbers Entered*/
			DigitsEntered++;
		}
	}	
}

/************************************************************************/
/*Parameters input array to save in it the input from keypad			*/
/*for the password														*/
/************************************************************************/
void InputPassword (u8* Password)
{
	u8 KeyPressed = 0, DigitsEntered = 0;
	while (DigitsEntered < 4)
	{
		KeyPressed = H_KeyPad_Read();
		if (KeyPressed)
		{
			KeyPressed = Password[DigitsEntered];
			/*Print on LCD Stars for every character entered*/
			DigitsEntered++;
		}
	}
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

