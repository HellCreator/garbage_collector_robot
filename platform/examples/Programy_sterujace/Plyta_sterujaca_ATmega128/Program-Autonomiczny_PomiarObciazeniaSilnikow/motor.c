/*******************************************************************************
						Sterowanie silnikami DC
********************************************************************************
ver. 1.0
Pawel Piatek
p.piatek@wobit.com.pl
WObit 2008

Opis:
	Pliik zawiera funkcje do sterowania driverami silnikow DC na p³ycie 
MOBOT-EXP MB oraz dodatkowymi driverami na plycie rozszerzen MOBOT-EXP MCB
*******************************************************************************/
#include "main.h"

/*******************************************************************************
Funkcja:
	void MOTOR_init(void)
Argumenty: 
	- brak
Opis:
	Inicjalizacja timera 3 do generowania sygnalu PWM (Pulse Width Modulation)
o rozdzielczosci 8 bitow i czestotliwosci ok 4kHz
*******************************************************************************/
void MOTOR_init(void)
{
  	TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<COM3A0)|(1<<COM3B0)|(1<<WGM30);   
    TCCR3B |= (1<<CS31); //8bit Phase Correct PWM inverting mode dla silników kó³
						//preskaler przez 8 co da czêstotliwoœæ ok 4kHz

  	PG3_SLEEP_PORT &= ~(1<<PG3_SLEEP); //uspienie silnikow	
}

/*******************************************************************************
Funkcja:
	void MOTOR_drive(signed int left_speed,signed int right_speed)
Argumenty: 
	- signed int left_speed - wspolczynnik wypelnienia dla silnika od lewego 
		kola, mozna zadawac watosci z pzedzialu -255 - 255, kierunek obrotu 
		zalezy od znaku 
	- signed int right_speed - wspolczynnik wypelnienia dla silnika od prawego 
		kola, mozna zadawac watosci z pzedzialu -255 - 255, kierunek obrotu 
		zalezy od znaku 
Opis:
	Funkcja wyprowadza drivery ze stanu uspienia jezeli przynajniej jedna 
predkosc zadana jest rozna od zera (nalezy pamietac, ze sygnal PG3_SLEEP jest 
wspolny z driverem silnika krokowego na plytce MOBOT-EXP MCB). Funkcja zmienia 
kierunek obrotu silnika (kierunki lewo i prawo sa umowne) w zaleznosci od znaku
wartosci zadanej oraz ogranicza wartosc zadana tak aby nie nastepowalo 
przepelnienie.
*******************************************************************************/
void MOTOR_drive(signed int left_speed,signed int right_speed)
{
	if((left_speed != 0) || (right_speed != 0))
		PG3_SLEEP_PORT |= (1<<PG3_SLEEP); //wybudzenie driverow DC ze stanu uspienia

	if(left_speed > 0) //obot w prawo
	{
		M2_PORT |= (1<<M2_IN2);
		M2_PORT &= ~(1<<M2_IN1);
	}
	else if(left_speed < 0) //obrot w lewo
	{
		M2_PORT &= ~(1<<M2_IN2);
		M2_PORT |= (1<<M2_IN1);
	}
	
	if(right_speed > 0) //obrot w prawo
	{
		M1_PORT |= (1<<M1_IN2);
		M1_PORT &= ~(1<<M1_IN1);
	}
	else if(right_speed<0) //obrot w lewo
	{
		M1_PORT &= ~(1<<M1_IN2);
		M1_PORT |= (1<<M1_IN1);
	}

    if(abs(right_speed) >= PWM_MAX) //ograniczenie wartosci maksymalnej
		OCR3B = PWM_MAX;
	else
		OCR3B = (unsigned char)(abs(right_speed));
	             
	  
    if(abs(left_speed) >= PWM_MAX) //ograniczenie wartosci maksymalnej
		OCR3A = PWM_MAX;
	else
		OCR3A = (unsigned char)(abs(left_speed));  
	
}
/*******************************************************************************
Funkcja:
	void MOTOR_break(void)
Argumenty: 
	- brak
Opis:
	Realizuje hamowanie przez zwarcie obu wyprowadzen silnikow do masy. Przydatna
da awaryjnego zatrzymania robota.
*******************************************************************************/
void MOTOR_break(void)
{
     MOTOR_drive(255,255);
     M1_PORT |=  (1<<M1_IN1);                     
     M1_PORT |=  (1<<M1_IN2);                     
     M2_PORT |=  (1<<M2_IN1);                     
     M2_PORT |=  (1<<M2_IN2);   
}
/*******************************************************************************
Funkcja:
	void MOTOR_sleep(void)
Argumenty: 
	- brak
Opis:
	Funkcja wprowadza drivery silnikow od kol napedowych oraz driver silnika 
krokowego w stan uspienia (wspolne wyprowadzenie PG3_SLEEP), obnizajac pobor 
pradu
*******************************************************************************/
void MOTOR_sleep(void)
{
	PG3_SLEEP_PORT &= ~(1<<PG3_SLEEP); //przejscie driverow DC w stan uspienia	
}
/*******************************************************************************
Funkcja:
	void MOTOR34_init(void)
Argumenty: 
	- brak
Opis:
	Inicjalizacja timera 1 do generowania sygnalu PWM (Pulse Width Modulation)
o rozdzielczosci 8 bitow i czestotliwosci ok 4kHz dla driverow silnikow DC 
znajdujacych sie na plycie rozszerzen MOBOT-EXP MCB
*******************************************************************************/
void MOTOR34_init(void)
{
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM30);
    TCCR1B = (1<<CS11); //8bit Phase Correct PWM, 
						//preskaler przez 8 co da czêstotliwoœæ ok 4kHz
}
/*******************************************************************************
Funkcja:
	void MOTOR3_set_speed(signed int speed)
Argumenty: 
	- signed int speed - wspolczynnik wypelnienia dla silnika mozna zadawac 
		watosci z pzedzialu -255 - 255, kierunek obrotu zalezy od znaku 
Opis:
	Funkcja zmienia kierunek obrotu silnika (kierunki lewo i prawo sa umowne) w 
zaleznosci od znaku wartosci zadanej oraz ogranicza wartosc zadana tak aby nie 
nastepowalo przepelnienie.
*******************************************************************************/
void MOTOR3_set_speed(signed int speed)
{

	if(speed > 0) //obrot w prawo
	{
		M3_PORT |= (1<<M3_RIN);
		M3_PORT &= ~(1<<M3_FIN);
	}
	else if(speed < 0) //obrot w lewo
	{
		M3_PORT &= ~(1<<M3_RIN);
		M3_PORT |= (1<<M3_FIN);
	}

    if(abs(speed)>=PWM34_MAX) //ograniczenie wartosci maksymalnej
		OCR1A = PWM34_MAX;
	else
		OCR1A = (unsigned char)(abs(speed));
}
/*******************************************************************************
Funkcja:
	void MOTOR3_break(void)
Argumenty: 
	- brak
Opis:
	Realizuje hamowanie przez zwarcie obu wyprowadzen silnika do masy. Przydatna
da awaryjnego zatrzymania.
*******************************************************************************/
void MOTOR3_break(void)
{
	M3_PORT |= ((1<<M3_RIN)|(1<<M3_FIN));
	OCR1A = PWM34_MAX;
}
/*******************************************************************************
Funkcja:
	void MOTOR4_set_speed(signed int speed)
Argumenty: 
	- signed int speed - wspolczynnik wypelnienia dla silnika mozna zadawac 
		watosci z pzedzialu -255 - 255, kierunek obrotu zalezy od znaku 
Opis:
	Funkcja zmienia kierunek obrotu silnika (kierunki lewo i prawo sa umowne) w 
zaleznosci od znaku wartosci zadanej oraz ogranicza wartosc zadana tak aby nie 
nastepowalo przepelnienie.
*******************************************************************************/
void MOTOR4_set_speed(signed int speed)
{

	if(speed > 0) //obrot w prawo
	{
		M4_PORT |= (1<<M4_RIN);
		M4_PORT &= ~(1<<M4_FIN);
	}
	else if(speed < 0) //obrot w lewo
	{
		M4_PORT &= ~(1<<M4_RIN);
		M4_PORT |= (1<<M4_FIN);
	}

    if(abs(speed)>=PWM34_MAX) //ograniczenie wartosci maksymalnej
		OCR1B = PWM34_MAX;
	else
		OCR1B = (unsigned char)(abs(speed));           
}
/*******************************************************************************
Funkcja:
	void MOTOR4_break(void)
Argumenty: 
	- brak
Opis:
	Realizuje hamowanie przez zwarcie obu wyprowadzen silnika do masy. Przydatna
da awaryjnego zatrzymania.
*******************************************************************************/
void MOTOR4_break(void)
{
	M4_PORT |= ((1<<M4_RIN)|(1<<M4_FIN));
	OCR1B = PWM34_MAX;
}
/*******************************************************************************
Funkcja:
	void MOTOR56_init(void)
Argumenty: 
	- brak
Opis:
	Inicjalizacja wyjscia OC3C do generowania sygnalu PWM (Pulse Width Modulation)
dla driverow silnikow DC znajdujacych sie na plycie rozszerzen MOBOT-EXP MCB. 
Sygnal PWM jest wspolny dla obu silnikow (5 i 6), mozna go wykorzystac do 
ograniczenia napiecia dla silnikow o napieciu znamionowym mniejszym od napiecia 
zasilania. Funkcja nie inicjalizuje timera, dlatego do generowania przebiegu 
nalezy wywolac dodatkowo funkcje MOTOR_init();
*******************************************************************************/
void MOTOR56_init()
{
	TCCR3A |= (1<<COM3C1); //reszta jest juz zainicjowana w funkcji MOTOR_init
	
	OCR3C = PWM56;
}
/*******************************************************************************
Funkcja:
	void MOTOR5_right(void)
Argumenty: 
	- brak
Opis:
	Ustawia kierunek obrotu silnika 5 w prawo (kierunek umowny)
*******************************************************************************/
void MOTOR5_right(void)
{
	M5_PORT |= (1<<M5_RIN);
	M5_PORT &= ~(1<<M5_FIN);
}
/*******************************************************************************
Funkcja:
	void MOTOR5_left(void)
Argumenty: 
	- brak
Opis:
	Ustawia kierunek obrotu silnika 5 w lewo (kierunek umowny)
*******************************************************************************/
void MOTOR5_left(void)
{
	M5_PORT &= ~(1<<M5_RIN);
	M5_PORT |= (1<<M5_FIN);
}
/*******************************************************************************
Funkcja:
	void MOTOR5_stop(void)
Argumenty: 
	- brak
Opis:
	Powoduje zatrzymanie silnika 5 prezez odlaczenie zasilania
*******************************************************************************/
void MOTOR5_stop(void)
{
	M5_PORT &= ~((1<<M5_RIN)|(1<<M5_FIN));
}
/*******************************************************************************
Funkcja:
	void MOTOR5_stop(void)
Argumenty: 
	- brak
Opis:
	Realizuje hamowanie silnika 5 prezez zwarcie obu wyprowadzen do masy
*******************************************************************************/
void MOTOR5_break(void)
{
	M5_PORT |= ((1<<M5_RIN)|(1<<M5_FIN));
}
/*******************************************************************************
Funkcja:
	void MOTOR6_right(void)
Argumenty: 
	- brak
Opis:
	Ustawia kierunek obrotu silnika 6 w prawo (kierunek umowny)
*******************************************************************************/
void MOTOR6_right(void)
{
	M6_PORT |= (1<<M6_RIN);
	M6_PORT &= ~(1<<M6_FIN);
}
/*******************************************************************************
Funkcja:
	void MOTOR6_left(void)
Argumenty: 
	- brak
Opis:
	Ustawia kierunek obrotu silnika 6 w lewo (kierunek umowny)
*******************************************************************************/
void MOTOR6_left(void)
{
	M6_PORT &= ~(1<<M6_RIN);
	M6_PORT |= (1<<M6_FIN);
}
/*******************************************************************************
Funkcja:
	void MOTOR6_stop(void)
Argumenty: 
	- brak
Opis:
	Powoduje zatrzymanie silnika 6 prezez odlaczenie zasilania
*******************************************************************************/
void MOTOR6_stop(void)
{
	M6_PORT &= ~((1<<M6_RIN)|(1<<M6_FIN));
}
/*******************************************************************************
Funkcja:
	void MOTOR6_stop(void)
Argumenty: 
	- brak
Opis:
	Realizuje hamowanie silnika 6 prezez zwarcie obu wyprowadzen do masy
*******************************************************************************/
void MOTOR6_break(void)
{
	M6_PORT |= ((1<<M6_RIN)|(1<<M6_FIN));
}
