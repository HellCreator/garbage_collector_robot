/*******************************************************************************
						Obsluga przetwornika ADC
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Pliik zawiera funkcje do obslugi przetwornika ADC, oraz funkcje obslugi 
przerwania ktora uaktualnia zmiene globalne uint_global_prad_Mx przechowujace 
wartosci pradow silnikow w mA. Przetwornik pracuje z czestotliwoscia 125kHz przy
kwarcu 16MHz, w trybie ci�g�ej konwersji. Dla kazdego mierzonego kanalu ADC 
liczona jest srednia z 16 probek co daje uaktualnienie wszystkich wartosci pradow 
z czestotliwoscia ok 122Hz (ok. 8ms)
*******************************************************************************/
#include "Main.h"

ISR(ADC_vect) //Przerwanie ADC Conversion Complete
{
	static signed int pomiar=0;
	static unsigned char ADC_channel = POMIAR_M1;
	static unsigned char ile_pomiarow = ADC_ILOSC_POMIAROW;

	switch(ADC_channel)
	{
		case POMIAR_M1:
		{
			pomiar += ADC;
			if(--ile_pomiarow == 0)
			{
				ADC_turn_off(); //ma na celu upewnienie sie ze nastepny wynik konwersji nie bedzie dotyczyl poprzednigo kanalu ADC
				ADC_channel = POMIAR_M2;
				ile_pomiarow = ADC_ILOSC_POMIAROW;
				
                //wejscie roznicowe pomiaru pradu M2 (ADC3-ADC2), wzmocnienie 10
				ADMUX = (1<<REFS0)| 0b01101 ;
				
                uint_global_prad_M1 = 3.68*(pomiar >> 4);//wyliczenie sredniej, przesuniecie w prawo o 4 bity jest rownowazne dzieleniu przez 16
                										//wynik wyskalowany tak zeby byl w mA
				pomiar = 0;
				
				ADC_turn_on();
				ADC_start_conversion();
				
			}
			break;
		}
		case POMIAR_M2:
		{
			pomiar += ADC;
			if(--ile_pomiarow == 0)
			{
				ADC_turn_off(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu ADC
				ADC_channel = POMIAR_M3;
				ile_pomiarow = ADC_ILOSC_POMIAROW;
				
				//wejscie (sdingle ended) pomiaru pradu M3 (ADC4)
                ADMUX = (1<<REFS0) | 0b00100 ;
				
                uint_global_prad_M2 = 3.68*(pomiar >> 4);//wyliczenie sredniej, przesuniecie w prawo o 4 bity jest rownowazne dzieleniu przez 16
                										//wynik wyskalowany tak zeby byl w mA					
				pomiar = 0;

				ADC_turn_on();
				ADC_start_conversion();
			}
			break;
		}
		case POMIAR_M3:
		{
			pomiar += ADC;
			if(--ile_pomiarow == 0)
			{
				ADC_turn_off(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu ADC
				ADC_channel = POMIAR_M4;
				ile_pomiarow = ADC_ILOSC_POMIAROW;
								
                //wejscie (sdingle ended) pomiaru pradu M4 (ADC5)
                ADMUX = (1<<REFS0) | 0b00101 ;
                
				uint_global_prad_M3 = 0.6745*(pomiar >> 4);//wyliczenie sredniej, przesuniecie w prawo o 4 bity jest rownowazne dzieleniu przez 16
                										//wynik wyskalowany tak zeby byl w mA
				pomiar = 0;

				ADC_turn_on();
				ADC_start_conversion();
			}
			break;
		}		
		case POMIAR_M4:
		{
			pomiar += ADC;
			if(--ile_pomiarow == 0)
			{
				ADC_turn_off(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu ADC
				ADC_channel = POMIAR_M1;
				ile_pomiarow = ADC_ILOSC_POMIAROW;
				
				//wejscie roznicowe pomiaru pradu M1 (ADC1-ADC0), wzmocnienie 10
				ADMUX = (1<<REFS0) | 0b01001 ;				
				
				uint_global_prad_M4 = 0.6745*(pomiar >> 4);//wyliczenie sredniej, przesuniecie w prawo o 4 bity jest rownowazne dzieleniu przez 16
                										//wynik wyskalowany tak zeby byl w mA
				pomiar=0;
				
				ADC_turn_on();
				ADC_start_conversion();
			}
			break;
		}
		
	}
}// koniec obslugi przerwania ADC Conversion Complete

/*******************************************************************************
Funkcja:
	void ADC_init(void)
Argumenty: 
	- brak
Opis:
	Inicjalizacja przetwornika ADC, wybor zrodla odniesienia, wlaczenie przerwan, 
autowyzwalanie i konfiguracja zegara ADC oraz w��czenie przetwornika
*******************************************************************************/
void ADC_init(void)
{
	ADMUX|=(1<<REFS0);    //napiecie ref AVCC
	
    ADCSRA|=(1<<ADEN)|(1<<ADFR)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//preskaler 128 - 125kHz@16MHz, wlaczone przerwania, konwersja caigla
}

/*******************************************************************************
Funkcja:
	inline void ADC_start_conversion(void)
Argumenty: 
	- brak
Opis:
	Rozpoczyna pierwsza konwersje, po wlaczeniu przetwornika ADC
*******************************************************************************/
void ADC_start_conversion(void)
{
	ADCSRA|=(1<<ADSC);
}

/*******************************************************************************
Funkcja:
	inline void ADC_turn_off(void)
Argumenty: 
	- brak
Opis:
	Wylaczenie (calkowite) przetwornika ADC
*******************************************************************************/
void ADC_turn_off(void)
{
	ADCSRA &= ~(1<<ADEN);
}

/*******************************************************************************
Funkcja:
	inline void ADC_turn_on(void)
Argumenty: 
	- brak
Opis:
	Funkcja wlacza przetwornik ADC, nie rozpoczynajac konwersji
*******************************************************************************/
void ADC_turn_on(void)
{
	ADCSRA |= (1<<ADEN);
}
