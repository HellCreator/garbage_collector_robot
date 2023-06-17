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
Program mo�na testowa� za pomoc� dowolnego terminala.
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
	char adc_tmp[10];
	int iter;

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
				case 'x':
					UART1_print("echo 2");
					UART1_print("\r\n");
					break;
			}
		}
		if(UART0_data_in_rx_buffer())
		{
			switch(rcv = UART0_receive_byte())
			{
				case 'x':
					// simple echo comand check connection
					UART0_print("echo");
					UART0_print("\r\n");
					break;
				case 'c':
					UART0_print("set new left speed (-255 - 255): ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
					MOTOR_left(itmp);
					break;
				case 'e':
					UART0_print("set new left speed (-255 - 255): ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
					MOTOR_right(itmp);
					break;
				case 'v':
					// set velocity example:
					//v 150
					UART0_print("set new speed (-255 - 255): ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
					MOTOR_drive(itmp,itmp);
					break;
				case 'w':
					// move forward maximum speed
					MOTOR_drive(255, 255);
					UART0_print("Forward");
					UART0_print("\r\n");
					break;
				case 's':
					// move backward maximum speed
					MOTOR_drive(-255, -255);
					UART0_print("Backward");
					UART0_print("\r\n");
					break;
				case 'd':
					// turn right
					MOTOR_drive(200, -150);
					UART0_print("Right");
					UART0_print("\r\n");
					break;
				case 'a':
					// turn left
					MOTOR_drive(-150, 200);
					UART0_print("Left");
					UART0_print("\r\n");
					break;
				case 'b':
					// stop platform
					MOTOR_break();
					UART0_print("MOTOR_break");
					UART0_print("\r\n");						
					break;
				case 'q':
					// reduce power consumption
					MOTOR_sleep();
					UART0_print("MOTOR_sleep");
					UART0_print("\r\n");
					break;
				case 'm':
					UART0_print("voltage /10 V");
					UART0_print("\r\n");
					for (iter = 0; iter < 10; iter++) {adc_tmp[iter] = '\0';};
					itoa(uint_global_prad_M3, adc_tmp, 10);
					adc_tmp[9] = '\0';
					UART0_print("accumulator ");
					UART0_print(adc_tmp);
					for (iter = 0; iter < 10; iter++) {adc_tmp[iter] = '\0';};
					itoa(uint_global_prad_M4, adc_tmp, 10);
					adc_tmp[9] = '\0';
					UART0_print(" Internal reference 1.26V ");
					UART0_print(adc_tmp);
					UART0_print("\r\n");
					// v ref 2.56 V
					// bandcap 1.26 V
					// vin *1024/vref
					//ADC4 3741 ADC5 3886
					break;
				case 'y':
					KEY_PORT |= (1<<KEY1)|(1<<KEY2); //wlaczenie kluczy tranzystorowych
					break;			
				case 'z':
					KEY_PORT &= ~((1<<KEY1)|(1<<KEY2)); //wlaczenie kluczy tranzystorowych
					break;												
			}
		}		
	}
    
return 0;
}
