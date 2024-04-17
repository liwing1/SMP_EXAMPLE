#include <msp430.h>
#include "_LED/led.h"
#include "_UART/uart.h"
#include "_CONFIG/config.h"
#include "driverlib.h"
uint16_t result=0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    PinConfig();
    PiscaLed();
    ClockConfig();
    UartConfig();
//    TimerConfig();
//    AdcDMAConfig();

    __enable_interrupt();


    while (1)
    {
//    // Envia um caractere pela UART
        UART_sendChar(EUSCI_A1_BASE, 'A');
//    printf("Hello World!\n");
//
//
//    // Recebe um caractere pela UART
//    char receivedChar = UART_receiveChar(EUSCI_A0_BASE);
//        result = ADC();

        _delay_cycles(500000);

    }
}


// ADC10_A interrupt service routine
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//__interrupt
//#elif defined(__GNUC__)
//__attribute__((interrupt(ADC10_VECTOR)))
//#endif
//void ADC10_A_ISR (void) {
//
//    switch (__even_in_range(ADC10IV,12)){
//        case  0: break; //No interrupt
//        case  2: break; //conversion result overflow
//        case  4: break; //conversion time overflow
//        case  6: break; //ADC10HI
//        case  8: break; //ADC10LO
//        case 10: break; //ADC10IN
//        case 12:        //ADC10IFG0
//            //Move Results from Memory Buffer
//            //(Automatically clears ADC10IFG0 by reading memory buffer)
//            result = ADC10_A_getResults(ADC10_A_BASE);
//            if (ADC10_A_getResults(ADC10_A_BASE)  < 500){
//                //Clear P1.0 LED off
//                GPIO_setOutputLowOnPin(GPIO_PORT_P8,
//                    GPIO_PIN4
//                    );
//            } else   {
//                //Set P1.0 LED on
//                GPIO_setOutputHighOnPin(
//                    GPIO_PORT_P8,
//                    GPIO_PIN4
//                    );
//            }
//            //Clear CPUOFF bit from 0(SR)
//            __bic_SR_register_on_exit(CPUOFF);
//            break;
//        default: break;
//    }
//}


