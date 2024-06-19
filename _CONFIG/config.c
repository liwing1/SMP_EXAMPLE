/*
 * config.c
 *
 *  Created on: 11 de dez de 2023
 *      Author: rails
 */

#include <_CONFIG/config.h>
#include <_UART/uart.h>
#include "driverlib.h"
volatile uint16_t ADC_Result_Average;
uint8_t i;
uint16_t ADC_Result_sum;
uint16_t ADC_Result[64];


void PinConfig(void)
{

    /* Set debug LED as output */
    GPIO_setAsOutputPin(LED_PORT_8, LED_PIN_4);
    GPIO_setAsOutputPin(LED_PORT_8, LED_PIN_5);
    GPIO_setAsOutputPin(LED_PORT_8, LED_PIN_6);
    GPIO_setAsOutputPin(LED_PORT_3, LED_PIN_2);
    GPIO_setAsOutputPin(LED_PORT_3, LED_PIN_3);

    GPIO_setOutputLowOnPin(LED_PORT_8, LED_PIN_4);
    GPIO_setOutputLowOnPin(LED_PORT_8, LED_PIN_5);
    GPIO_setOutputLowOnPin(LED_PORT_8, LED_PIN_6);
    GPIO_setOutputLowOnPin(LED_PORT_3, LED_PIN_2);
    GPIO_setOutputLowOnPin(LED_PORT_3, LED_PIN_3);

    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);

    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
}

void ClockConfig(void)
{
//    // Set VCore = 1 for 12MHz clock
//    PMM_setVCore(PMM_CORE_LEVEL_3);
//
//    //    //ACLK, MCLK, MCLK set out to pins
////    GPIO_setAsPeripheralModuleFunctionOutputPin(
////        GPIO_PORT_P11,
////        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2
////        );
//
//    // Initialize FLL based on REFO = 32728 Hz
//    UCS_initClockSignal(
//        UCS_FLLREF,
//        UCS_REFOCLK_SELECT,
//        UCS_CLOCK_DIVIDER_1
//    );
//
//    // Initialize DCO based on FLL
//    // UCS_DCO_FREQUENCY = FLL_FREQUENCY * UCS_DCO_RATIO
//    UCS_initFLLSettle(
//        UCS_DCO_FREQUENCY,
//        UCS_DCO_RATIO
//    );
//
//    // Initialize ACLK based on REFO
//    UCS_initClockSignal(
//        UCS_ACLK,
//        UCS_REFOCLK_SELECT,
//        UCS_CLOCK_DIVIDER_1
//    );
//
//    // Initialize MCLK based on DCO
//    UCS_initClockSignal(
//        UCS_MCLK,
//        UCS_DCOCLK_SELECT,
//        UCS_CLOCK_DIVIDER_1
//    );
//
//    // Initialize SMCLK based on DCO
//    UCS_initClockSignal(
//        UCS_SMCLK,
//        UCS_DCOCLK_SELECT,
//        UCS_CLOCK_DIVIDER_1
//    );

    /*
     * TESTESTESTES
     * */
    PMM_setVCore(PMM_CORE_LEVEL_3); //    SetVCore(3);
    __bis_SR_register(SCG0);                        /* Disable the FLL control loop */
    UCSCTL6 = (UCSCTL6 | XT1DRIVE_3);               /* Highest drive setting for XT1 startup */
    while (SFRIFG1 & OFIFG)
    {
        /* Check OFIFG fault flag */
        UCSCTL7 &= ~(DCOFFG | XT1LFOFFG | XT2OFFG); /* Clear OSC fault flags */
        SFRIFG1 &= ~OFIFG;                          /* Clear OFIFG fault flag */
    }
    UCSCTL6 = (UCSCTL6 & ~(XT1DRIVE_3));            /* Reduce drive to something weaker */

    UCSCTL0 = 0;
    UCSCTL1 = DCORSEL_6;                            /* Set RSELx for DCO = 25MHz */
    UCSCTL2 = FLLD_2 | (192 - 1);                   /* Set DCO Multiplier for 25MHz */
                                                    /* Set FLL to 32768*4*192 => 25165824Hz */
    __bic_SR_register(SCG0);                        /* Enable the FLL control loop */

    UCSCTL5 |= DIVS__1;
    UCSCTL4 = SELM__DCOCLK | SELS__DCOCLK | SELA__XT1CLK;     /* 24MHz MCLK, 24MHz SMCLK, 32KHz ACLK */
    /*
     * TESTESTESTES
     * */
}

void UartConfig(uint16_t uart_config){
    // Configuração dos pinos UART (TX e RX)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN5);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN2); //UCA2RX
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN3); //UCA2TX

    // Inicialização da UART
    UART_init(uart_config, 115200);

}

void TimerConfig(void)
{
    /* Clear TAIFG and CCIFG interrupts */
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

    Timer_A_clearCaptureCompareInterrupt(
        TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_1
    );

    Timer_A_clearCaptureCompareInterrupt(
        TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_2
    );

    /* TA1 parameter definition */
    Timer_A_initContinuousModeParam param = {0};
    param.clockSource =                             TIMER_A_CLOCKSOURCE_ACLK;                   // Clock source = ACLK
    param.clockSourceDivider =                      TIMER_A_CLOCKSOURCE_DIVIDER_1;              // Clock source prescaler = 1
    param.timerInterruptEnable_TAIE =               TIMER_A_TAIE_INTERRUPT_DISABLE;             // Roll over interrupts
    param.timerClear =                              TIMER_A_DO_CLEAR;                           // Reset counting process
    param.startTimer =                              true;                                       // Immediately start?
    Timer_A_initContinuousMode(TIMER_A1_BASE, &param);                                          // Set up TA configuration

    /* TA1 CCR1 parameter definition */
    Timer_A_initCompareModeParam initCCR1Param = {0};
    initCCR1Param.compareRegister =                 TIMER_A_CAPTURECOMPARE_REGISTER_1;
    initCCR1Param.compareInterruptEnable =          TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCCR1Param.compareOutputMode =               TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCCR1Param.compareValue =                    (int) round(CCR1_COMP_VALUE);
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCCR1Param);

    /* TA1 CCR2 parameter definition */
    Timer_A_initCompareModeParam initCCR2Param = {0};
    initCCR2Param.compareRegister =                 TIMER_A_CAPTURECOMPARE_REGISTER_2;
    initCCR2Param.compareInterruptEnable =          TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCCR2Param.compareOutputMode =               TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCCR2Param.compareValue =                    (int) round(CCR2_COMP_VALUE);
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCCR2Param);

    //Set up Timer A
    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    initCompParam.compareValue = 0x8000;
    Timer_A_initCompareMode(TIMER_A0_BASE, &initCompParam);

    //Start timer A0
    Timer_A_initUpDownModeParam initUpDownParam = {0};
    initUpDownParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initUpDownParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initUpDownParam.timerPeriod = 0xFFFE;
    initUpDownParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initUpDownParam.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    initUpDownParam.timerClear = TIMER_A_DO_CLEAR;
    initUpDownParam.startTimer = true;
    Timer_A_initUpDownMode(TIMER_A0_BASE, &initUpDownParam);

    //Enter LPM0, enable interrupts
    __bis_SR_register(GIE);
}


//void SD24B(void){
//    REFCTL0 = REFMSTR | REFVSEL_1 | REFON;
////    SD_CTL1 &= ~SD24GRP0SC;
//
//    SD24BCTL0 |= SD24SSEL_1; // Seleciona SMCLK como fonte de clock SD24_B
//    SD24BCTL0 |= SD24REFS; // Seleciona REF interna
//
//    SD24BCCTL0 = SD24SNGL | SD24SCS_5; // Conversão única, grupo 1
//
//    SD24BIE |= SD24IE2; //Habilita interrupção do canal 2
//}

void AdcDMAConfig(void)
{

        //Initialize the ADC10_A Module
        /*
         * Base Address for the ADC10_A Module
         * Use internal ADC10_A bit as sample/hold signal to start conversion
         * USE MODOSC 5MHZ Digital Oscillator as clock source
         * Use default clock divider of 1
         */
        ADC10_A_init(ADC10_A_BASE,
            ADC10_A_SAMPLEHOLDSOURCE_SC,
            ADC10_A_CLOCKSOURCE_ADC10OSC,
            ADC10_A_CLOCKDIVIDER_1);

        ADC10_A_enable(ADC10_A_BASE);
        /*
         * Base Address for the ADC10_A Module
         * Sample/hold for 16 clock cycles
         * Enable Multiple Sampling
         */
        ADC10_A_setupSamplingTimer(ADC10_A_BASE,
            ADC10_A_CYCLEHOLD_16_CYCLES,
            ADC10_A_MULTIPLESAMPLESDISABLE);

        //Configure Memory Buffer
        /*
         * Base Address for the ADC10_A Module
         * Use input A1
         * Use positive reference of AVcc
         * Use negative reference of AVss
         */
        ADC10_A_configureMemory(ADC10_A_BASE,
            ADC10_A_INPUT_A5,
            ADC10_A_VREFPOS_AVCC,
            ADC10_A_VREFNEG_AVSS);

        ADC10_A_clearInterrupt(ADC10_A_BASE,
             ADC10IFG0);
        ADC10_A_enableInterrupt(ADC10_A_BASE,
             ADC10IE0);

//        //Initialize and Setup DMA Channel 0
//         /*
//          * Configure DMA channel 0
//          * Configure channel for repeated single transfer
//          * DMA interrupt flag will be set after every 64 transfers
//          * Use DMA Trigger Source 24 (ADC10IFG)
//          * Transfer Word-to-Word
//          * Trigger upon Rising Edge of Trigger Source Signal
//          */
//         DMA_initParam param = {0};
//         param.channelSelect = DMA_CHANNEL_0;
//         param.transferModeSelect = DMA_TRANSFER_REPEATED_SINGLE;
//         param.transferSize = 64;
//         param.triggerSourceSelect = DMA_TRIGGERSOURCE_24;
//         param.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
//         param.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;
//         DMA_init(&param);
//         /*
//          * Configure DMA channel 0
//          * Use ADC10_A Memory Buffer as source
//          * Increment destination address after every transfer
//          */
//         DMA_setSrcAddress(DMA_CHANNEL_0,
//             ADC10_A_getMemoryAddressForDMA(ADC10_A_BASE),
//             DMA_DIRECTION_UNCHANGED);
//         /*
//          * Base Address for the DMA Module
//          * Configure DMA channel 0
//          * Use ADC_Result[0] as destination
//          * Increment destination address after every transfer
//          */
//         DMA_setDstAddress(DMA_CHANNEL_0,
//             (uint32_t)(uintptr_t)&ADC_Result[0],
//             DMA_DIRECTION_INCREMENT);
//
//         //Enable DMA channel 0 interrupt
//         DMA_clearInterrupt(DMA_CHANNEL_0);
//         DMA_enableInterrupt(DMA_CHANNEL_0);
//
//         //Enable transfers on DMA channel 0
//         DMA_enableTransfers(DMA_CHANNEL_0);
}

uint16_t ADC(void){

    //Enable and Start the conversion
      //in Repeated Single-Channel Conversion Mode
      ADC10_A_startConversion(ADC10_A_BASE,
          ADC10_A_REPEATED_SINGLECHANNEL);

//      __bis_SR_register(CPUOFF + GIE);        //LPM0, ADC10_A_ISR will force exit
//      __no_operation();                       //For debug only

      __delay_cycles(50000);

      return ADC10_A_getResults(ADC10_A_BASE);
  }

