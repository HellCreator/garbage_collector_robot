
#include "io_cfg.h"

#ifndef _STEPPER_H_
#define STEPPER_H_

#define STEPPER_DIR_R STEP_DIR_PORT |= (1<<STEP_DIR)
#define STEPPER_DIR_L STEP_DIR_PORT &= ~(1<<STEP_DIR)

#define STEPPER_CLK_HI STEP_CLK_PORT|=(1<<STEP_CLK)
#define STEPPER_CLK_LO STEP_CLK_PORT&=~(1<<STEP_CLK)

#define STEPPER_EN_HI STEP_ENABLE_PORT |= (1<<STEP_ENABLE)
#define STEPPER_EN_LO STEP_ENABLE_PORT &= ~(1<<STEP_ENABLE)
	
#define LEFT 1
#define RIGHT 0


//#define MIN_STEP_PERIOD 220			//jednostka=1/(16M/256)
volatile unsigned char MIN_STEP_PERIOD;			//jednostka=1/(16M/256)

void STEP_init(void);
inline void stepper_step(unsigned char dir);
void stepper_home(void);

inline void STEP_enable(void);
inline void STEP_disable(void);

volatile int global_stepper_dest;
volatile int global_stepper_pos;
volatile unsigned char global_home_flag;



#endif //_STEPPER_H_
