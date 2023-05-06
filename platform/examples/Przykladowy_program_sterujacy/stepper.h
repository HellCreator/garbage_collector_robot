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
inline void STEP_enable(void);
inline void STEP_disable(void);
inline void STEP_set_dir_l(void);
inline void STEP_set_dir_r(void);
inline void STEP_start(void);
inline void STEP_stop(void);
void STEP_go_to_pos(int pos);
inline void STEP_sleep(void);//wspolne z driverami od silnikow DC
inline void STEP_wake_up(void);//wspolne z driverami od silnikow DC
inline int STEP_get_pos(void);

#endif //__STEPPER_H
