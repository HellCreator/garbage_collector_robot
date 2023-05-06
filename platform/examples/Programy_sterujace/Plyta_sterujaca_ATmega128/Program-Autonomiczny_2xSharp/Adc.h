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



#define U_ref 5.0  
#define conv_mul ( U_ref / 1024.0 )
#define conv_mul_mv U_ref 

#define ADC_GAIN 10
#define MC33887_I_COEF  0.00266
// dla pomiaru roznicowego ze wzmocnieniem  10
//#define MOTOR_I_SENS_COEF 5000.0*MC33887_I_COEF*1000/(ADC_GAIN*512.0)  // bo w mV
// dla pomiaru zwyklego
#define MOTOR_I_SENS_MUL 5000.0*MC33887_I_COEF*1000/1024.0  // bo w mV


//definicje dla pomiaru 
#define POMIAR_M1       1
#define POMIAR_M2       2
#define POMIAR_M3		3
#define POMIAR_M4		4

#define POMIAR_ADC6		5
#define POMIAR_ADC7		6

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

volatile unsigned int uint_global_adc6;
volatile unsigned int uint_global_adc7;

#endif //__adc_h
