/*
 * UART.c
 *
 * Created: 22.04.2018 20:19:41
 * Author : Artemka
 */ 

#include "USART.h"

volatile u8 usartRxBuf[USART_SIZE_BUF];
volatile u8 rxBufTail					= 0;
volatile u8 rxBufHead					= 0;
volatile u8 rxCount						= 0;

volatile u8	usartTxBuf[USART_SIZE_BUF];
volatile u8	txBufTail					= 0;
volatile u8	txBufHead					= 0;
volatile u8	txCount						= 0;

void USART_Init(void)
{
	UBRR0H = 0;
	UBRR0L = BAUD_RATE(USART_CPU_CLK, USART_BAUD_RATE);		// Определяем скорость UART

	//разр. прерыв при приеме и передачи, разр приема, разр передачи.
	UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);

	//размер слова 8 разрядов
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_FlushTxBuf(void)
{
	txBufTail = 0;
	txCount = 0;
	txBufHead = 0;
}

void USART_PutChar(u8 sym)
{
	if(BitIsSet(UCSR0A, UDRE0) && (txCount == 0)) UDR0 = sym;
	else {
		if (txCount < USART_SIZE_BUF)				// если в буфере еще есть место
		{               
			usartTxBuf[txBufTail] = sym;    // помещаем в него символ
			txCount++;                      // инкрементируем счетчик символов
			txBufTail++;                    // и индекс хвоста буфера
			if (txBufTail == USART_SIZE_BUF) txBufTail = 0;
		}
	}
}

void USART_SendStr(u8 * data)
{
	u8 sym;
	while(*data)
	{
		sym = *data++;
		USART_PutChar(sym);
	}
}

u8 USART_GetChar(void)
{
	u8 sym;
	if (rxCount > 0){                     
		sym = usartRxBuf[rxBufHead];        
		rxCount--;                          
		rxBufHead++;                        
		if (rxBufHead == USART_SIZE_BUF) rxBufHead = 0;
		return sym;                         
	}
	return 0;
}

void USART_FlushRxBuf(void)
{
	rxBufTail = 0;
	rxBufHead = 0;
	rxCount = 0;
}

ISR(USART_TX_vect)
{
	  if (txCount > 0){								// если буфер не пустой
		  UDR0 = usartTxBuf[txBufHead];				// записываем в UDR символ из буфера
		  txCount--;								// уменьшаем счетчик символов
		  txBufHead++;								// инкрементируем индекс головы буфера
		  if (txBufHead == USART_SIZE_BUF) txBufHead = 0;
	  }
}

ISR(USART_RX_vect)
{
	  if (rxCount < USART_SIZE_BUF){                
		  usartRxBuf[rxBufTail] = UDR0;        
		  rxBufTail++;                             
		  if (rxBufTail == USART_SIZE_BUF) rxBufTail = 0;
		  rxCount++;                                 
	  }
}

u8 USART_GetRxCount(void)
{
	return rxCount;
}

