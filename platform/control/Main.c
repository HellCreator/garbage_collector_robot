/*******************************************************************************
  Glowna funkcja przykladowego programu do plytki MOBOT-EXP MB w raz z modulem 
						rozszerzen MOBOT-EXP MCB
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Plik zawiera glowna funkcje programu oraz funkcje inicjalizujaca 
wyprowadzenia mikrokontrolera. W funkcji glownej wywolywane sa najpierw funkcje
inicjalizujace poszczegolne uzyte uklady peryferyjne mikrokontrolera, nastepnie
w glownej petli oczekuje na odbior polecen przez UART odpowiednio na nie reagujac.
Program mo¿na testowaæ za pomoc¹ dowolnego terminala.
Ustawienia portu COM:
57600 8N1
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
    unsigned char rcv;
    char tmp[8];
    int itmp;
	    
    ioinit();	
	UART0_init();					  
    UART1_init();
    TIMER_init();
    MOTOR_init();
    MOTOR34_init();
    MOTOR56_init();
    STEP_init();
    ADC_init();
    
    sei();	//globalne wlaczenie przerwan
    
	ADC_start_conversion();
	
	UART0_print("UART0 test\r\n");
	UART1_print("UART1 test\r\n");
	LED2_OFF;
	
	for(;;) //glowna petla programu
	{
		if(UART1_data_in_rx_buffer())
		{
			switch(rcv = UART1_receive_byte())
			{
				case 'v':
					UART1_print("Podaj nowa predkosc (-255 - 255): ");
					UART1_gets(tmp,8);
					itmp = atoi(tmp);
					UART1_print("\r\n");
					MOTOR_drive(itmp,itmp);
					break;		
				case 'b':
					MOTOR_break();						
					break;
				case 'l':
					MOTOR_sleep();
					break;
				case 'e':
					STEP_enable();
					break;
				case 'd':
					STEP_disable();
					break;		
				case 'w':
					STEP_start();
					break;
				case 's':
					STEP_stop();
					break;	
				case 'p':
					UART1_print("Podaj nowa pozycje: ");
					UART1_gets(tmp,8);
					itmp = atoi(tmp);
					UART1_print("\r\n");
					STEP_go_to_pos(itmp);
					break;																	
				case 'j':
					UART1_print("Podaj nowa predkosc (-255 - 255): ");
					UART1_gets(tmp,8);
					itmp = atoi(tmp);
					UART1_print("\r\n");
					MOTOR3_set_speed(itmp);	
					MOTOR4_set_speed(itmp);					
					break;
				case 'k':
					MOTOR3_break();
					MOTOR4_break();
					break;	
				case 'g':
					MOTOR5_left();
					MOTOR6_left();
					break;
				case 'h':
					MOTOR5_right();
					MOTOR6_right();
					break;	
				case 't':
					MOTOR5_stop();
					MOTOR6_stop();				
					break;	
				case 'y':
					MOTOR5_break();
					MOTOR6_break();					
					break;
				case 'o':
					PORTD |= (1<<KEY1)|(1<<KEY2); //wlaczenie kluczy tranzystorowych
					break;			
				case 'f':
					PORTD &= ~((1<<KEY1)|(1<<KEY2)); //wlaczenie kluczy tranzystorowych
					break;		
			}
		}
		if(UART0_data_in_rx_buffer())
		{
			switch(rcv = UART0_receive_byte())
			{
				case 'v':
					UART0_print("Podaj nowa predkosc (-255 - 255): ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
					MOTOR_drive(itmp,itmp);
					break;		
				case 'b':
					MOTOR_break();						
					break;
				case 'l':
					MOTOR_sleep();
					break;
				case 'e':
					STEP_enable();
					break;
				case 'd':
					STEP_disable();
					break;														
				case 'w':
					STEP_start();
					break;
				case 's':
					STEP_stop();
					break;			
				case 'p':
					UART0_print("Podaj nowa pozycje: ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
					STEP_go_to_pos(itmp);
					break;				
				case 'j':
					UART0_print("Podaj nowa predkosc (-255 - 255): ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
					MOTOR3_set_speed(itmp);	
					MOTOR4_set_speed(itmp);					
					break;
				case 'k':
					MOTOR3_break();
					MOTOR4_break();
					break;																														
				case 'g':
					MOTOR5_left();
					MOTOR6_left();
					break;
				case 'h':
					MOTOR5_right();
					MOTOR6_right();
					break;	
				case 't':
					MOTOR5_stop();
					MOTOR6_stop();				
					break;	
				case 'y':
					MOTOR5_break();
					MOTOR6_break();					
					break;
				case 'o':
					KEY_PORT |= (1<<KEY1)|(1<<KEY2); //wlaczenie kluczy tranzystorowych
					break;			
				case 'f':
					KEY_PORT &= ~((1<<KEY1)|(1<<KEY2)); //wlaczenie kluczy tranzystorowych
					break;												
			}
		}		
	}
    
return 0;
}
