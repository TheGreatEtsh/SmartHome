/*
 * SmartHome.c
 *
 * Created: 7/5/2022 12:34:53 AM
 * Author : Ahmed Hesham
 */ 

#define FIRST_PAGE				'0'
#define ADMIN_LOGIN_PAGE		'1'
#define USER_LOGIN_PAGE			'2'
#define WELCOME					 0
#define ADMIN_SECOND_PAGE		 1
#define USER_SECOND_PAGE		 2
#define ADMIN_THIRD_PAGE		 3
#define ADMIN_FOURTH_PAGE		 4
#define IDLE_STATE				 5
	

#include "Led.h"
#include "Buzzer.h"
#include "PushButton.h"
#include "Lcd.h"
#include "KeyPad.h"
#include "Servo.h"
#include "DcMotor.h"
#include "Lm35.h"
#include "Eeprom.h"
#include "Relay.h"
//#include "ExtInt.h"
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
void ACSwitch (u8* ACOpened);
void SwitchDoor (u8* DoorOpened);
void AppInit(void);
void CloseTheDoor(void);
void WelcomingMsg(void);
void A_Main_ExtInt0Exc (void);
void CheckAC(u8 ACState);
void CheckDoor(u8 DoorState);

u8 AppState = WELCOME;

/************************************************************************/
/*	Admin Password is 1234
	Admin UserName is 123							
	Admin Location is @ 101 byte address page 1							*/
/************************************************************************/
/*int main ()
{
	H_Eeprom_Init();
	//DeleteAll();
	//AppInit();
	while(1)
	{
		
	}
	return 0;
}*/

/************************************************************************/
/*                                                                      */
/************************************************************************/

int main ()
{
	H_Lcd_Init();
	H_KeyPad_Init();
	H_Eeprom_Init();
	H_DcMotor_Init();
	H_Lm35_Init();
	H_Servo_Init();
	H_Led_Init(LED_0);
	H_Led_Init(LED_1);
	H_Buzzer_Init();
	H_PushButton_Init();
	H_Relay_Init();
	
	//M_ExtInt_Init(INT0);
	
//	M_ExtInt_SetCallBackInt0(A_Main_ExtInt0Exc);
	
	char NumberOfUsers = 0;

	NumberOfUsers = H_Eeprom_Read(0,100);
 	u8 UserID		[3]	= {0,0,0};
 	u8 UserPassword [4] = {0,0,0,0};	
	u8 KeyPressed = 0;
	u8 Trails = 0;
	u8 DoorState = 0;
	u8 ACState = 0;
	

	
	
	while (1)
	{
		switch(AppState)
		{
			case WELCOME :
				WelcomingMsg();
				AppState = FIRST_PAGE;
				break;
			case FIRST_PAGE:
				H_Lcd_Clear();
				H_Lcd_WriteString("    Welcome     ");
				H_Lcd_GoTo(1,0);
				H_Lcd_WriteString("1:Admin 2:User");
				KeyPressed = H_KeyPad_Read();
				while(!KeyPressed)
				{
					KeyPressed = H_KeyPad_Read();
					if (KeyPressed == '1')
					{	
						AppState = ADMIN_LOGIN_PAGE;
					}
					else if (KeyPressed == '2')
					{
						AppState = USER_LOGIN_PAGE;
					}
					else
					{
						/*DO NOTHING*/
					}
					
				}
				break;
			
			case ADMIN_LOGIN_PAGE:
				Trails = 0;
				H_Lcd_Clear();
				H_Lcd_WriteString("Welcome Admin");
				_delay_ms(1000);
				H_Lcd_Clear();
 				while(Trails < 3)
				{
					H_Lcd_WriteString("Enter Username");
 					InputID(UserID);
					H_Lcd_Clear();
					H_Lcd_WriteString("Enter Password");
					InputPassword(UserPassword);
					
					if (AdminLogin(UserID,UserPassword) == 0)
					{
						AppState = ADMIN_SECOND_PAGE;
						break;
					}
					else 
					{
						Trails++;
						if (Trails == 3)
						{
							H_Buzzer_On();
							CloseTheDoor();
							KeyPressed = H_PushButton_Read();
							while (1)
							{
								if (H_PushButton_Read() == PRESSED)
								{
									_delay_ms(80);
									if (H_PushButton_Read() == PRESSED)
									{
										while(H_PushButton_Read() == PRESSED);	
										H_Buzzer_Off();
										AppState = WELCOME;
										break;
									}
								}	
							}
						}
					}
				}
				Trails = 0;
				break;
				
			
			case USER_LOGIN_PAGE:
				Trails = 0;
				H_Lcd_Clear();
				H_Lcd_WriteString("Welcome User");
				_delay_ms(1000);
				while(Trails < 3)
				{
					H_Lcd_Clear();
					H_Lcd_WriteString("Enter Username");
					InputID(UserID);
					H_Lcd_Clear();
					H_Lcd_WriteString("Enter Password");
					InputPassword(UserPassword);
					u8 LoginResult = UserLogin(UserID,UserPassword);
					
					if (LoginResult == 0)
					{
						AppState = USER_SECOND_PAGE;
						break;
					}
					else if (LoginResult == 2)
					{
						AppState = FIRST_PAGE;
						break;
					}
					else if (LoginResult == 1)
					{
						Trails++;
						if (Trails == 3)
						{
							H_Buzzer_On();
							CloseTheDoor();
							H_Lcd_Clear();
							KeyPressed = H_PushButton_Read();
							while (1)
							{
								if (H_PushButton_Read() == PRESSED)
								{
									_delay_ms(80);
									if (H_PushButton_Read() == PRESSED)
									{
										while(H_PushButton_Read() == PRESSED);
										H_Buzzer_Off();
										AppState = WELCOME;
										break;
									}
								}
							}
						}
					}
				}
				Trails = 0;
				break;
			
			case ADMIN_SECOND_PAGE:
				H_Lcd_Clear();
				H_Lcd_WriteString(" 1:Switch Door  ");
				H_Lcd_GoTo(1,0);
				H_Lcd_WriteString("2:AddUser 3:Next");
				KeyPressed = H_KeyPad_Read();
				while(!KeyPressed)
				{
					KeyPressed = H_KeyPad_Read();
					if (KeyPressed == '1')
					{
						SwitchDoor(&DoorState);
					}
					else if (KeyPressed == '2')
					{
						H_Lcd_Clear();
						H_Lcd_WriteString("Enter Username");
						InputID(UserID);
						H_Lcd_Clear();
						H_Lcd_WriteString("Enter Password");
						InputPassword(UserPassword);
						AddUser(UserID, UserPassword);
					}
					else if (KeyPressed == '3')
					{
						AppState = ADMIN_THIRD_PAGE;
					}		
				}
				break;
			case ADMIN_THIRD_PAGE:
				H_Lcd_Clear();        
				H_Lcd_WriteString(" 4:Remove User  ");
				H_Lcd_GoTo(1,0);
				H_Lcd_WriteString("5:Back 6:Next");
				KeyPressed = H_KeyPad_Read();
				while(!KeyPressed)
				{
					KeyPressed = H_KeyPad_Read();
					if (KeyPressed == '4')
					{
						H_Lcd_Clear();
						H_Lcd_WriteString("Enter Username");
						InputID(UserID);
						H_Lcd_Clear();
						DeleteUser(NumberOfUsers,UserID);
					}
					else if (KeyPressed == '5')
					{
						AppState = ADMIN_SECOND_PAGE;
					}
					else if (KeyPressed == '6')
					{
						AppState = ADMIN_FOURTH_PAGE;
					}
				}
				break;
			case ADMIN_FOURTH_PAGE:
				H_Lcd_Clear();
				H_Lcd_WriteString(" 7:Switch AC  ");
				H_Lcd_GoTo(1,0);
				H_Lcd_WriteString("8:1stPage 9:Done");
				KeyPressed = H_KeyPad_Read();
				while(!KeyPressed)
				{
					KeyPressed = H_KeyPad_Read();
					if (KeyPressed == '7')
					{
						ACSwitch(&ACState);
					}
					else if (KeyPressed == '8')
					{
						AppState = ADMIN_SECOND_PAGE;
					}
					else if (KeyPressed == '9')
					{
						AppState = IDLE_STATE;
					}
				}
				break;
				
			case IDLE_STATE:
				KeyPressed = H_KeyPad_Read();
				while(!KeyPressed)
				{
					KeyPressed = H_KeyPad_Read();
					CheckAC(ACState);
					CheckDoor(DoorState);
				}
				AppState = WELCOME;
				break;
				
			case USER_SECOND_PAGE :
				H_Lcd_Clear();
				H_Lcd_WriteString(" 1:Switch AC  ");
				H_Lcd_GoTo(1,0);
				H_Lcd_WriteString("   2: Done    ");
				KeyPressed = H_KeyPad_Read();
				while(!KeyPressed)
				{
					KeyPressed = H_KeyPad_Read();
					if (KeyPressed == '1')
					{
						ACSwitch(&ACState);
					}
					else if (KeyPressed == '2')
					{
						AppState = IDLE_STATE;
					}
				}
				break;	
		}
			
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
					H_Lcd_Clear();
					H_Lcd_WriteString("Invalid Username");
					_delay_ms(500);
				}
			}
		
		
			if (WrongEntry == 0)
			{
				/*print on LCD valid entry*/
				H_Lcd_Clear();
				H_Lcd_WriteString("Valid Entry");
				_delay_ms(500);
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
			H_Lcd_Clear();
			H_Lcd_WriteString("Valid Entry");
			_delay_ms(500);
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
			
			H_Lcd_Clear();
			H_Lcd_WriteString("User Added");
			H_Lcd_GoTo(1,0);
			H_Lcd_WriteString("No of Users ");
			H_Lcd_WriteNumber(H_Eeprom_Read(0,100));
			_delay_ms(500);
		}
	}
	else /*Number of users is equal or more than 10 users */
	{
		/*Print on LCD you reached max number of users*/
		H_Lcd_Clear();
		H_Lcd_WriteString("Reached max");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Number Of Users");
		_delay_ms(500);
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
		_delay_ms(500);
	}
	else if ((ValidPassword == 4) && (ValidUsername != 3)  )
	{
		/*Print on LCD invalid Username*/
		H_Lcd_WriteString("Invalid Username");
		_delay_ms(500);
		WrongEntry++;
	}
	else if ((ValidUsername == 3) && (ValidPassword != 4))
	{
		/*Print on LCD Invalid Password*/
		H_Lcd_WriteString("Invalid Password");
		_delay_ms(500);
		WrongEntry++;
	}
	else if ((ValidUsername != 3) && (ValidPassword != 4))
	{
		H_Lcd_WriteString("Invalid Username");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Invalid Password");
		_delay_ms(500);
		/*Print on LCD Invalid Username & Invalid Password*/
		WrongEntry++;
	} 
	else
	{
		/*Do Nothing*/
	}
	_delay_ms(300);
	H_Lcd_Clear();
	_delay_ms(300);
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
			H_Lcd_Clear();
			H_Lcd_WriteString("Invalid Username");
			H_Lcd_GoTo(1,0);
			H_Lcd_WriteString("Or Password");
			_delay_ms(1000);
			WrongEntry++;
			
		}
	}
	else
	{
		/*print on LCD There is no users added yet*/
		H_Lcd_Clear();
		H_Lcd_WriteString("There is no users");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("added yet");
		_delay_ms(1000);
		WrongEntry = 2;
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
void SwitchDoor (u8* DoorOpened)
{
	if (*DoorOpened == 0)
	{
		H_Lcd_Clear();
		H_Lcd_GoTo(0,0);
		H_Lcd_WriteString("Door Is Opened");
		H_Relay_On();
		H_Servo_SetAngel(120);
		_delay_ms(500);
		*DoorOpened = 1;
	}
	else
	{
		H_Lcd_Clear();
		H_Lcd_GoTo(0,0);
		H_Lcd_WriteString("Door Is Closed");
		H_Relay_Off();
		H_Servo_SetAngel(0);
		_delay_ms(500);
		*DoorOpened = 0;
	}
	
}

void ACSwitch (u8* ACOpened)
{
	H_Led_On(LED_1);
	if (*ACOpened == 0)
	{
		u8 Temperature = 0;
		Temperature = H_Lm35_Read();
		if (Temperature > 26)
		{
			H_Led_On(LED_0);
			H_Led_Off(LED_1);
			H_DcMotor_SetDirection(CW);
			H_DcMotor_Speed(100);
			H_DcMotor_Start();
		}
		else if (Temperature < 21)
		{
			H_Led_On(LED_1);
			H_Led_Off(LED_0);
			H_DcMotor_Stop();
		}
		
		*ACOpened = 1;	
		
	}
	else
	{
		H_DcMotor_Stop();
		H_Led_Off(LED_1);
		H_Led_Off(LED_0);
		*ACOpened = 0;
	}
		
	
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
	for (u8 i = 0; i<0xFF ; i++)
	{
	 	H_Eeprom_Write(0xFF,0,i);
	}
	H_Eeprom_Write(0,0,100);
}

void AppInit(void)
{
	H_Eeprom_Write('1',0,101);
	H_Eeprom_Write('2',0,102);
	H_Eeprom_Write('3',0,103);	
	H_Eeprom_Write('1',0,104);	
	H_Eeprom_Write('2',0,105);	
	H_Eeprom_Write('3',0,106);	
	H_Eeprom_Write('4',0,107);		
}

void WelcomingMsg(void)
{
	H_Lcd_Clear();
	H_Lcd_WriteString("Welcome to Smart");
	H_Lcd_GoTo(1,0);
	H_Lcd_WriteString("Home Project");
	_delay_ms(2000);
	H_Lcd_Clear();
}

void CloseTheDoor(void)
{
	H_Servo_SetAngel(0);
	H_Relay_Off();
}

/*void A_Main_ExtInt0Exc (void)
{
	
}
*/
void CheckAC(u8 ACState)
{
	if (ACState == 1)
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
		else if (Temperature < 21)
		{
			H_Led_On(LED_1);
			H_Led_Off(LED_0);
			H_DcMotor_Stop();
		}
		H_Lcd_Clear();
		H_Lcd_WriteString("AC Opened");
	}
	else 
	{
		H_DcMotor_Stop();
		H_Led_Off(LED_1);
		H_Led_Off(LED_0);
		H_Lcd_Clear();
		H_Lcd_WriteString("AC Closed");	
	}
}

void CheckDoor(u8 DoorState)
{
	if (DoorState == 1)
	{
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Door Is Opened");
		H_Servo_SetAngel(120);
		H_Relay_On();
		_delay_ms(500);
	}
	else
	{
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Door Is Closed");
		H_Servo_SetAngel(0);
		H_Relay_Off();
		_delay_ms(500);
		
		
	}
	
}