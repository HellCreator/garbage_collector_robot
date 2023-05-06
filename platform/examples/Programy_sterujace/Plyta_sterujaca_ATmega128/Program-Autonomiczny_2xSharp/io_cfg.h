/*******************************************************************************
			Konfiguracja plytki MOBOT-EXP MB oraz MOBOT-EXP MCB
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008
*******************************************************************************/

// sterowanie silnikami od kol napedowych
#define M1_IN1          PA3
#define M1_IN2          PA4
#define M1_P            PE4
#define M1_P_DDR        DDRE
#define M1_PORT         PORTA
#define M1_DDR          DDRA

#define M2_IN1          PA5
#define M2_IN2          PA6
#define M2_P            PE3
#define M2_P_DDR        DDRE
#define M2_PORT         PORTA
#define M2_DDR          DDRA

#define FS_AB           PC2
#define FS_AB_DDR       DDRC
#define FS_AB_PORT      PORTC
#define FS_AB_PIN       PINC

#define PG3_SLEEP           PG3
#define PG3_SLEEP_PORT      PORTG
#define PG3_SLEEP_DDR       DDRG
//sygnal PG3_SLEEP jest wspolny z driverem silnika krokowego

// sterowanie dodatkowymi silnikami
#define M3_FIN          PC0
#define M3_RIN          PC1
#define M3_P            PB5
#define M3_P_DDR        DDRB
#define M3_PORT         PORTC
#define M3_DDR          DDRC

#define M4_FIN          PC2
#define M4_RIN          PC3
#define M4_P            PB6
#define M4_P_DDR        DDRB
#define M4_PORT         PORTC
#define M4_DDR          DDRC

#define M5_FIN          PC4
#define M5_RIN          PC5
#define M56_P           PE5
#define M56_P_DDR       DDRE
#define M5_PORT         PORTC
#define M5_DDR          DDRC

#define M6_FIN          PC6
#define M6_RIN          PC7
#define M6_PORT         PORTC
#define M6_DDR          DDRC

// diody
#define LED1			PD7
#define LED1_PORT     	PORTD
#define LED1_DDR     	DDRD

#define LED2			PD6
#define LED2_PORT     	PORTD
#define LED2_DDR     	DDRD

//klucze tranzystorowe
#define KEY1			PD5
#define KEY2			PD4
#define KEY_PORT     	PORTD
#define KEY_DDR     	DDRD
//sterowanie driverem od silnika krokowego
#define STEP_CLK		PB7
#define STEP_CLK_PORT	PORTB
#define STEP_CLK_DDR	DDRB

#define STEP_DIR		PG2
#define STEP_DIR_PORT	PORTG
#define STEP_DIR_DDR	DDRG

#define STEP_ENABLE			PG4
#define STEP_ENABLE_PORT	PORTG
#define STEP_ENABLE_DDR		DDRG
//sygnal PG3/SLEEP jest wspolny z driverami silnika DC

// Diody LED zlacza ladowarki
#define LED4			PE6
#define LED4_PORT     	PORTE
#define LED4_DDR     	DDRE

#define LED5			PE2
#define LED5_PORT     	PORTE
#define LED5_DDR     	DDRE

