/*******************************************************************************
							Pliki naglowkowe
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Plik zawiera polecenie dolaczenia wszystkich uzywanych plikow naglowkowych
oraz definicje mart sluzacych do zapalania, gaszenia oraz przelaczania diod LED
*******************************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_


#include "io_cfg.h"
#include "Adc.h"
#include "motor.h"
#include "timer.h"
#include "Uart.h"
#include "Stepper2.h"


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// klucze

#define KEY1_ON   KEY_PORT|=(1<<KEY1)
#define KEY1_OFF  KEY_PORT&=~(1<<KEY1)

#define KEY2_ON   KEY_PORT|=(1<<KEY2)
#define KEY2_OFF  KEY_PORT&=~(1<<KEY2)



// diody
#define LED1_OFF         LED1_PORT|=(1<<LED1)
#define LED1_ON          LED1_PORT&=~(1<<LED1)
#define LED1_TOGGLE      LED1_PORT^=(1<<LED1)

#define LED2_OFF         LED2_PORT|=(1<<LED2)
#define LED2_ON          LED2_PORT&=~(1<<LED2)
#define LED2_TOGGLE      LED2_PORT^=(1<<LED2)

#define LED4_ON         LED4_PORT|=(1<<LED4)
#define LED4_OFF          LED4_PORT&=~(1<<LED4)
#define LED4_TOGGLE      LED4_PORT^=(1<<LED4)

#define LED5_ON         LED5_PORT|=(1<<LED5)
#define LED5_OFF          LED5_PORT&=~(1<<LED5)
#define LED5_TOGGLE      LED5_PORT^=(1<<LED5)


//================ PARAMETRY  JAZDY AUTONOMICZNEJ ROBOTA =======================

#define DRIVE_DELAY_100MS 100
#define DRIVE_DELAY_200MS 200
#define DRIVE_DELAY_500MS 500
#define DRIVE_DELAY_1S    1000
#define DRIVE_DELAY_2S    2000

#define GLOBAL_MAX_SPEED 255
#define GLOBAL_MID_SPEED 170
#define GLOBAL_MID2_SPEED 200
#define GLOBAL_MIN_SPEED 130


#define PROX_DIST      40   // zblizene 50cm
#define PROX_MIN_DIST  20


#define MOTOR_STOP_CURRENT 800UL 

#define GP2D12_70CM   500
#define GP2D12_50CM   700
#define GP2D12_30CM   950
#define GP2D12_20CM   1400
#define GP2D12_15CM   1700
#define GP2D12_10CM   2500


//==============================================================================





volatile unsigned int drive_timer;
volatile int global_left_speed;
volatile int global_right_speed;


////////////////////////////////////////////////////////////////////////////////////////////////
#endif//_MAIN_H_
