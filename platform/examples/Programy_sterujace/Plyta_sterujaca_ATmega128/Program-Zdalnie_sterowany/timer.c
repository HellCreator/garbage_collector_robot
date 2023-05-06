/*******************************************************************************
							Obsluga timera 0
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Plik zawiera funkcje do obslugi timera0 oraz funkcje obslugi przerwania 
(wywolywana co 1 ms) zmieniajaca stan diody LED1 co 500ms.
*******************************************************************************/
#include "Main.h"

volatile unsigned int TIMER_wait_time_ms = 0;



ISR(TIMER0_COMP_vect)
{
static int i=0;

i++;
if(i>20)
	{
	i = 0;
	PORTE^=(1<<6);
	}

if(timeout)
	{
	timeout--;
	}

if(!timeout)
	{
	MOTOR_drive(0, 0); 
	timeout = 0;
	}
}
/*******************************************************************************
Funkcja:
	void TIMER_init(void)
Argumenty: 
	- brak
Opis:
	Inicjalizacja timera 0 do generowania przerwania co 1ms
*******************************************************************************/
void TIMER_init(void)
	{
	OCR0 = 249;
	TIMSK |= (1<<OCIE0); //wlaczenie przerwania
	TCCR0 = ((1<<WGM01)|(1<<CS02) |(1<<CS01) |(1<<CS00));//tryb pracy  CTC, preskaler 1024, co daje przy OCR0=249 1kHz@16Mhz, czyli co 1ms	
	}
/*******************************************************************************
Funkcja:
	void TIMER_wait_ms(unsigned int time_ms)
Argumenty: 
	- unsigned int time_ms - czas opoznienia w ms , maksymalna wartosc to 65 535
Opis:
	Funkcja oczekuje w petli okreslona ilosc milnisekund.
*******************************************************************************/
void TIMER_wait_ms(unsigned int time_ms)
{
	TIMER_wait_time_ms = time_ms;
	while(TIMER_wait_time_ms) ; //czekanie zadany czas
}