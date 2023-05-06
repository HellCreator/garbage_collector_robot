/*******************************************************************************
  Glowna funkcja przykladowego programu do plytki MOBOT-EXP MB w raz z modulem 
						rozszerzen MOBOT-EXP MCB
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
Maciej Pabel
m.pabel@wobit.com.pl
WObit 2009

Opis:
	Plik zawiera glowna funkcje programu oraz funkcje inicjalizujaca 
wyprowadzenia mikrokontrolera. W funkcji glownej wywolywane sa najpierw funkcje
inicjalizujace poszczegolne uzyte uklady peryferyjne mikrokontrolera, nastepnie
w glownej petli oczekuje na odbior polecen przez UART odpowiednio na nie reagujac.
Program wspolpracuje z programem steruj¹cym Explorer.exe (wymagany joystick i modul 
radiowy do PC MOBOT-RCR-USB-V2) lub MOBOT Virtual Joystick
*******************************************************************************/
#include "Main.h"


/*******************************************************************************
Funkcja:
	void ioinit(void)
Argumenty: 
	- brak
Opis:
	Wstepna Inicjalizacja wejsc/wyjsc
*******************************************************************************/
void ioinit(void)
{
	M1_DDR |= (1<<M1_IN1)|(1<<M1_IN2)|(1<<M2_IN1)|(1<<M2_IN2); //jako wyjscia
	M1_P_DDR |= (1<<M1_P)|(1<<M2_P); //piny od PWMa jako wyjscia

	PG3_SLEEP_DDR |= (1<<PG3_SLEEP); //jako wyjscie
	PG3_SLEEP_PORT &= ~(1<<PG3_SLEEP); //uspienie silnikow	
	FS_AB_DDR &= ~(1<<FS_AB);  //pinod FS jako wejscie	
	
	STEP_CLK_DDR |= (1<<STEP_CLK); //jako wyjscie
	STEP_DIR_DDR |= (1<<STEP_DIR); //jako wyjscie
	STEP_ENABLE_DDR |= (1<<STEP_ENABLE); //jako wyjscie
	
	LED1_DDR |= (1<<LED1)|(1<<LED2); //jako wyjscia
	
	M3_DDR |= (1<<M3_FIN)|(1<<M3_RIN); //jako wyjscia
	M3_P_DDR |= (1<<M3_P);//jako wyjscie
	
	M4_DDR |= (1<<M4_FIN)|(1<<M4_RIN);//jako wyjscia
	M4_P_DDR |= (1<<M4_P); //jako wyjscie
	
	M5_DDR |= (1<<M5_FIN)|(1<<M5_RIN); //jako wyjscia
	M56_P_DDR |= (1<<M56_P); //jako wyjscie
	M6_DDR |= (1<<M6_FIN)|(1<<M6_RIN); //jako wyjscie
	
	KEY_DDR |= (1<<KEY1)|(1<<KEY2); //klucze tranzystorowe jako wyjscia
	DDRF = 0x00; //caly port jako wejscie (konieczne gdy uzywamy ADC)
}

/*******************************************************************************
Funkcja:
	int main(void)
Argumenty: 
	- brak
Opis:
	Glowna funkcja programu
*******************************************************************************/
int main(void)
	{    
	#define OFFSET		200
	#define SIZE		30

	ioinit();	
	UART0_init();		
    MOTOR_init(); 
	TIMER_init(); 

	DDRE |=   ((1<<2)|(1<<6));
	//PORTE &=~ ((1<<2)|(1<<4));

	PORTE |= ((1<<2)|(1<<6));

	
	sei();	//globalne wlaczenie przerwan    

	int i;

	LED1_ON;
	LED2_ON;
	
	for(i=0;i<200;i++)
		{
		_delay_ms(10);
		}

	
	LED1_OFF;
	LED2_OFF;
	

	//UART0_transmit_byte(0x43);
	//UART0_transmit_byte(0x78);
	//UART0_transmit_byte(0x1E);
	//UART0_transmit_byte(0x07);
	//UART0_transmit_byte(6); //ustawienie kanalu 6 dla modulu radiowego 

	
	while(1) //glowna petla programu
		{
		if(UART0_data_in_rx_buffer())
			{													
			data = UART0_receive_byte(); 
			//-----odbieranie danych----------------------------------------------
    		if(data == 0xFC)                                                  	//poczatek ramki
   				{
        		start_recive = 1;
        		us_rx_buffer[0] = data;
        		to_recive = 1;
        		}
			else                                                                //inny znak
				{
         		if(start_recive)
					{
            		us_rx_buffer[to_recive] = data;
            		to_recive++;
            		}
				}
			//-----interpretacja--------------------------------------------------
			if(to_recive == 4 && start_recive) 
				{
				static int e=0;
				e++;
				if(e>10)
					{
					e = 0;
					PORTE^=(1<<2);
					}

				
				
				//PORTE&=~(1<<2);

				left_speed  = us_rx_buffer[3];
				right_speed = us_rx_buffer[2];			
				
				left_speed  = left_speed  - 100;
				right_speed = right_speed - 100;
			
				#ifdef BINARY_MODE				
					if(abs(left_speed) > SIZE && abs(right_speed) > SIZE)
						{
						LED1_ON;
						
						if(left_speed > 0 && right_speed > 0)
							{
							left_speed  = +255;
							right_speed = +255;
							}
						else if(left_speed < 0 && right_speed < 0)
							{
							left_speed  = -255;
							right_speed = -255;
							}
						else if(left_speed > 0 && right_speed < 0)
							{
							left_speed  = -255;
							right_speed = +255;
							}
						else if(left_speed < 0 && right_speed > 0)
							{
							left_speed  = +255;
							right_speed = -255;
							}
						}
					else
						{ 
						LED1_OFF;
						}				
				#else
					left_speed  = (-1)*left_speed  * 255 / 100;
					right_speed = (-1)*right_speed * 255 / 100;
				#endif //BINARY_MODE
				
				MOTOR_drive(left_speed, right_speed); 
				start_recive = 0;
				timeout = 7;
				
				//PORTE|=(1<<2);
				}  			
			}
		}		
	}
