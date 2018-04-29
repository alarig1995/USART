
/*
 * USART.h
 *
 * Created: 22.04.2018 20:20:32
 *  Author: Artemka
 */ 

#ifndef USART_H
	#define USART_H
	
	//-------------------------------------------------
	#include <avr/io.h>
	#include <avr/interrupt.h>
	
	//-------------------------------------------------
	// Определяем макросы типов данных
	#define b1	bit					//	0 to 1
	#define u8	unsigned char		//	0 to 255
	#define	s8	signed char			//	-128 to 127
	#define u16	unsigned int		//	0 to 65535
	#define s16 signed int			//	-32768 to 32767
	#define u32 unsigned long int	//	0 to 4294967295
	#define s32 signed long int		//	-2147483648 to 2147483647

	//-------------------------------------------------
	// Определяем макросы для работы с битами
	#define ClearBit(reg, bit)		reg &= (~(1 << (bit)))		//	Сбросить бит в 0
	#define SetBit(reg, bit)		reg |= (1 << (bit))			//	Установить бит в 1
	#define InvertBit(reg, bit)		reg ^= (1 << (bit))			//	Инвертировать бит
	#define BitIsClear(reg, bit)	((reg & (1 << (bit))) == 0)	//	Бит сброшен?
	#define BitIsSet(reg, bit)		((reg & (1 << (bit))) != 0)	//	Бит установлен?
	
	//-------------------------------------------------
	// Необходимые определения для USART
	#define BAUD_RATE(CLK, BAUD) ((CLK) / (16 * BAUD) - 1)  
	
	#define USART_SIZE_BUF		16			// Определяем размер приемного и передающего буфферов
	#define USART_CPU_CLK		16000000UL
	#define USART_BAUD_RATE		9600UL	
	
	//-------------------------------------------------
	// Определение необходимых переменных
	
	volatile u8 usartTxBuf[USART_SIZE_BUF];
	volatile u8 txBufTail;
	volatile u8 txBufHead;
	volatile u8 txCount;
	
	volatile u8 usartRxBuf[USART_SIZE_BUF];
	volatile u8 rxBufTail;
	volatile u8 rxBufHead;
	volatile u8 rxCount;
	
	//-------------------------------------------------
	// Определение функций USART
	
	void USART_Init(void);
	
	void USART_FlushTxBuf(void);
	
	void USART_PutChar(u8 sym);
	
	void USART_SendStr(u8 * data);
	
	u8 USART_GetChar(void);
	
	void USART_FlushRxBuf(void);
	
	u8 USART_GetRxCount(void);



#endif	// USART_H
