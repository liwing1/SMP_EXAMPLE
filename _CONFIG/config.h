/*
 * config.h
 *
 *  Created on: 26 de out de 2023
 *      Author: Pedro
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include "driverlib.h"
#include <math.h>

/* Clock definition */

// DCO frequency
#define UCS_DCO_FREQUENCY   25000

// DCO-FLL frequency ratio
#define UCS_DCO_RATIO   762

/**
 * ClockConfig()
 *
 * @brief Clock configuration
 * @param none
 * @return none
 */

// Debug LED pin
#define LED_PORT_8 GPIO_PORT_P8
#define LED_PORT_3 GPIO_PORT_P3
#define LED_PIN_2 GPIO_PIN2
#define LED_PIN_3 GPIO_PIN3
#define LED_PIN_4 GPIO_PIN4
#define LED_PIN_5 GPIO_PIN5
#define LED_PIN_6 GPIO_PIN6

/* Timer definition */

// Project frequencies
#define FREQ_INSTRUMENTATION 4096
#define FREQ_TEMP_BACKGROUND 100

// Timer CCR compare values based on ACLK and project frequencies
#define CCR1_COMP_VALUE 32768.0 / FREQ_INSTRUMENTATION
#define CCR2_COMP_VALUE 32768.0 / FREQ_TEMP_BACKGROUND

/**
 * TimerConfig()
 *
 * @brief Essentially, Timer TA1 will be used in continuousMode, i.e rolling the counter over only on overflow.
 *        The following capture-compare interrupts are programmed and expected:
 *        1. The CCR1, triggered at 4096Hz
 *        2. The CCR2, triggered at 100Hz
 * @param none
 * @return none
 */

void PinConfig(void);
void ClockConfig(void);
void UartConfig(void);
void TimerConfig(void);
void AdcDMAConfig(void);
uint16_t ADC(void);


#endif /* CONFIG_CONFIG_H_ */



