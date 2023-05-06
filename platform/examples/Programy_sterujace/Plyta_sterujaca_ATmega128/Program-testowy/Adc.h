/*******************************************************************************
						Obsluga przetwornika ADC
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Pliik zawiera definicje symboli oraz deklaracje funkcji do obslugi 
przetwornika ADC.
*******************************************************************************/
#ifndef __adc_h
#define __adc_h


#define ADC_ILOSC_POMIAROW 16

//definicje dla pomiaru 
#define POMIAR_M1       1
#define POMIAR_M2       2
#define POMIAR_M3		3
#define POMIAR_M4		4

//deklaracje funkcji
void ADC_init(void);
inline void ADC_start_conversion(void);
inline void ADC_turn_off(void);
inline void ADC_turn_on(void);


//zmienne globalne
volatile unsigned int uint_global_prad_M1;
volatile unsigned int uint_global_prad_M2;
volatile unsigned int uint_global_prad_M3;
volatile unsigned int uint_global_prad_M4;


#endif //__adc_h
