#include <msp430.h>
#include "_LED/led.h"
#include "_UART/uart.h"
#include "_CONFIG/config.h"
#include "driverlib.h"

#include "string.h"

#define EUSCI_AX_BASE   EUSCI_A0_BASE
#if (EUSCI_AX_BASE == EUSCI_A0_BASE)
#define RS485_PIN_EN    GPIO_PIN0
#else
#define RS485_PIN_EN    GPIO_PIN1
#endif

uint8_t rx_bytes[32] = {0};
uint8_t rx_ptr = 0;

/**
 * main.c
 */

static bool is_tx_complete = false;
void RS485_sendBuf(const uint8_t* buf, uint16_t len);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    PinConfig();
    PiscaLed();
    ClockConfig();
    UartConfig(EUSCI_AX_BASE);
//    TimerConfig();
//    AdcDMAConfig();

    __enable_interrupt();

    //uint8_t buf[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    while (1)
    {
        if(rx_ptr != 0)
        {
            if(rx_bytes[0] == 0x68)
            {
                RS485_sendBuf(rx_bytes, rx_ptr);
            }
            memset(rx_bytes, 0, rx_ptr);
            rx_ptr = 0;
        }
        _delay_cycles(50000000);
    }
}

void RS485_sendBuf(const uint8_t* buf, uint16_t len){
    is_tx_complete = false;

    GPIO_setOutputHighOnPin(GPIO_PORT_P4, RS485_PIN_EN);
    _delay_cycles(500);

    uint16_t i;
    for(i = 0; i < len; i++){
        UART_sendChar(EUSCI_AX_BASE, buf[i]);
    }
    while(!(EUSCI_A_UART_getInterruptStatus(EUSCI_AX_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)));

//    _delay_cycles(500);
//    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
    is_tx_complete = true;
}


// USCI_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;              // No interrupt
        case USCI_UART_UCRXIFG:             // RXIFG
//            while (!(UCA0IFG & UCTXIFG)) ;  // USCI_A0 TX buffer ready?
            rx_bytes[rx_ptr++] = UCA0RXBUF; // TX -> RXed character
            break;
        case USCI_UART_UCTXIFG: break;      // TXIFG
        case USCI_UART_UCSTTIFG: break;     // TTIFG
        case USCI_UART_UCTXCPTIFG:
            if(is_tx_complete){
                is_tx_complete = false;
                GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            }
            break;   // TXCPTIFG
        default: break;
    }
}


// USCI_A2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A2_VECTOR))) USCI_A2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(UCA2IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;              // No interrupt
        case USCI_UART_UCRXIFG:             // RXIFG
//            while (!(UCA2IFG & UCTXIFG)) ;  // USCI_A0 TX buffer ready?
            rx_bytes[rx_ptr++] = UCA2RXBUF; // TX -> RXed character
            break;
        case USCI_UART_UCTXIFG: break;      // TXIFG
        case USCI_UART_UCSTTIFG: break;     // TTIFG
        case USCI_UART_UCTXCPTIFG:
            if(is_tx_complete){
                is_tx_complete = false;
                GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            }
            break;   // TXCPTIFG
        default: break;
    }
}

