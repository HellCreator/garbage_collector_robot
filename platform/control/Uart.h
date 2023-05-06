////////////////////////////////////////////////////////////////////////////////
//Copyright (c) 2005
//	Lukasz Matecki
//	lutecki@op.pl
////////////////////////////////////////////////////////////////////////////////
//April 25, 2006 08:57:56 AM 
////////////////////////////////////////////////////////////////////////////////
#ifndef _UART_H_
#define _UART_H_

////////////////////////////////////////////////////////////////////////////////
//Uart
////////////////////////////////////////////////////////////////////////////////

#define UART0_RX_BUFFER_SIZE 128 	//2,4,8,16,32,64,128 lub 256 bajtow
#define UART0_TX_BUFFER_SIZE 64

#define UART1_RX_BUFFER_SIZE 128 	//2,4,8,16,32,64,128 lub 256 bajtow
#define UART1_TX_BUFFER_SIZE 64
/*	
#ifdef _AVR_IOM128_H_
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define UDR UDR0
#define UBRRL UBRR0L
#define SIG_UART_RECV USART0_RX_vect
#define SIG_UART_DATA USART0_UDRE_vect
#endif
*/

#ifdef _AVR_IOM128_H_
#define UCSRA UCSR1A
#define UCSRB UCSR1B
#define UCSRC UCSR1C
#define UDR UDR1
#define UBRRL UBRR1L
#define SIG_UART_RECV USART1_RX_vect
#define SIG_UART_DATA USART1_UDRE_vect
#endif

#define UART_CR UART_transmit_byte('\r')
#define UART_LF UART_transmit_byte('\n')
#define UART_NULL UART_transmit_byte('\0')
#define UART_RET UART_transmit_byte('\r');UART_transmit_byte('\n')

#define UART_RX_DISABLE UCSRB&=~(1<<RXEN)
#define UART_RX_ENABLE UCSRB|=(1<<RXEN)
#define UART_TX_DISABLE UCSRB&=~(1<<TXEN)
#define UART_TX_ENABLE UCSRB|=(1<<TXEN)

#define UART_RX_INT_DISABLE UCSRB&=~(1<<RXCIE)
#define UART_RX_INT_ENABLE UCSRB|=(1<<RXCIE)

#define UART_TX_INT_DISABLE UCSRB&=~(1<<UDRIE)
#define UART_TX_INT_ENABLE UCSRB|=(1<<UDRIE)

#define SEND_ESC UART_transmit_byte(27);UART_transmit_byte('[')
#define UART_CLS SEND_ESC;UART_transmit_byte('2');UART_transmit_byte('J')
#define UART_CLEAR_TO_END SEND_ESC;UART_transmit_byte('K')
#define UART_RESET UART_transmit_byte(27);UART_transmit_byte('c')
#define UART_HOME UART_transmit_byte(27);UART_transmit_byte('H')


unsigned char uart_echo;

extern void UART0_init(void);
extern void UART0_disable(void);
extern unsigned char UART0_receive_byte( void );
extern void UART0_buffer_rx_byte(unsigned char data);
extern unsigned char UART0_buffer_rx_block(unsigned char *tekst);
extern void UART0_transmit_byte( unsigned char data );
extern unsigned char UART0_data_in_rx_buffer(void);
extern unsigned char UART0_data_in_tx_buffer(void);
extern unsigned char UART0_bytes_in_rx_buffer(void);
extern unsigned char UART0_bytes_in_tx_buffer(void);
extern unsigned char UART0_print(unsigned char *tekst);
extern unsigned char UART0_gets(unsigned char *buf,unsigned char ile);
extern void UART0_return(void);
extern void UART0_send_block(const void *var, unsigned char cnt);
extern void UART0_receive_block(void *var, unsigned char cnt);

extern void UART1_init(void);
extern void UART1_disable(void);
extern unsigned char UART1_receive_byte( void );
extern void UART1_buffer_rx_byte(unsigned char data);
extern unsigned char UART1_buffer_rx_block(unsigned char *tekst);
extern void UART1_transmit_byte( unsigned char data );
extern unsigned char UART1_data_in_rx_buffer(void);
extern unsigned char UART1_data_in_tx_buffer(void);
extern unsigned char UART1_bytes_in_rx_buffer(void);
extern unsigned char UART1_bytes_in_tx_buffer(void);
extern unsigned char UART1_print(unsigned char *tekst);
extern unsigned char UART1_gets(unsigned char *buf,unsigned char ile);
extern void UART1_return(void);
extern void UART1_send_block(const void *var, unsigned char cnt);
extern void UART1_receive_block(void *var, unsigned char cnt);


extern void UART0_flush_rx_buffer(void);
extern void UART0_flush_tx_buffer(void);
extern void UART1_flush_rx_buffer(void);
extern void UART1_flush_tx_buffer(void);


volatile unsigned char UART0_TxHead;
volatile unsigned char UART0_TxTail;
volatile unsigned char UART0_RxHead;
volatile unsigned char UART0_RxTail;

volatile unsigned char UART1_TxHead;
volatile unsigned char UART1_TxTail;
volatile unsigned char UART1_RxHead;
volatile unsigned char UART1_RxTail;


#endif
