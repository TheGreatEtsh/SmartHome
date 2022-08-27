/*
 * SmartHome.c
 *
 * Created: 7/5/2022 12:34:53 AM
 * Author : Ahmed Hesham
 */ 
#include "StdTypes.h"
#include "Reg.h"
#include "Led.h"
#include "Buzzer.h"
#include "Lcd.h"
#include "KeyPad.h"
#include "ExtInt.h"
#include "Servo.h"
#include "DcMotor.h"
#include "Lm35.h"
#include "Eeprom.h"
#include "Uart.h"
#include "Hc05.h"

#define F_CPU 16000000UL
#include <util/delay.h>
/***********************************_PROTOTYPES_*******************************/
void InputID		(u8* ID);
void InputPassword	(u8* Password);
u8 AdminLogin (u8* ID, u8* Password);
void AddUser (u8* ID, u8* Password);
u8 UserLogin ( u8* ID, u8* Password);
void CheckEEPROM(void);
void DeleteUser (u8 NumberOfUsers, u8* ID);
void DeleteAll (void);

void Login_System_Init();
void BreakDownMode();
void welcomeScreen_Login();
void AC_Switch();
void Door_Switch();
void Relay_Switch();
void Admin_MainScreen();
void UserMode();
void User_MainScreen();

/************************************************************************/
/*	Admin Password is 1234
	Admin UserName is 123							
	Admin Location is @ 101 byte address page 1							*/
/************************************************************************/

/***********************************_MACOS_*******************************/
#define Open	1
#define Close	0
#define On	1
#define Off	0
#define FALSE	0
#define TRUE	1
/***********************************_Code_*******************************/
u8 Air_cond= Off;
u8 Door = Open;
u8 Lamps = Off;
u8 FRIST_USE= TRUE;

u8 Admin_UserName[3]="111";
u8 Admin_Pass[4]="1234";
u8 Admin_ByteAddress=101;
u8 PageAddress=1;


int main ()
{
	H_Lcd_Init();
	H_KeyPad_Init();
	H_Eeprom_Init();
	
	H_Hc05_Init();
	H_Lm35_Init();
	H_DcMotor_Init();
	H_Servo_Init();
	Door_Switch();
	//System_Init();
	
	if (FRIST_USE	==	TRUE)
	{
		Login_System_Init();
		FRIST_USE=FALSE;
	}
	else
	{
		/*DO Nothing*/
	}
		
	welcomeScreen_Login();
	Admin_MainScreen();
	
	while (1)
	{
	}
	
	return 0;
}

void Login_System_Init(){
	
	//Save Init Admin Username and pass to EEPROM
	//Save Admin ID
	u8 UserID[3]={0,0,0}, UserPassword[4]={0,0,0,0};
		
	InputID(UserID);
	InputPassword(UserPassword);
	AddUser (UserID, UserPassword);
}

void BreakDownMode()
{
	H_Buzzer_Init();
	H_Buzzer_On();
	while(1);
}

void welcomeScreen_Login(){
	_delay_ms(500);
	H_Lcd_Clear();
	H_Lcd_GoTo(0,4);
	H_Lcd_WriteString("Welcome");
	
	H_Lcd_GoTo(1,3);
	H_Lcd_WriteString("Smart Home");
	_delay_ms(3000);
	
	u8	Trial=0, SuccessLogin=0, UserID[3]={0,0,0}, UserPassword[4]={0,0,0,0};
	while(SuccessLogin != 1){
		Trial++;
		if(Trial > 3)
		{
			H_Lcd_Clear();
			H_Lcd_GoTo(0,2);
			H_Lcd_WriteString("Warning...");
			BreakDownMode();
		}
		else
		{
			SuccessLogin = AdminLogin(UserID,UserPassword);
		}
	}
}

void AC_Switch()
{
	if(Air_cond == Off)
	{
		Air_cond == On;
		u8 Local_u8_Speed=0;
		H_DcMotor_SetDirection(CW);
		// By timer
		while(1)
		{
			Local_u8_Speed=(((f32)H_Lm35_Read()/500) * 100);
			H_DcMotor_SetSpeed(Local_u8_Speed);
			H_DcMotor_Start();
			H_Lcd_Clear();
			H_Lcd_WriteString("A/C On");
		}
		
	}
	else if(Air_cond == On)
	{
		Air_cond == Off;
		H_DcMotor_Stop();
		H_Lcd_Clear();
		H_Lcd_WriteString("A/C Off");
	}
}

void Door_Switch()
{
	if(Door == Open)
	{
		Door = Close;
		H_Servo_SetAngel(0);
		H_Lcd_Clear();
		H_Lcd_WriteString("Door Closed");
	}
	else if(Door == Close)
	{
		Door = Open;
		H_Servo_SetAngel(120);
		H_Lcd_Clear();
		H_Lcd_WriteString("Door Opened");
	}
}

void Relay_Switch()
{
	
}

void Admin_MainScreen()
{
	u8 Bleutooth_Data = 0, NumberOfUsers=0;
	while (1)
	{
		u8 UserId[3]={0}, UserPass[4]={0};
		Bleutooth_Data = H_Hc05_Rec();
		switch(Bleutooth_Data)
		{
			case 'A':		 AC_Switch();					break;
			case 'B':	InputID(UserId); InputPassword(UserPass);
							AddUser(UserId,UserPass);		break;
			case 'c':	NumberOfUsers = H_Eeprom_Read(0,100);
							InputID(UserId);
							DeleteUser(NumberOfUsers,UserId);	break;
							
			case 'D':		 Door_Switch();					break;
			case 'R':			Relay_Switch();				break;
			case 'U':			UserMode();					break;
			default:									    break;
		}
	}
	
}

void UserMode()
{
	u8 UserId[3]={0}, UserPass[4]={0};
	InputID(UserId);
	InputPassword(UserPass);
	
	u8	Trial=0, SuccessLogin=0;
	while(SuccessLogin != 1){
		Trial++;
		if(Trial > 3)
		{
			H_Lcd_Clear();
			H_Lcd_GoTo(0,2);
			H_Lcd_WriteString("Warning...");
			BreakDownMode();
		}
		else
		{
			SuccessLogin = UserLogin(UserId,UserPass);
		}
	}
	User_MainScreen();
}


void User_MainScreen()
{
	u8 key;
	while (1)
	{
		key= H_KeyPad_Read();
		if(key!=0)
		{
			switch(key)
			{
				case '*':	 AC_Switch();	 break;
				case '#':	Relay_Switch();	 break;
				default:				    break;
			}		
		}		
	}
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
			_delay_ms(100);
			H_Lcd_Clear();
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
		H_Lcd_Clear();
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
	H_Lcd_WriteString("User ID:");

	u8 UserID[3]={0,0,0}, UserPassword[4]={0,0,0,0};
	InputID(UserID);
	
	H_Lcd_GoTo(1,0);
	H_Lcd_WriteString("Password:");
	InputPassword(UserPassword);

	u8 ByteAddress = 0, ValidUsername = 0, ValidPassword = 0, Counter = 0, EEPROMData = 0;
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
		return 1;
	}
	else if ((ValidPassword == 4) && (ValidUsername != 3)  )
	{
		/*Print on LCD invalid Username*/
		H_Lcd_WriteString("Invalid Username");
		return 0;
	}
	else if ((ValidUsername == 3) && (ValidPassword != 4))
	{
		/*Print on LCD Invalid Password*/
		H_Lcd_WriteString("Invalid Password");
		return 0;
	}
	else if ((ValidUsername != 3) && (ValidPassword != 4))
	{
		H_Lcd_WriteString("Invalid Username");
		H_Lcd_GoTo(1,0);
		H_Lcd_WriteString("Invalid Password");
		/*Print on LCD Invalid Username & Invalid Password*/
		return 0;
	} 
	else
	{
		/*Do Nothing*/
	}
	return 0;
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
	H_Lcd_Clear();
	H_Lcd_WriteString("User ID:");
	
	u8 KeyPressed = 0, DigitsEntered = 0;
	while (DigitsEntered < 3)
	{
		KeyPressed = H_KeyPad_Read();
		if (KeyPressed)
		{
			ID[DigitsEntered] = KeyPressed;
			/*Print on LCD Numbers Entered*/
			H_Lcd_GoTo(0,9+DigitsEntered);
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
	H_Lcd_WriteString("Password:");
	H_Lcd_GoTo(1,9);
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