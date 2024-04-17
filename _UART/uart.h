/*
 * uart.h
 *
 *  Created on: 13 de dez de 2023
 *      Author: rails
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "driverlib.h"

void UART_init(uint16_t baseAddress, uint32_t baudRate);
void UART_sendChar(uint16_t baseAddress, char data);
char UART_receiveChar(uint16_t baseAddress);


#endif /* UART_UART_H_ */

