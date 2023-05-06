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
w glownej petli oczekuje na odbior polecen przez UART odpowiednio na nie reagujac
*******************************************************************************/
#include "Main.h"
#include "Sharp.h"

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
	LED4_DDR |= (1<<LED4)|(1<<LED5); //jako wyjscia


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
    unsigned char I_mes_timer = 0; 
    unsigned char buf[32];
    
    unsigned char index;
	unsigned char min=255;
	unsigned char i;
	
	unsigned int sharp_lewy;
	unsigned int sharp_prawy;
    signed int sharp_roznica;
	unsigned char pos_cnt=0;
	
    ioinit();	
	UART0_init();					  
    //UART1_init();
    TIMER_init();
    MOTOR_init();
    //MOTOR34_init();
    //MOTOR56_init();
    ADC_init();
    
    ADC_start_conversion();
	_delay_ms(20);
	
    //STEP_init();
    
    sei();	//globalne wlaczenie przerwan

	UART0_print("UART0 test\r\n");
	//UART1_print("UART1 test\r\n");
	
	LED2_OFF;
	LED4_OFF;
	
	
	//global_stepper_dest=0;
    // _delay_ms(20);
	//stepper_home();

    //DDRE|=(1<<PE2)|(1<<PE6);
    
    //PORTE|=(1<<PE2);
    //PORTE&=~(1<<PE4);
    
    // LEWY SILNIK - M2
    // PRAWY SILNIK - M1
    // SHARP LEWY  - ADC7 - JP5 
    // SHARP PRAWY - ADC6 - JP4 
    
	for(;;) //glowna petla programu
	{
        
		// RADIO
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
				//	STEP_start();
					break;
				case 's':
                        if(global_stepper_pos==0)		global_stepper_dest=200;
     	                if(global_stepper_pos==200)		global_stepper_dest=0;	
				//	STEP_stop();
					break;			
				case 'p':
					UART0_print("Podaj nowa pozycje: ");
					UART0_gets(tmp,8);
					itmp = atoi(tmp);
					UART0_print("\r\n");
				//	STEP_go_to_pos(itmp);
				    global_stepper_dest = itmp;
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
    


 	if(drive_timer==0)
 	{     

       	//UART0_print("I_M1: ");
        //itoa(uint_global_prad_M1,buf,10);
        //UART0_print(buf);
    	//UART0_print("\n\r");
        //UART0_print("I_M2: ");
        //itoa(uint_global_prad_M2,buf,10);
    	//UART0_print(buf);
    	//UART0_print("\n\r");

	
        sharp_lewy = sharpGP2D12(conv_mul_mv*uint_global_adc7);
       	//UART0_print("ADC6: ");
        //itoa(sharp_lewy,buf,10);
    	//UART0_print(buf);
    	//UART0_print("\n\r");
    	
        sharp_prawy = sharpGP2D12(conv_mul_mv*uint_global_adc6);
       	//UART0_print("ADC7: ");
        //itoa(sharp_prawy,buf,10);
    	//UART0_print(buf);
    	//UART0_print("\n\r");

      
             		
 		if((uint_global_prad_M1>MOTOR_STOP_CURRENT)||(uint_global_prad_M2>MOTOR_STOP_CURRENT))    
 		{
            I_mes_timer++;
            if(I_mes_timer>3)
            {     
                LED5_ON;  
                I_mes_timer = 0;
                
                if((uint_global_prad_M1>MOTOR_STOP_CURRENT)||(uint_global_prad_M2>MOTOR_STOP_CURRENT))
                {
                    if(global_right_speed>0)     
                    {                             
                        MOTOR_drive(-GLOBAL_MID_SPEED,-GLOBAL_MID_SPEED); 
                        TIMER_wait_ms(800);  
                        MOTOR_drive(GLOBAL_MAX_SPEED,-GLOBAL_MAX_SPEED); 
                        TIMER_wait_ms(1100);                     
                    }
                    else
                    {
                        MOTOR_drive(GLOBAL_MID_SPEED,GLOBAL_MID_SPEED); 
                        TIMER_wait_ms(800);  
                        MOTOR_drive(GLOBAL_MAX_SPEED,-GLOBAL_MAX_SPEED); 
                        TIMER_wait_ms(1100);   
                    }
                }
            }
        } 
        else
        {
            LED5_OFF;
            I_mes_timer = 0;
            
            
            //sharp_roznica = sharp_lewy - sharp_prawy;
            // UWAGA - znaki porownna przeciwne - gdyz sharp GP2D2 ma funkcje napiecia odwrotna do odleglosci
            /*
            if(sharp_prawy<50 && sharp_lewy<30)
            {
                global_right_speed = -255;
                global_left_speed  = -255; 
            }
            else
            {
                global_right_speed = sharp_lewy * 3;
                global_left_speed  = sharp_prawy * 3;
            }
            */
            
            

            if((sharp_lewy>30) && (sharp_prawy>30))
            {
                pos_cnt = 0;
                if(((signed char)sharp_lewy - (signed char)sharp_prawy)<=-10)
                {
                   global_left_speed  = 200;
                   global_right_speed = 70;
                }
                else if ((signed char)(sharp_lewy - (signed char)sharp_prawy)>=10)
                {
                   global_left_speed  = 70;
                   global_right_speed = 200;
                }
                else
                {
                    global_left_speed=GLOBAL_MAX_SPEED;
                    global_right_speed=GLOBAL_MAX_SPEED; 
                }
                drive_timer = DRIVE_DELAY_500MS;
            }
            /*
            else  
            {
                pos_cnt++;
                if(sharp_lewy<=30)
                {
                   //global_left_speed  = 200;
                   //global_right_speed = -200;
                    MOTOR_drive(200,-200); 
                    TIMER_wait_ms(1000); 
                }
                 if(sharp_prawy<=30)
                {
                   MOTOR_drive(-200,200);
                   TIMER_wait_ms(1000); 
                   
                }   
                if(pos_cnt>5)
                {
                   MOTOR_drive(-200,200);
                   TIMER_wait_ms(3000);  
                   pos_cnt = 0;    
                }
                drive_timer = DRIVE_DELAY_500MS;
            }
            */
            else if (((sharp_lewy<=30)&&(sharp_lewy>10))  || ((sharp_prawy<=30)&&(sharp_prawy>10)))
            {
                pos_cnt++;
                if(sharp_lewy<=30)
                {
                   //global_left_speed  = 200;
                   //global_right_speed = -200;
                    MOTOR_drive(200,-200); 
                    TIMER_wait_ms(1000); 
                }
                if(sharp_prawy<=30)
                {
                   MOTOR_drive(-200,200);
                   TIMER_wait_ms(1000); 
                }   
                if(pos_cnt>5)
                {
                   MOTOR_drive(-200,200);
                   TIMER_wait_ms(3000);  
                   pos_cnt = 0;    
                }
                drive_timer = DRIVE_DELAY_500MS;
            }
            else
            {
                   MOTOR_drive(-200,-200);
                   TIMER_wait_ms(800); 
                   MOTOR_drive(200,-200);
                   TIMER_wait_ms(1200);   
                  
                   drive_timer = DRIVE_DELAY_100MS;
            }
    
        }  
	}	
     MOTOR_drive(global_left_speed,global_right_speed);  
	}
    
return 0;
}
