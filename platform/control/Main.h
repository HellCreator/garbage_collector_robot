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

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "io_cfg.h"
#include "Adc.h"
#include "motor.h"
#include "timer.h"
#include "stepper.h"
#include "Uart.h"


// diody
#define LED1_OFF        LED1_PORT|=(1<<LED1)
#define LED1_ON        LED1_PORT&=~(1<<LED1)
#define LED1_TOGGLE     LED1_PORT^=(1<<LED1)

#define LED2_OFF         LED2_PORT|=(1<<LED2)
#define LED2_ON        LED2_PORT&=~(1<<LED2)
#define LED2_TOGGLE     LED2_PORT^=(1<<LED2)


////////////////////////////////////////////////////////////////////////////////////////////////
#endif//_MAIN_H_
