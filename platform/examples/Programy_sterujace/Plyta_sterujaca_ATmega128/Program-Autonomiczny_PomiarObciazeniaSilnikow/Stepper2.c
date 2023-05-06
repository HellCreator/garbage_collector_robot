////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "Stepper2.h"
#include "Uart.h"
#include "Sharp.h"
#include "Adc.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#define SLOPE_SIZE 30  

void STEP_init(void)
{
	DDRG|=(1<<4)|(1<<3)|(1<<2);				//pin /SLEEP oraz DIR
	DDRB|=(1<<7);							//pin CLK
	PORTG|=(1<<3);							//wylaczenie SLEEP
	STEPPER_EN_LO;							//wlaczenie ENABLE
	
	TCCR2|=(1<<CS22)|(1<<CS20)|(1<<WGM21);
	TIMSK|=(1<<OCIE2);


	//OCR2=16;
	//OCR2=16;
	
	DDRE&=~(1<<PE7);   // wejscie przerwania INT7 
	PORTE|=(1<<PE7);							//pullup
	
    EICRB|=(1<<ISC71);						//przerwanie na opadajacym zboczu INT0
	EIMSK|=(1<<INT7);

	global_home_flag=0;
	
	MIN_STEP_PERIOD=40;
}

inline void STEP_enable(void)
{
	STEP_ENABLE_PORT &= ~(1<<STEP_ENABLE);
}

inline void STEP_disable(void)
{
	STEP_ENABLE_PORT |= (1<<STEP_ENABLE);
}


inline void stepper_step(unsigned char dir)
{
	if(dir) 
	{
		STEPPER_DIR_R;
		global_stepper_pos--;
	}
	else
	{
		STEPPER_DIR_L;
		global_stepper_pos++;
	}
	STEPPER_CLK_HI;
	_delay_us(1);
	STEPPER_CLK_LO;

}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void stepper_home(void)
{
	MIN_STEP_PERIOD = 100;
	global_stepper_dest=100;
	
    //while(!global_home_flag);
    while(global_stepper_pos!=100)
    {
        if(global_home_flag)
        break;  
    }
    
    if(!global_home_flag)
    {
       global_stepper_dest = -250;
       while(!global_home_flag);
    }

	global_home_flag=0;
	MIN_STEP_PERIOD = 40;
	OCR2=MIN_STEP_PERIOD;
	//global_stepper_pos = 100;
	global_stepper_dest = 200;
}

SIGNAL(SIG_OUTPUT_COMPARE2)
{
	static unsigned char buf[16];
	
	if(global_stepper_pos<global_stepper_dest)
	{
		if(global_stepper_pos>(global_stepper_dest-SLOPE_SIZE))
		{
			OCR2++;
		}
		else
		{
			if(OCR2>MIN_STEP_PERIOD) OCR2--;
		}
		stepper_step(RIGHT);
	}
	
	if(global_stepper_pos>global_stepper_dest)
	{
		if(global_stepper_pos<(global_stepper_dest+SLOPE_SIZE))
		{
			OCR2++;
		}
		else
		{
			if(OCR2>MIN_STEP_PERIOD) OCR2--;
		}
		stepper_step(LEFT);
		
	}
	if(global_stepper_pos==global_stepper_dest)
	{
		OCR2=MIN_STEP_PERIOD+SLOPE_SIZE;
	}

    if((global_stepper_pos>=0) && (global_stepper_pos<=200))
    sharp_scan_tbl[global_stepper_pos] = (unsigned char)sharp_scan(uint_global_adc6);
	
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
SIGNAL(SIG_INTERRUPT7)
{
	global_home_flag=1;
	if((global_stepper_pos<80) || (global_stepper_pos>120))
	global_stepper_pos=100;
}
