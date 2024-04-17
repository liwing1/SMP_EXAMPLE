/*
 * led.c
 *
 *  Created on: 13 de dez de 2023
 *      Author: rails
 */
#include <_CONFIG/config.h>
#include <_LED/led.h>

void PiscaLed(void){

    GPIO_toggleOutputOnPin(LED_PORT_3, LED_PIN_2);
    __delay_cycles(10000);
    GPIO_toggleOutputOnPin(LED_PORT_8, LED_PIN_6);
    __delay_cycles(10000);
    GPIO_toggleOutputOnPin(LED_PORT_8, LED_PIN_5);
    __delay_cycles(10000);
    GPIO_toggleOutputOnPin(LED_PORT_8, LED_PIN_4);
    __delay_cycles(10000);
    GPIO_toggleOutputOnPin(LED_PORT_3, LED_PIN_3);
    __delay_cycles(10000);

}


