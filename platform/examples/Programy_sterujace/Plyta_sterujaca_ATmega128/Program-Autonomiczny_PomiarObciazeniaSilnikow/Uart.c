////////////////////////////////////////////////////////////////////////////////
//Copyright (c) 2005
//	Lukasz Matecki
//	lutecki@op.pl
////////////////////////////////////////////////////////////////////////////////
//April 25, 2006 08:57:43 AM 
////////////////////////////////////////////////////////////////////////////////
#include "Main.h"


#define UART0_RX_BUFFER_MASK ( UART0_RX_BUFFER_SIZE - 1 )
#if ( UART0_RX_BUFFER_SIZE & UART0_RX_BUFFER_MASK )
   #error RX buffer size is not a power of 2
#endif

#define UART0_TX_BUFFER_MASK ( UART0_TX_BUFFER_SIZE - 1 )
#if ( UART0_TX_BUFFER_SIZE & UART0_TX_BUFFER_MASK )
   #error TX buffer size is not a power of 2
#endif

#define UART1_RX_BUFFER_MASK ( UART1_RX_BUFFER_SIZE - 1 )
#if ( UART1_RX_BUFFER_SIZE & UART1_RX_BUFFER_MASK )
   #error RX buffer size is not a power of 2
#endif

#define UART1_TX_BUFFER_MASK ( UART1_TX_BUFFER_SIZE - 1 )
#if ( UART1_TX_BUFFER_SIZE & UART1_TX_BUFFER_MASK )
   #error TX buffer size is not a power of 2
#endif
////////////////////////////////////////////////////////////////////////////////
/* Static Variables */
static unsigned char UART0_RxBuf[UART0_RX_BUFFER_SIZE];
static unsigned char UART0_TxBuf[UART0_TX_BUFFER_SIZE];
static unsigned char UART1_RxBuf[UART1_RX_BUFFER_SIZE];
static unsigned char UART1_TxBuf[UART1_TX_BUFFER_SIZE];
////////////////////////////////////////////////////////////////////////////////

extern void UART0_init(void)//inicjalizacja komunikacji
{
unsigned char x;
   
	//UBRRH=0x01;
	//UBRRL=0xA0;		//dla 2400@16MHz
	//UBRRL=207;		//dla 4800@16MHz
	//UBRRL=103;		//dla 9600@16MHz
	//UBRRL=51;			//dla 19200@16MHz
	UBRR0L=16;			//dla 57600@16MHz
	//UBRRL=8;			//dla 57600@8MHz
	//UBRRL=8;			//dla 115200@16MHz
   
   UCSR0B=0x98;//10011000
   UCSR0C=0x86;
   
   x = 0;			   /* Flush receive buffer */
   UART0_RxTail = x;
   UART0_RxHead = x;
   UART0_TxTail = x;
   UART0_TxHead = x;
   
   uart_echo=0;
   
}
extern void UART1_init(void)//inicjalizacja komunikacji
{
unsigned char x;
   
	//UBRRH=0x01;
	//UBRRL=0xA0;		//dla 2400@16MHz
	//UBRRL=207;		//dla 4800@16MHz
	//UBRRL=103;		//dla 9600@16MHz
	//UBRRL=51;			//dla 19200@16MHz
	UBRR1L=16;			//dla 57600@16MHz
	//UBRRL=8;			//dla 57600@8MHz
	//UBRRL=8;			//dla 115200@16MHz
   
   UCSR1B=0x98;//10011000
   UCSR1C=0x86;
   
   x = 0;			   /* Flush receive buffer */
   UART1_RxTail = x;
   UART1_RxHead = x;
   UART1_TxTail = x;
   UART1_TxHead = x;
   
   uart_echo=0;
   
}
////////////////////////////////////////////////////////////////////////////////
//Wylaczanie RS
////////////////////////////////////////////////////////////////////////////////
extern void UART0_disable(void)
{
	UCSR0B &= ~((1<<RXCIE)|(1<<TXCIE)|(1<<UDRIE)|(1<<RXEN)|(1<<TXEN));	
}
extern void UART1_disable(void)
{
	UCSR1B &= ~((1<<RXCIE)|(1<<TXCIE)|(1<<UDRIE)|(1<<RXEN)|(1<<TXEN));	
}
////////////////////////////////////////////////////////////////////////////////
//Przerwanie od odebranego znaku
////////////////////////////////////////////////////////////////////////////////
SIGNAL(SIG_UART0_RECV)
{
   
	static unsigned char data;
	static unsigned char tmphead;

	data = UDR0; 						//przepisz rejestr do data

   	
	tmphead = ( UART0_RxHead + 1 ) & UART0_RX_BUFFER_MASK;
									//oblicz wskasnik bufora
	UART0_RxHead = tmphead;		

	if ( tmphead == UART0_RxTail )	//obsluga bledu przepelnienia bufora
	{
		UART0_flush_rx_buffer();  	

	}

	else
	{
	UART0_RxBuf[tmphead] = data; 	//zapisz dane w buforze odbiorczym
	}

}
SIGNAL(SIG_UART1_RECV)
{
   
	static unsigned char data;
	static unsigned char tmphead;

	data = UDR1; 						//przepisz rejestr do data

   	
	tmphead = ( UART1_RxHead + 1 ) & UART1_RX_BUFFER_MASK;
									//oblicz wskasnik bufora
	UART1_RxHead = tmphead;		

	if ( tmphead == UART1_RxTail )	//obsluga bledu przepelnienia bufora
	{
		UART1_flush_rx_buffer();  	

	}

	else
	{
	UART1_RxBuf[tmphead] = data; 	//zapisz dane w buforze odbiorczym
	}

}
////////////////////////////////////////////////////////////////////////////////
//Przerwanie od pustego rejestru nadawczego (wywolywane jest cyklicznie
//jesli dane zostaly wyslane, dlatego jest niezbedne jego wlaczanie/wylaczanie)
////////////////////////////////////////////////////////////////////////////////
SIGNAL(SIG_UART0_DATA)
{
	
	static unsigned char tmptail;
	static unsigned char temp;
	
	if ( UART0_TxHead != UART0_TxTail )//sprawdz czy wyslano juz wszystkie dane
	{
		tmptail = ( UART0_TxTail + 1 ) & UART0_TX_BUFFER_MASK;
		UART0_TxTail = tmptail;	  
		temp = UART0_TxBuf[tmptail];	//rozpocznij transmisje
		UDR0 = temp;

	}
	else
	{
	  	UCSR0B &= ~(1<<UDRIE);			//wylacz przerwania UDRE
	}
}
SIGNAL(SIG_UART1_DATA)
{
	
	static unsigned char tmptail;
	static unsigned char temp;
	
	if ( UART1_TxHead != UART1_TxTail )//sprawdz czy wyslano juz wszystkie dane
	{
		tmptail = ( UART1_TxTail + 1 ) & UART1_TX_BUFFER_MASK;
		UART1_TxTail = tmptail;	  
		temp = UART1_TxBuf[tmptail];	//rozpocznij transmisje
		UDR1 = temp;

	}
	else
	{
	  	UCSR1B &= ~(1<<UDRIE);			//wylacz przerwania UDRE
	}
}
////////////////////////////////////////////////////////////////////////////////
//Odbieranie danych - pobieranie z bufora
////////////////////////////////////////////////////////////////////////////////
extern unsigned char UART0_receive_byte( void )
{
	unsigned char tmptail;
	
	
	while(UART0_RxHead == UART0_RxTail);
	
   	tmptail = ( UART0_RxTail + 1 ) & UART0_RX_BUFFER_MASK;
   									//pobierz dane z bufora
   	UART0_RxTail = tmptail;	
   	

   
	return UART0_RxBuf[tmptail]; 
}
extern unsigned char UART1_receive_byte( void )
{
	unsigned char tmptail;
	
	
	while(UART1_RxHead == UART1_RxTail);
	
   	tmptail = ( UART1_RxTail + 1 ) & UART1_RX_BUFFER_MASK;
   									//pobierz dane z bufora
   	UART1_RxTail = tmptail;	
   	

   
	return UART1_RxBuf[tmptail]; 
}
////////////////////////////////////////////////////////////////////////////////
//Wysylanie bajtu poprzez wpisanie do bufora nadawczeo i uruchomienie przerwania
//od pustego rejestru danej do wyslania
////////////////////////////////////////////////////////////////////////////////
extern void UART0_transmit_byte( unsigned char data )
{
	unsigned char tmphead;
	

	
   	tmphead = ( UART0_TxHead + 1 ) & UART0_TX_BUFFER_MASK; 
   									
   	while ( tmphead == UART0_TxTail );
   									//czekaj na wolne miejsce w buforze
   	
   	UART0_TxBuf[tmphead] = data;		//zapisz dane w buforze
   	UART0_TxHead = tmphead;			

   	UCSR0B |= (1<<UDRIE);				//uruchom przerwania
   	
}
extern void UART1_transmit_byte( unsigned char data )
{
	unsigned char tmphead;
	

	
   	tmphead = ( UART1_TxHead + 1 ) & UART1_TX_BUFFER_MASK; 
   									
   	while ( tmphead == UART1_TxTail );
   									//czekaj na wolne miejsce w buforze
   	
   	UART1_TxBuf[tmphead] = data;		//zapisz dane w buforze
   	UART1_TxHead = tmphead;			

   	UCSR1B |= (1<<UDRIE);				//uruchom przerwania
   	
}
////////////////////////////////////////////////////////////////////////////////
//Sprawdzenie, czy w buforze rx sa dane
////////////////////////////////////////////////////////////////////////////////
extern unsigned char UART0_data_in_rx_buffer( void )
{

	
   return ( UART0_RxHead != UART0_RxTail );  
   									//zwroc zero jesli bufor wejsciowy pusty
}
extern unsigned char UART1_data_in_rx_buffer( void )
{
   return ( UART1_RxHead != UART1_RxTail );  
   									//zwroc zero jesli bufor wejsciowy pusty
}
////////////////////////////////////////////////////////////////////////////////
//Sprawdzenie, czy sa dane do przeslania w buforze tx
////////////////////////////////////////////////////////////////////////////////
extern unsigned char UART0_data_in_tx_buffer( void )
{
   return ( UART0_TxHead != UART0_TxTail ); 
   									//zwroc zero jesli bufor wyjsciowy pusty
}
extern unsigned char UART1_data_in_tx_buffer( void )
{
   return ( UART1_TxHead != UART1_TxTail ); 
   									//zwroc zero jesli bufor wyjsciowy pusty
}
////////////////////////////////////////////////////////////////////////////////
//Odczytywanie ilosci bajtow w buforach
////////////////////////////////////////////////////////////////////////////////
extern unsigned char UART0_bytes_in_rx_buffer(void)
{
	unsigned char temp;
	unsigned char sreg;
	sreg=SREG;
	cli();
	
	if(UART0_RxHead >= UART0_RxTail)
	{
		temp=UART0_RxHead-UART0_RxTail;
		SREG=sreg;
		return(temp);
	}
	else
	{
		temp=UART0_RX_BUFFER_SIZE-UART0_RxTail+UART0_RxHead;
		SREG=sreg;
		return(temp);
	}
	
}
extern unsigned char UART1_bytes_in_rx_buffer(void)
{
	unsigned char temp;
	unsigned char sreg;
	sreg=SREG;
	cli();
	
	if(UART1_RxHead >= UART1_RxTail)
	{
		temp=UART1_RxHead-UART1_RxTail;
		SREG=sreg;
		return(temp);
	}
	else
	{
		temp=UART1_RX_BUFFER_SIZE-UART1_RxTail+UART1_RxHead;
		SREG=sreg;
		return(temp);
	}
	
}
extern unsigned char UART0_bytes_in_tx_buffer(void)
{
	unsigned char temp;
	unsigned char sreg;
	sreg=SREG;
	cli();
	
	if(UART0_TxHead >= UART0_TxTail)
	{
		temp=UART0_TxHead-UART0_TxTail;
		SREG=sreg;
		return(temp);
	}
	else
	{
		temp=UART0_TX_BUFFER_SIZE-UART0_TxTail+UART0_TxHead;
		SREG=sreg;
		return(temp);
	}
}
extern unsigned char UART1_bytes_in_tx_buffer(void)
{
	unsigned char temp;
	unsigned char sreg;
	sreg=SREG;
	cli();
	
	if(UART1_TxHead >= UART1_TxTail)
	{
		temp=UART1_TxHead-UART1_TxTail;
		SREG=sreg;
		return(temp);
	}
	else
	{
		temp=UART1_TX_BUFFER_SIZE-UART1_TxTail+UART1_TxHead;
		SREG=sreg;
		return(temp);
	}
}
////////////////////////////////////////////////////////////////////////////////
//Czyszczenie buforow
////////////////////////////////////////////////////////////////////////////////
extern void UART0_flush_tx_buffer(void)
{
	unsigned char sreg;				//zachowanie stanu rejestru statusowego
	sreg=SREG;
	cli();
		UART0_TxHead=0;
		UART0_TxTail=0;
	SREG=sreg;
}
extern void UART1_flush_tx_buffer(void)
{
	unsigned char sreg;				//zachowanie stanu rejestru statusowego
	sreg=SREG;
	cli();
		UART1_TxHead=0;
		UART1_TxTail=0;
	SREG=sreg;
}
extern void UART0_flush_rx_buffer(void)
{
	unsigned char sreg;
	sreg=SREG;
	cli();
		UART0_RxHead=0;
		UART0_RxTail=0;
	SREG=sreg;
}
extern void UART1_flush_rx_buffer(void)
{
	unsigned char sreg;
	sreg=SREG;
	cli();
		UART1_RxHead=0;
		UART1_RxTail=0;
	SREG=sreg;
}
////////////////////////////////////////////////////////////////////////////////
//Przejscie do nowej linii i dodanie znaku konca stringa (konwencja DOS - \r\n)
////////////////////////////////////////////////////////////////////////////////
extern void UART0_return(void)//przejscie do nowej linii oraz zakonczenie stringa w terminalu
{
UART0_transmit_byte('\0');
UART0_transmit_byte('\r');
UART0_transmit_byte('\n');
}
extern void UART1_return(void)//przejscie do nowej linii oraz zakonczenie stringa w terminalu
{
UART1_transmit_byte('\0');
UART1_transmit_byte('\r');
UART1_transmit_byte('\n');
}
////////////////////////////////////////////////////////////////////////////////
//Wypisywanie stringa na terminal (wlacznie ze znakiem konca stringa)
////////////////////////////////////////////////////////////////////////////////
extern unsigned char UART0_print(unsigned char *tekst)//wypisywanie na terminal
{
unsigned char i=0;


for(;;)
{
	UART0_transmit_byte(*(tekst+i));
	if(!(*(tekst+i)))	break;
	i++;
}


return i;
}
extern unsigned char UART1_print(unsigned char *tekst)//wypisywanie na terminal
{
unsigned char i=0;


for(;;)
{
	UART1_transmit_byte(*(tekst+i));
	if(!(*(tekst+i)))	break;
	i++;
}


return i;
}
////////////////////////////////////////////////////////////////////////////////
//Odczytywanie stringow lub linii z rsa
////////////////////////////////////////////////////////////////////////////////
extern unsigned char UART0_gets(unsigned char *buf,unsigned char ile)
{
unsigned char i=0;

while(i<ile)
{
	*(buf+i)=UART0_receive_byte();

	if((*(buf+i)=='\r')||(*(buf+i)=='\n')||(*(buf+i)=='\0'))
		{
			*(buf+i)='\0';
			return(i+1);	//zwroc ilosc znakow wraz ze znakiem \0
		}
	else
		{
			i++;//przestawienie wskaznika na nowa pozyzje tablicy
		}
}

*(buf+i-1)='\0';
return(i);							//zwracamy liczbe odebranych bajtow
}
extern unsigned char UART1_gets(unsigned char *buf,unsigned char ile)
{
unsigned char i=0;

while(i<ile)
{
	*(buf+i)=UART1_receive_byte();

	if((*(buf+i)=='\r')||(*(buf+i)=='\n')||(*(buf+i)=='\0'))
		{
			*(buf+i)='\0';
			return(i+1);	//zwroc ilosc znakow wraz ze znakiem \0
		}
	else
		{
			i++;//przestawienie wskaznika na nowa pozyzje tablicy
		}
}

*(buf+i-1)='\0';
return(i);							//zwracamy liczbe odebranych bajtow
}

////////////////////////////////////////////////////////////////////////////////
//Przesylanie danych binarnych
////////////////////////////////////////////////////////////////////////////////
extern void UART0_send_block(const void *var, unsigned char cnt)
{
	unsigned char* ptr;
	ptr=(unsigned char *)var;
					
	for(unsigned int i=0;i<cnt;i++)
	{
		UART0_transmit_byte(*(ptr));
		ptr++;
	}
}
extern void UART1_send_block(const void *var, unsigned char cnt)
{
	unsigned char* ptr;
	ptr=(unsigned char *)var;
					
	for(unsigned int i=0;i<cnt;i++)
	{
		UART1_transmit_byte(*(ptr));
		ptr++;
	}
}
////////////////////////////////////////////////////////////////////////////////
//Odbieranie danych binarnych
////////////////////////////////////////////////////////////////////////////////
extern void UART0_receive_block(void *var, unsigned char cnt)
{
	unsigned char* ptr;
	ptr=(unsigned char *)var;
					
	for(unsigned int i=0;i<cnt;i++)
	{
		*(ptr)=UART0_receive_byte();
		ptr++;
	}
}
extern void UART1_receive_block(void *var, unsigned char cnt)
{
	unsigned char* ptr;
	ptr=(unsigned char *)var;
					
	for(unsigned int i=0;i<cnt;i++)
	{
		*(ptr)=UART1_receive_byte();
		ptr++;
	}
}
