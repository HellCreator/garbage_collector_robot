/*******************************************************************************
						Sterowanie silnikiem krokowym
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Plik zawiera funkcje do sterowania silnikiem krokowym przy uzyciu timera 2.
Pozwalaja one na zadawanie pozycji w (mikro)krokach, faktczny kat obrotu walu 
silnika zalezy od podzialu krokow ustawionego zworkami na plytce sterownika
MOBOT-EXP MCB 
*******************************************************************************/
#include "Main.h"

volatile int global_step_pos = 0;
volatile int global_step_target_pos = 0;


ISR(TIMER2_COMP_vect) //obsluga przerwania od Compare Match
{
	static unsigned char count = 0;
	
	if(count++ & 0x01) 	//zamiast sprawdzania czy jest rowne 1 i przypisywania 0 
						//jest inkrementacja i bitowe AND, konieczne bo przerwanie 
						//jest generowane przy zboczu narastajacym i opadajacym
						//na pinie procesora OC2
	{
		if(STEP_DIR_PORT & (1<<STEP_DIR)) //jezeli kreci sie w prawo
			global_step_pos++;//uaktualnienie pozycji silnika
		else	//jezeli kreci sie w lewo
			global_step_pos--;//uaktualnienie pozycji silnika

		if(global_step_target_pos == global_step_pos) //jesli dojedzie na zadana pozycje
			STEP_stop();//zatrzymanie silnika
	}
}
/*******************************************************************************
Funkcja:
	void STEP_init(void)
Argumenty: 
	- brak
Opis:
	Inicjalizacja timera 2 do generowania przebiegu PWM o stalym wypelnieniu 50% 
i czestotliwosci zalezmej od rejestru OCR2
*******************************************************************************/
void STEP_init(void)
{
	OCR2 = 194; //40Hz, zgodnie ze wzorem f_OC2=f_clk/(2*N(1+OCR2)) gdzie N jest 
				//preskalerem - patrz dokumentacja procesora
	TCCR2 |= (1<<WGM21)|(1<<COM20);//tryb CTC , zmiana stanu OC2 na przeciwny 
									//przy compare match, preskaler 1024
	
	TIMSK |= (1<<OCIE2); //przerwanie od Compare Match
	
	STEP_disable();
}
/*******************************************************************************
Funkcja:
	inline void STEP_enable(void)
Argumenty: 
	- brak
Opis:
	Powoduje wlaczenie tranzystorow w sterowniku silnika krokowego, przez co 
przez silnik zaczyna plynac prad i pojawia sie moment trzymajacy
*******************************************************************************/
inline void STEP_enable(void)
{
	STEP_ENABLE_PORT &= ~(1<<STEP_ENABLE);
}
/*******************************************************************************
Funkcja:
	inline void STEP_enable(void)
Argumenty: 
	- brak
Opis:
	Powoduje wylaczenie tranzystorow w sterowniku silnika krokowego, przez co 
przez silnik przestaje plynac prad i zanika sie moment trzymajacy
*******************************************************************************/
inline void STEP_disable(void)
{
	STEP_ENABLE_PORT |= (1<<STEP_ENABLE);
}
/*******************************************************************************
Funkcja:
	iinline void STEP_set_dir_l(void)
Argumenty: 
	- brak
Opis:
	Przelacza kierunek obrotow silnika krokowego w lewo (kierunek umowny)
*******************************************************************************/
inline void STEP_set_dir_l(void)
{
	STEP_DIR_PORT &= ~(1<<STEP_DIR);
}
/*******************************************************************************
Funkcja:
	inline void STEP_set_dir_r(void)
Argumenty: 
	- brak
Opis:
	Przelacza kierunek obrotow silnika krokowego w prawo (kierunek umowny)
*******************************************************************************/
inline void STEP_set_dir_r(void)
{
	STEP_DIR_PORT |= (1<<STEP_DIR);
}
/*******************************************************************************
Funkcja:
	inline void STEP_start(void)
Argumenty: 
	- brak
Opis:
	Wlacza timer generujacy przebieg prostokatny dla sterownika silnika
krokowego, dzieki czemu silnik zaczyna sie obracac
*******************************************************************************/
inline void STEP_start(void)
{
	TCCR2 |= (1<<WGM21)|(1<<COM20)|(1<<CS22)|(1<<CS20);
}
/*******************************************************************************
Funkcja:
	inline void STEP_stop(void)
Argumenty: 
	- brak
Opis:
	Wylacza timer generujacy przebieg prostokatny dla sterownika silnika
krokowego, dzieki czemu silnik przestaje sie obracac.
*******************************************************************************/
inline void STEP_stop(void)
{
	TCCR2 &= ~((1<<CS22)|(1<<CS20));
}
/*******************************************************************************
Funkcja:
	void STEP_go_to_pos(int pos)
Argumenty: 
	- int pos - pozycja na ktora ma dojechac silnik krokowy wyrazona w 
		(mikro)krokach, faktyczny kat orotu zalezy od podzialu krokow sterownika 
		ustawionego zworkami na plytce
Opis:
	Funkcja wybudza drivery ze stanu uspienia (rowniez te od silnikow DC -
- wspolny sygnal PG3_SLEEP). Funkcja ustala odpowiedni kierunek obrotow w 
zaleznosci od aktualnej i zadanej pozycji, po czym wlacza prad w silniku i timer
do generowania impulsow.
*******************************************************************************/
void STEP_go_to_pos(int pos)
{
	STEP_wake_up(); 	//wybudzenie driverq ze stanu uspienia 
										//(wspolne z silnikami DC)
	global_step_target_pos = pos;
	if(global_step_pos == pos) // jesli pozycja zadana jest taka jak aktualna
	{
		STEP_stop();
	}
	if(global_step_pos > pos) // jesli pozycja zadana jest na lewo od aktualnej 
	{
		STEP_set_dir_l();
		STEP_enable();
		STEP_start();
	}
	if(global_step_pos < pos) // jesli pozycja zadana jest na prawo od aktualnej 
	{
		STEP_set_dir_r();
		STEP_enable();
		STEP_start();
	}	
}
/*******************************************************************************
Funkcja:
	void STEP_sleep(void)
Argumenty: 
	- brak
Opis:
	Funkcja wprowadza drivery silnikow od kol napedowych oraz driver silnika 
krokowego w stan uspienia (wspolne wyprowadzenie PG3_SLEEP), obnizajac pobor 
pradu
*******************************************************************************/
inline void STEP_sleep(void)
{
	PG3_SLEEP_PORT &= ~(1<<PG3_SLEEP); 
}
/*******************************************************************************
Funkcja:
	void STEP_wake_up(void)
Argumenty: 
	- brak
Opis:
	Funkcja wyprowadza drivery silnikow od kol napedowych oraz driver silnika 
krokowego ze stanu uspienia (wspolne wyprowadzenie PG3_SLEEP).
*******************************************************************************/
inline void STEP_wake_up(void)
{
	PG3_SLEEP_PORT |= (1<<PG3_SLEEP); 	
}
/*******************************************************************************
Funkcja:
	inline int STEP_get_pos(void)
Argumenty: 
	- brak
Opis:
	Funkcja zwraca aktualne polozenie walu silnika w (mikro)krokach, faktyczny 
kat orotu zalezy od podzialu krokow sterownika ustawionego zworkami na plytce
*******************************************************************************/
inline int STEP_get_pos(void)
{
	return global_step_pos;
}
