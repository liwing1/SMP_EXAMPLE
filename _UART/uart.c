/*
 * uart.c
 *
 *  Created on: 13 de dez de 2023
 *      Author: rails
 */
#include "uart.h"

void UART_init(uint16_t baseAddress, uint32_t baudRate) {
    // Configure UART
    EUSCI_A_UART_initParam uartConfig = {
        .selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .clockPrescalar = 163,
        .firstModReg = 13,
        .secondModReg = 85,
        .parity = EUSCI_A_UART_NO_PARITY,
        .msborLsbFirst = EUSCI_A_UART_LSB_FIRST,
        .numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,
        .uartMode = EUSCI_A_UART_MODE,
        .overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };

    EUSCI_A_UART_init(baseAddress, &uartConfig);
    EUSCI_A_UART_enable(baseAddress);

    EUSCI_A_UART_enableInterrupt(baseAddress, EUSCI_A_UART_RECEIVE_INTERRUPT|EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT);
}

void UART_sendChar(uint16_t baseAddress, char data) {
    while (!EUSCI_A_UART_getInterruptStatus(baseAddress, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG));
    EUSCI_A_UART_transmitData(baseAddress, data);
}

char UART_receiveChar(uint16_t baseAddress) {
    while (!EUSCI_A_UART_getInterruptStatus(baseAddress, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG));
    return EUSCI_A_UART_receiveData(baseAddress);
}

void UART_sendString(uint16_t baseAddress, uint8_t* str){
    uint8_t* ptr;
    for(ptr = str; *ptr != '\0'; *ptr++)
    {
        UART_sendChar(baseAddress, *ptr);
    }
}
