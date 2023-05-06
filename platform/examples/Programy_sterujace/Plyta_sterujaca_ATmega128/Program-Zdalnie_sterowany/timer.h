/*******************************************************************************
							Obsluga timera 0
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Plik zawiera deklaracje funkcji do obslugi timera0
*******************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

void TIMER_init(void);
void TIMER_wait_ms(unsigned int time_ms);


#endif //__TIMER_H
