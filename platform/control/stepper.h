/*******************************************************************************
						Sterowanie silnikiem krokowym
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Plik zawiera deklaracje funkcji do sterowania silnikiem krokowym.
*******************************************************************************/
#ifndef __STEPPER_H
#define __STEPPER_H

void STEP_init(void);
void STEP_enable(void);
void STEP_disable(void);
void STEP_set_dir_l(void);
void STEP_set_dir_r(void);
void STEP_start(void);
void STEP_stop(void);
void STEP_go_to_pos(int pos);
void STEP_sleep(void);//wspolne z driverami od silnikow DC
void STEP_wake_up(void);//wspolne z driverami od silnikow DC
int STEP_get_pos(void);

#endif //__STEPPER_H
