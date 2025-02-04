/*
 *******************************************************************************
 * Copyright (c) 2016-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#include "analog.h"

#include "lock_resource.h"
#include "stm32yyxx_ll_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private_Variables */
#if (defined(HAL_ADC_MODULE_ENABLED) && !defined(HAL_ADC_MODULE_ONLY)) || \
    (defined(HAL_DAC_MODULE_ENABLED) && !defined(HAL_DAC_MODULE_ONLY))
static PinName g_current_pin = NC;
#endif

/* Private_Defines */
#if defined(HAL_ADC_MODULE_ENABLED) && !defined(HAL_ADC_MODULE_ONLY)

#ifndef ADC_SAMPLINGTIME
#if defined(ADC_SAMPLETIME_8CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_8CYCLES_5;
#elif defined(ADC_SAMPLETIME_12CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_12CYCLES_5;
#elif defined(ADC_SAMPLETIME_13CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_13CYCLES_5;
#elif defined(ADC_SAMPLETIME_15CYCLES)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_15CYCLES;
#elif defined(ADC_SAMPLETIME_16CYCLES)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_16CYCLES;
#elif defined(ADC_SAMPLETIME_19CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_19CYCLES_5;
#endif
#endif /* !ADC_SAMPLINGTIME */

/*
 * Minimum ADC sampling time is required when reading
 * internal channels so set it to max possible value.
 * It can be defined more precisely by defining:
 * ADC_SAMPLINGTIME_INTERNAL
 * to the desired ADC sample time.
 */
#ifndef ADC_SAMPLINGTIME_INTERNAL
#if defined(ADC_SAMPLETIME_480CYCLES)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_480CYCLES
#elif defined(ADC_SAMPLETIME_384CYCLES)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_384CYCLES
#elif defined(ADC_SAMPLETIME_810CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_810CYCLES_5
#elif defined(ADC_SAMPLETIME_640CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_640CYCLES_5
#elif defined(ADC_SAMPLETIME_601CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_601CYCLES_5
#elif defined(ADC_SAMPLETIME_247CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_247CYCLES_5
#elif defined(ADC_SAMPLETIME_239CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_239CYCLES_5
#elif defined(ADC_SAMPLETIME_160CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_160CYCLES_5
#else
#error "ADC sampling time could not be defined for internal channels!"
#endif
#endif /* !ADC_SAMPLINGTIME_INTERNAL */

#ifndef ADC_CLOCK_DIV
#ifdef ADC_CLOCK_SYNC_PCLK_DIV4
#define ADC_CLOCK_DIV ADC_CLOCK_SYNC_PCLK_DIV4
#elif ADC_CLOCK_SYNC_PCLK_DIV2
#define ADC_CLOCK_DIV ADC_CLOCK_SYNC_PCLK_DIV2
#elif defined(ADC_CLOCK_ASYNC_DIV1)
#define ADC_CLOCK_DIV ADC_CLOCK_ASYNC_DIV1
#endif
#endif /* !ADC_CLOCK_DIV */

#ifndef ADC_REGULAR_RANK_1
#define ADC_REGULAR_RANK_1 1
#endif

/* Private Functions */
static uint32_t get_adc_channel(PinName pin, uint32_t* bank)
{
    uint32_t function = pinmap_function(pin, PinMap_ADC);
    uint32_t channel  = 0;
    switch (STM_PIN_CHANNEL(function)) {
#ifdef ADC_CHANNEL_0
        case 0:
            channel = ADC_CHANNEL_0;
            break;
#endif
        case 1:
            channel = ADC_CHANNEL_1;
            break;
        case 2:
            channel = ADC_CHANNEL_2;
            break;
        case 3:
            channel = ADC_CHANNEL_3;
            break;
        case 4:
            channel = ADC_CHANNEL_4;
            break;
        case 5:
            channel = ADC_CHANNEL_5;
            break;
        case 6:
            channel = ADC_CHANNEL_6;
            break;
        case 7:
            channel = ADC_CHANNEL_7;
            break;
        case 8:
            channel = ADC_CHANNEL_8;
            break;
        case 9:
            channel = ADC_CHANNEL_9;
            break;
        case 10:
            channel = ADC_CHANNEL_10;
            break;
        case 11:
            channel = ADC_CHANNEL_11;
            break;
        case 12:
            channel = ADC_CHANNEL_12;
            break;
        case 13:
            channel = ADC_CHANNEL_13;
            break;
        case 14:
            channel = ADC_CHANNEL_14;
            break;
        case 15:
            channel = ADC_CHANNEL_15;
            break;
#ifdef ADC_CHANNEL_16
        case 16:
            channel = ADC_CHANNEL_16;
            break;
#endif
        case 17:
            channel = ADC_CHANNEL_17;
            break;
#ifdef ADC_CHANNEL_18
        case 18:
            channel = ADC_CHANNEL_18;
            break;
#endif
#ifdef ADC_CHANNEL_19
        case 19:
            channel = ADC_CHANNEL_19;
            break;
#endif
#ifdef ADC_CHANNEL_20
        case 20:
            channel = ADC_CHANNEL_20;
            break;
        case 21:
            channel = ADC_CHANNEL_21;
            break;
        case 22:
            channel = ADC_CHANNEL_22;
            break;
        case 23:
            channel = ADC_CHANNEL_23;
            break;
        case 24:
            channel = ADC_CHANNEL_24;
            break;
        case 25:
            channel = ADC_CHANNEL_25;
            break;
        case 26:
            channel = ADC_CHANNEL_26;
            break;
#ifdef ADC_CHANNEL_27
        case 27:
            channel = ADC_CHANNEL_27;
            break;
        case 28:
            channel = ADC_CHANNEL_28;
            break;
        case 29:
            channel = ADC_CHANNEL_29;
            break;
        case 30:
            channel = ADC_CHANNEL_30;
            break;
        case 31:
            channel = ADC_CHANNEL_31;
            break;
#endif
#endif
        default:
            channel = 0;
            break;
    }
#ifdef ADC_CHANNELS_BANK_B
    if (STM_PIN_ANALOG_CHANNEL_BANK_B(function)) {
        *bank = ADC_CHANNELS_BANK_B;
    }
    else {
        *bank = ADC_CHANNELS_BANK_A;
    }
#else
    UNUSED(bank);
#endif
    return channel;
}

static uint32_t get_adc_internal_channel(PinName pin)
{
    uint32_t channel = 0;
    switch (pin) {
#if defined(ADC_CHANNEL_TEMPSENSOR)
        case PADC_TEMP:
            channel = ADC_CHANNEL_TEMPSENSOR;
            break;
#endif
#if defined(ADC_CHANNEL_TEMPSENSOR_ADC1)
        case PADC_TEMP:
            channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
            break;
#endif
#if defined(ADC5) && defined(ADC_CHANNEL_TEMPSENSOR_ADC5)
        case PADC_TEMP_ADC5:
            channel = ADC_CHANNEL_TEMPSENSOR_ADC5;
            break;
#endif
#ifdef ADC_CHANNEL_VREFINT
        case PADC_VREF:
            channel = ADC_CHANNEL_VREFINT;
            break;
#endif
#ifdef ADC_CHANNEL_VBAT
        case PADC_VBAT:
            channel = ADC_CHANNEL_VBAT;
            break;
#endif
        default:
            channel = 0;
            break;
    }
    return channel;
}
#endif /* HAL_ADC_MODULE_ENABLED && !HAL_ADC_MODULE_ONLY */

#if defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY)
uint32_t get_pwm_channel(PinName pin)
{
    uint32_t function = pinmap_function(pin, PinMap_TIM);
    uint32_t channel  = 0;
    switch (STM_PIN_CHANNEL(function)) {
        case 1:
            channel = TIM_CHANNEL_1;
            break;
        case 2:
            channel = TIM_CHANNEL_2;
            break;
        case 3:
            channel = TIM_CHANNEL_3;
            break;
        case 4:
            channel = TIM_CHANNEL_4;
            break;
        default:
            channel = 0;
            break;
    }
    return channel;
}
#endif /* HAL_TIM_MODULE_ENABLED && !HAL_TIM_MODULE_ONLY */

#if defined(HAL_DAC_MODULE_ENABLED) && !defined(HAL_DAC_MODULE_ONLY)
static uint32_t get_dac_channel(PinName pin)
{
    uint32_t function = pinmap_function(pin, PinMap_DAC);
    uint32_t channel  = 0;
    switch (STM_PIN_CHANNEL(function)) {
#ifdef DAC_CHANNEL_0
        case 0:
            channel = DAC_CHANNEL_0;
            break;
#endif
        case 1:
            channel = DAC_CHANNEL_1;
            break;
#ifdef DAC_CHANNEL_2
        case 2:
            channel = DAC_CHANNEL_2;
            break;
#endif
        default:
            channel = 0;
            break;
    }
    return channel;
}

////////////////////////// DAC INTERFACE FUNCTIONS /////////////////////////////

/**
 * @brief DAC MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    /* DAC Periph clock enable */
    if (hdac->Instance == DAC1) {
#ifdef __HAL_RCC_DAC_CLK_ENABLE
        __HAL_RCC_DAC_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_DAC1_CLK_ENABLE
        __HAL_RCC_DAC1_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
        __HAL_RCC_DAC12_CLK_ENABLE();
#endif
    }
#ifdef DAC2
    else if (hdac->Instance == DAC2) {
#ifdef __HAL_RCC_DAC2_CLK_ENABLE
        __HAL_RCC_DAC2_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
        __HAL_RCC_DAC12_CLK_ENABLE();
#endif
    }
#endif
#ifdef DAC3
    else if (hdac->Instance == DAC3) {
#ifdef __HAL_RCC_DAC3_CLK_ENABLE
        __HAL_RCC_DAC3_CLK_ENABLE();
#endif
    }
#endif
#ifdef DAC4
    else if (hdac->Instance == DAC4) {
#ifdef __HAL_RCC_DAC4_CLK_ENABLE
        __HAL_RCC_DAC4_CLK_ENABLE();
#endif
    }
#endif

    /* Configure DAC GPIO pins */
    pinmap_pinout(g_current_pin, PinMap_DAC);
}

/**
 * @brief  This function will set the DAC to the required value
 * @param  port : the gpio port to use
 * @param  pin : the gpio pin to use
 * @param  value : the value to push on the adc output
 * @param  do_init : if set to 1 the initialization of the adc is done
 * @retval None
 */
void dac_write_value(PinName pin, uint32_t value, uint8_t do_init)
{
    DAC_HandleTypeDef DacHandle           = {};
    DAC_ChannelConfTypeDef dacChannelConf = {};
    uint32_t dacChannel;

    DacHandle.Instance = (DAC_TypeDef*)pinmap_peripheral(pin, PinMap_DAC);
    if (DacHandle.Instance == NP) {
        return;
    }
    dacChannel = get_dac_channel(pin);
#if defined(STM32G4xx)
    if (!IS_DAC_CHANNEL(DacHandle.Instance, dacChannel)) {
#else
    if (!IS_DAC_CHANNEL(dacChannel)) {
#endif
        return;
    }
    if (do_init == 1) {
        /*##-1- Configure the DAC peripheral #######################################*/
        g_current_pin = pin;
        if (HAL_DAC_Init(&DacHandle) != HAL_OK) {
            /* Initialization Error */
            return;
        }

        dacChannelConf.DAC_Trigger      = DAC_TRIGGER_NONE;
        dacChannelConf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
#if defined(DAC_OUTPUTSWITCH_ENABLE)
        dacChannelConf.DAC_OutputSwitch = DAC_OUTPUTSWITCH_ENABLE;
#endif
        /*##-2- Configure DAC channel1 #############################################*/
        if (HAL_DAC_ConfigChannel(&DacHandle, &dacChannelConf, dacChannel) != HAL_OK) {
            /* Channel configuration Error */
            return;
        }
    }

    /*##-3- Set DAC Channel1 DHR register ######################################*/
    if (HAL_DAC_SetValue(&DacHandle, dacChannel, DAC_ALIGN_12B_R, value) != HAL_OK) {
        /* Setting value Error */
        return;
    }

    /*##-4- Enable DAC Channel1 ################################################*/
    HAL_DAC_Start(&DacHandle, dacChannel);
}

/**
 * @brief  DeInitialize the DAC MSP.
 * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
    /* DAC Periph clock disable */
    if (hdac->Instance == DAC1) {
#ifdef __HAL_RCC_DAC_FORCE_RESET
        __HAL_RCC_DAC_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC1_FORCE_RESET
        __HAL_RCC_DAC1_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC12_FORCE_RESET
        __HAL_RCC_DAC12_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC_RELEASE_RESET
        __HAL_RCC_DAC_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC1_RELEASE_RESET
        __HAL_RCC_DAC1_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC12_RELEASE_RESET
        __HAL_RCC_DAC12_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC_CLK_DISABLE
        __HAL_RCC_DAC_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_DAC1_CLK_DISABLE
        __HAL_RCC_DAC1_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
        __HAL_RCC_DAC12_CLK_ENABLE();
#endif
    }
#ifdef DAC2
    else if (hdac->Instance == DAC2) {
#ifdef __HAL_RCC_DAC2_FORCE_RESET
        __HAL_RCC_DAC2_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC12_FORCE_RESET
        __HAL_RCC_DAC12_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC2_RELEASE_RESET
        __HAL_RCC_DAC2_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC12_RELEASE_RESET
        __HAL_RCC_DAC12_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC2_CLK_ENABLE
        __HAL_RCC_DAC2_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
        __HAL_RCC_DAC12_CLK_ENABLE();
#endif
    }
#endif
#ifdef DAC3
    else if (hdac->Instance == DAC3) {
#ifdef __HAL_RCC_DAC3_FORCE_RESET
        __HAL_RCC_DAC3_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC3_RELEASE_RESET
        __HAL_RCC_DAC3_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC3_CLK_DISABLE
        __HAL_RCC_DAC3_CLK_DISABLE();
#endif
    }
#endif
#ifdef DAC4
    else if (hdac->Instance == DAC4) {
#ifdef __HAL_RCC_DAC4_FORCE_RESET
        __HAL_RCC_DAC4_FORCE_RESET();
#endif
#ifdef __HAL_RCC_DAC4_RELEASE_RESET
        __HAL_RCC_DAC4_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_DAC4_CLK_DISABLE
        __HAL_RCC_DAC4_CLK_DISABLE();
#endif
    }
#endif
}

/**
 * @brief  This function will stop the DAC
 * @param  port : the gpio port to use
 * @param  pin : the gpio pin to use
 * @retval None
 */
void dac_stop(PinName pin)
{
    DAC_HandleTypeDef DacHandle;
    uint32_t dacChannel;

    DacHandle.Instance = (DAC_TypeDef*)pinmap_peripheral(pin, PinMap_DAC);
    if (DacHandle.Instance == NP) {
        return;
    }
    dacChannel = get_dac_channel(pin);
#if defined(STM32G4xx)
    if (!IS_DAC_CHANNEL(DacHandle.Instance, dacChannel)) {
#else
    if (!IS_DAC_CHANNEL(dacChannel)) {
#endif
        return;
    }

    HAL_DAC_Stop(&DacHandle, dacChannel);

    if (HAL_DAC_DeInit(&DacHandle) != HAL_OK) {
        /* DeInitialization Error */
        return;
    }
}
#endif  // HAL_DAC_MODULE_ENABLED && !HAL_DAC_MODULE_ONLY

#if defined(HAL_ADC_MODULE_ENABLED) && !defined(HAL_ADC_MODULE_ONLY)
////////////////////////// ADC INTERFACE FUNCTIONS /////////////////////////////

/**
 * @brief ADC MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* ADC Periph clock enable */
    if (hadc->Instance == ADC1) {
#ifdef __HAL_RCC_ADC1_CLK_ENABLE
        __HAL_RCC_ADC1_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC12_CLK_ENABLE
        __HAL_RCC_ADC12_CLK_ENABLE();
#endif
    }
#ifdef ADC2
    else if (hadc->Instance == ADC2) {
#ifdef __HAL_RCC_ADC2_CLK_ENABLE
        __HAL_RCC_ADC2_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC12_CLK_ENABLE
        __HAL_RCC_ADC12_CLK_ENABLE();
#endif
    }
#endif
#ifdef ADC3
    else if (hadc->Instance == ADC3) {
#ifdef __HAL_RCC_ADC3_CLK_ENABLE
        __HAL_RCC_ADC3_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC34_CLK_ENABLE
        __HAL_RCC_ADC34_CLK_ENABLE();
#endif
#if defined(ADC345_COMMON)
        __HAL_RCC_ADC345_CLK_ENABLE();
#endif
    }
#endif
#ifdef ADC4
    else if (hadc->Instance == ADC4) {
#ifdef __HAL_RCC_ADC34_CLK_ENABLE
        __HAL_RCC_ADC34_CLK_ENABLE();
#endif
#if defined(ADC345_COMMON)
        __HAL_RCC_ADC345_CLK_ENABLE();
#endif
    }
#endif
#ifdef ADC5
    else if (hadc->Instance == ADC5) {
#if defined(ADC345_COMMON)
        __HAL_RCC_ADC345_CLK_ENABLE();
#endif
    }
#endif
#ifdef __HAL_RCC_ADC_CLK_ENABLE
    __HAL_RCC_ADC_CLK_ENABLE();
#endif
    /* For STM32F1xx, STM32H7xx, and STM32MP1xx ADC prescaler is configured in
       SystemClock_Config (variant.cpp) */
#if defined(__HAL_RCC_ADC_CONFIG) && !defined(STM32F1xx) && !defined(STM32H7xx) && !defined(STM32MP1xx)
    hsem_lock(CFG_HW_RCC_CRRCR_CCIPR_SEMID, HSEM_LOCK_DEFAULT_RETRY);
    /* ADC Periph interface clock configuration */
    __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_SYSCLK);
    hsem_unlock(CFG_HW_RCC_CRRCR_CCIPR_SEMID);
#endif

    /* Configure ADC GPIO pin */
    if (!(g_current_pin & PADC_BASE)) {
        pinmap_pinout(g_current_pin, PinMap_ADC);
    }
}

/**
 * @brief  DeInitializes the ADC MSP.
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
#ifdef __HAL_RCC_ADC_FORCE_RESET
    __HAL_RCC_ADC_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC_RELEASE_RESET
    __HAL_RCC_ADC_RELEASE_RESET();
#endif

    if (hadc->Instance == ADC1) {
#ifdef __HAL_RCC_ADC1_FORCE_RESET
        __HAL_RCC_ADC1_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC1_RELEASE_RESET
        __HAL_RCC_ADC1_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_FORCE_RESET
        __HAL_RCC_ADC12_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_RELEASE_RESET
        __HAL_RCC_ADC12_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC1_CLK_DISABLE
        __HAL_RCC_ADC1_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC12_CLK_DISABLE
        __HAL_RCC_ADC12_CLK_DISABLE();
#endif
    }
#ifdef ADC2
    else if (hadc->Instance == ADC2) {
#ifdef __HAL_RCC_ADC2_FORCE_RESET
        __HAL_RCC_ADC2_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC2_RELEASE_RESET
        __HAL_RCC_ADC2_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_FORCE_RESET
        __HAL_RCC_ADC12_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_RELEASE_RESET
        __HAL_RCC_ADC12_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC2_CLK_DISABLE
        __HAL_RCC_ADC2_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC2_CLK_DISABLE
        __HAL_RCC_ADC2_CLK_DISABLE();
#endif
    }
#endif
#ifdef ADC3
    else if (hadc->Instance == ADC3) {
#ifdef __HAL_RCC_ADC3_FORCE_RESET
        __HAL_RCC_ADC3_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC3_RELEASE_RESET
        __HAL_RCC_ADC3_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_FORCE_RESET
        __HAL_RCC_ADC34_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_RELEASE_RESET
        __HAL_RCC_ADC34_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC3_CLK_DISABLE
        __HAL_RCC_ADC3_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC34_CLK_DISABLE
        __HAL_RCC_ADC34_CLK_DISABLE();
#endif
#if defined(ADC345_COMMON)
        __HAL_RCC_ADC345_FORCE_RESET();
        __HAL_RCC_ADC345_RELEASE_RESET();
        __HAL_RCC_ADC345_CLK_DISABLE();
#endif
    }
#endif
#ifdef ADC4
    else if (hadc->Instance == ADC4) {
#ifdef __HAL_RCC_ADC34_FORCE_RESET
        __HAL_RCC_ADC34_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_RELEASE_RESET
        __HAL_RCC_ADC34_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_CLK_DISABLE
        __HAL_RCC_ADC34_CLK_DISABLE();
#endif
#if defined(ADC345_COMMON)
        __HAL_RCC_ADC345_FORCE_RESET();
        __HAL_RCC_ADC345_RELEASE_RESET();
        __HAL_RCC_ADC345_CLK_DISABLE();
#endif
    }
#endif
#ifdef ADC5
    else if (hadc->Instance == ADC5) {
#if defined(ADC345_COMMON)
        __HAL_RCC_ADC345_FORCE_RESET();
        __HAL_RCC_ADC345_RELEASE_RESET();
        __HAL_RCC_ADC345_CLK_DISABLE();
#endif
    }
#endif
#ifdef __HAL_RCC_ADC_CLK_DISABLE
    __HAL_RCC_ADC_FORCE_RESET();
    __HAL_RCC_ADC_RELEASE_RESET();
    __HAL_RCC_ADC_CLK_DISABLE();
#endif
}

/**
 * @brief  This function will set the ADC to the required value
 * @param  pin : the pin to use
 * @param  resolution : resolution for converted data: 6/8/10/12/14/16
 * @retval the value of the adc
 */
uint16_t adc_read_value(PinName pin, uint32_t resolution)
{
    ADC_HandleTypeDef AdcHandle           = {};
    ADC_ChannelConfTypeDef AdcChannelConf = {};
    __IO uint16_t uhADCxConvertedValue    = 0;
    uint32_t samplingTime                 = ADC_SAMPLINGTIME;
    uint32_t channel                      = 0;
    uint32_t bank                         = 0;

    if ((pin & PADC_BASE) && (pin < ANA_START)) {
#if defined(STM32H7xx)
        AdcHandle.Instance = ADC3;
#else
        AdcHandle.Instance = ADC1;
#if defined(ADC5) && defined(ADC_CHANNEL_TEMPSENSOR_ADC5)
        if (pin == PADC_TEMP_ADC5) {
            AdcHandle.Instance = ADC5;
        }
#endif
#endif
        channel      = get_adc_internal_channel(pin);
        samplingTime = ADC_SAMPLINGTIME_INTERNAL;
    }
    else {
        AdcHandle.Instance = (ADC_TypeDef*)pinmap_peripheral(pin, PinMap_ADC);
        channel            = get_adc_channel(pin, &bank);
    }

    if (AdcHandle.Instance == NP) {
        return 0;
    }

#ifdef ADC_CLOCK_DIV
    AdcHandle.Init.ClockPrescaler = ADC_CLOCK_DIV; /* (A)synchronous clock mode, input ADC clock divided */
#endif
#ifdef ADC_RESOLUTION_12B
    switch (resolution) {
#ifdef ADC_RESOLUTION_6B
        case 6:
            AdcHandle.Init.Resolution = ADC_RESOLUTION_6B; /* resolution for converted data */
            break;
#endif
        case 8:
            AdcHandle.Init.Resolution = ADC_RESOLUTION_8B; /* resolution for converted data */
            break;
        case 10:
            AdcHandle.Init.Resolution = ADC_RESOLUTION_10B; /* resolution for converted data */
            break;
        case 12:
        default:
            AdcHandle.Init.Resolution = ADC_RESOLUTION_12B; /* resolution for converted data */
            break;
#ifdef ADC_RESOLUTION_14B
        case 14:
            AdcHandle.Init.Resolution = ADC_RESOLUTION_14B; /* resolution for converted data */
            break;
#endif
#ifdef ADC_RESOLUTION_16B
        case 16:
            AdcHandle.Init.Resolution = ADC_RESOLUTION_16B; /* resolution for converted data */
            break;
#endif
    }
#else
    UNUSED(resolution);
#endif
#ifdef ADC_DATAALIGN_RIGHT
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT; /* Right-alignment for converted data */
#endif
#ifdef ADC_SCAN_SEQ_FIXED
    AdcHandle.Init.ScanConvMode =
        ADC_SCAN_SEQ_FIXED; /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
#else
    AdcHandle.Init.ScanConvMode =
        DISABLE; /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
#endif
#ifdef ADC_EOC_SINGLE_CONV
    AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV; /* EOC flag picked-up to indicate conversion end */
#endif
#if !defined(STM32F1xx) && !defined(STM32F2xx) && !defined(STM32F4xx) && !defined(STM32F7xx) && \
    !defined(STM32F373xC) && !defined(STM32F378xx)
    AdcHandle.Init.LowPowerAutoWait = DISABLE; /* Auto-delayed conversion feature disabled */
#endif
#if !defined(STM32F1xx) && !defined(STM32F2xx) && !defined(STM32F3xx) && !defined(STM32F4xx) && !defined(STM32F7xx) && \
    !defined(STM32G4xx) && !defined(STM32H7xx) && !defined(STM32L4xx) && !defined(STM32L5xx) &&                        \
    !defined(STM32MP1xx) && !defined(STM32WBxx)
    AdcHandle.Init.LowPowerAutoPowerOff = DISABLE; /* ADC automatically powers-off after a conversion and automatically
                                                      wakes-up when a new conversion is triggered */
#endif
#ifdef ADC_CHANNELS_BANK_B
    AdcHandle.Init.ChannelsBank = bank;
#elif defined(ADC_CHANNELS_BANK_A)
    AdcHandle.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
#endif
    AdcHandle.Init.ContinuousConvMode =
        DISABLE; /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#if !defined(STM32F0xx) && !defined(STM32L0xx)
    AdcHandle.Init.NbrOfConversion =
        1; /* Specifies the number of ranks that will be converted within the regular group sequencer. */
#endif
    AdcHandle.Init.DiscontinuousConvMode = DISABLE; /* Parameter discarded because sequencer is disabled */
#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
    AdcHandle.Init.NbrOfDiscConversion = 0; /* Parameter discarded because sequencer is disabled */
#endif
    AdcHandle.Init.ExternalTrigConv =
        ADC_SOFTWARE_START; /* Software start to trig the 1st conversion manually, without external event */
#if !defined(STM32F1xx) && !defined(STM32F373xC) && !defined(STM32F378xx)
    AdcHandle.Init.ExternalTrigConvEdge =
        ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
#endif
#if !defined(STM32F1xx) && !defined(STM32H7xx) && !defined(STM32MP1xx) && !defined(STM32F373xC) && !defined(STM32F378xx)
    AdcHandle.Init.DMAContinuousRequests = DISABLE; /* DMA one-shot mode selected (not applied to this example) */
#endif
#ifdef ADC_CONVERSIONDATA_DR
    AdcHandle.Init.ConversionDataManagement =
        ADC_CONVERSIONDATA_DR; /* Regular Conversion data stored in DR register only */
#endif
#ifdef ADC_OVR_DATA_OVERWRITTEN
    AdcHandle.Init.Overrun =
        ADC_OVR_DATA_OVERWRITTEN; /* DR register is overwritten with the last conversion result in case of overrun */
#endif
#ifdef ADC_LEFTBITSHIFT_NONE
    AdcHandle.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE; /* No bit shift left applied on the final ADC convesion data */
#endif

#if defined(STM32F0xx)
    AdcHandle.Init.SamplingTimeCommon = samplingTime;
#endif
#if defined(STM32G0xx)
    AdcHandle.Init.SamplingTimeCommon1 = samplingTime; /* Set sampling time common to a group of channels. */
    AdcHandle.Init.SamplingTimeCommon2 =
        samplingTime; /* Set sampling time common to a group of channels, second common setting possible.*/
#endif
#if defined(STM32L0xx)
    AdcHandle.Init.LowPowerFrequencyMode = DISABLE; /* To be enabled only if ADC clock < 2.8 MHz */
    AdcHandle.Init.SamplingTime          = samplingTime;
#endif
#if !defined(STM32F0xx) && !defined(STM32F1xx) && !defined(STM32F2xx) && !defined(STM32F3xx) && !defined(STM32F4xx) && \
    !defined(STM32F7xx) && !defined(STM32L1xx)
    AdcHandle.Init.OversamplingMode = DISABLE;
    /* AdcHandle.Init.Oversample ignore for STM32L0xx as oversampling disabled */
    /* AdcHandle.Init.Oversampling ignored for other as oversampling disabled */
#endif
#if defined(ADC_CFGR_DFSDMCFG) && defined(DFSDM1_Channel0)
    AdcHandle.Init.DFSDMConfig = ADC_DFSDM_MODE_DISABLE; /* ADC conversions are not transferred by DFSDM. */
#endif
#ifdef ADC_TRIGGER_FREQ_HIGH
    AdcHandle.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
#endif

    AdcHandle.State      = HAL_ADC_STATE_RESET;
    AdcHandle.DMA_Handle = NULL;
    AdcHandle.Lock       = HAL_UNLOCKED;
    /* Some other ADC_HandleTypeDef fields exists but not required */

    g_current_pin = pin; /* Needed for HAL_ADC_MspInit*/

    if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
        return 0;
    }

    AdcChannelConf.Channel = channel; /* Specifies the channel to configure into ADC */

#if defined(STM32L4xx) || defined(STM32L5xx) || defined(STM32WBxx)
    if (!IS_ADC_CHANNEL(&AdcHandle, AdcChannelConf.Channel)) {
#elif defined(STM32G4xx)
    if (!IS_ADC_CHANNEL(&AdcHandle, AdcChannelConf.Channel)) {
#else
    if (!IS_ADC_CHANNEL(AdcChannelConf.Channel)) {
#endif /* STM32L4xx || STM32WBxx */
        return 0;
    }
#ifdef ADC_SCAN_SEQ_FIXED
    AdcChannelConf.Rank =
        ADC_RANK_CHANNEL_NUMBER; /* Enable the rank of the selected channels when not fully configurable */
#else
    AdcChannelConf.Rank = ADC_REGULAR_RANK_1; /* Specifies the rank in the regular group sequencer */
#endif
#if !defined(STM32L0xx)
#if !defined(STM32G0xx)
    AdcChannelConf.SamplingTime = samplingTime; /* Sampling time value to be set for the selected channel */
#else
    AdcChannelConf.SamplingTime =
        ADC_SAMPLINGTIME_COMMON_1; /* Sampling time value to be set for the selected channel */
#endif
#endif
#if !defined(STM32F0xx) && !defined(STM32F1xx) && !defined(STM32F2xx) && !defined(STM32F4xx) && !defined(STM32F7xx) && \
    !defined(STM32G0xx) && !defined(STM32L0xx) && !defined(STM32L1xx) && !defined(STM32F373xC) &&                      \
    !defined(STM32F378xx)
    AdcChannelConf.SingleDiff   = ADC_SINGLE_ENDED; /* Single-ended input channel */
    AdcChannelConf.OffsetNumber = ADC_OFFSET_NONE;  /* No offset subtraction */
#endif
#if !defined(STM32F0xx) && !defined(STM32F1xx) && !defined(STM32F2xx) && !defined(STM32G0xx) && !defined(STM32L0xx) && \
    !defined(STM32L1xx) && !defined(STM32WBxx) && !defined(STM32F373xC) && !defined(STM32F378xx)
    AdcChannelConf.Offset = 0; /* Parameter discarded because offset correction is disabled */
#endif
#if defined(STM32H7xx) || defined(STM32MP1xx)
    AdcChannelConf.OffsetRightShift       = DISABLE; /* No Right Offset Shift */
    AdcChannelConf.OffsetSignedSaturation = DISABLE; /* Signed saturation feature is not used */
#endif

    /*##-2- Configure ADC regular channel ######################################*/
    if (HAL_ADC_ConfigChannel(&AdcHandle, &AdcChannelConf) != HAL_OK) {
        /* Channel Configuration Error */
        return 0;
    }

#if defined(STM32F0xx) || defined(STM32F1xx) || defined(STM32F3xx) || defined(STM32G0xx) || defined(STM32G4xx) || \
    defined(STM32H7xx) || defined(STM32L0xx) || defined(STM32L4xx) || defined(STM32MP1xx) || defined(STM32WBxx)
    /*##-2.1- Calibrate ADC then Start the conversion process ####################*/
#if defined(STM32F0xx) || defined(STM32G0xx) || defined(STM32F1xx) || defined(STM32F373xC) || defined(STM32F378xx)
    if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
#elif defined(STM32H7xx) || defined(STM32MP1xx)
    if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
#else
    if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
#endif
    {
        /* ADC Calibration Error */
        return 0;
    }
#endif

    /*##-3- Start the conversion process ####################*/
    if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
        /* Start Conversation Error */
        return 0;
    }

    /*##-4- Wait for the end of conversion #####################################*/
    /*  For simplicity reasons, this example is just waiting till the end of the
        conversion, but application may perform other tasks while conversion
        operation is ongoing. */
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK) {
        /* End Of Conversion flag not set on time */
        return 0;
    }

    /* Check if the continous conversion of regular channel is finished */
    if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC) {
        /*##-5- Get the converted value of regular channel  ########################*/
        uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
    }

    if (HAL_ADC_Stop(&AdcHandle) != HAL_OK) {
        /* Stop Conversation Error */
        return 0;
    }

    if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK) {
        return 0;
    }

    if (__LL_ADC_COMMON_INSTANCE(AdcHandle.Instance) != 0U) {
        LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(AdcHandle.Instance), LL_ADC_PATH_INTERNAL_NONE);
    }
    return uhADCxConvertedValue;
}
#endif /* HAL_ADC_MODULE_ENABLED && !HAL_ADC_MODULE_ONLY*/

#if defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY)

////////////////////////// PWM INTERFACE FUNCTIONS /////////////////////////////

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
    // Enable TIM clock
#if defined(TIM1_BASE)
    if (htim_pwm->Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
#endif
#if defined(TIM2_BASE)
    if (htim_pwm->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
#endif
#if defined(TIM3_BASE)
    if (htim_pwm->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
#endif
#if defined(TIM4_BASE)
    if (htim_pwm->Instance == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
#endif
#if defined(TIM5_BASE)
    if (htim_pwm->Instance == TIM5) {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
#endif
#if defined(TIM6_BASE)
    if (htim_pwm->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
#endif
#if defined(TIM7_BASE)
    if (htim_pwm->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
    }
#endif
#if defined(TIM8_BASE)
    if (htim_pwm->Instance == TIM8) {
        __HAL_RCC_TIM8_CLK_ENABLE();
    }
#endif
#if defined(TIM9_BASE)
    if (htim_pwm->Instance == TIM9) {
        __HAL_RCC_TIM9_CLK_ENABLE();
    }
#endif
#if defined(TIM10_BASE)
    if (htim_pwm->Instance == TIM10) {
        __HAL_RCC_TIM10_CLK_ENABLE();
    }
#endif
#if defined(TIM11_BASE)
    if (htim_pwm->Instance == TIM11) {
        __HAL_RCC_TIM11_CLK_ENABLE();
    }
#endif
#if defined(TIM12_BASE)
    if (htim_pwm->Instance == TIM12) {
        __HAL_RCC_TIM12_CLK_ENABLE();
    }
#endif
#if defined(TIM13_BASE)
    if (htim_pwm->Instance == TIM13) {
        __HAL_RCC_TIM13_CLK_ENABLE();
    }
#endif
#if defined(TIM14_BASE)
    if (htim_pwm->Instance == TIM14) {
        __HAL_RCC_TIM14_CLK_ENABLE();
    }
#endif
#if defined(TIM15_BASE)
    if (htim_pwm->Instance == TIM15) {
        __HAL_RCC_TIM15_CLK_ENABLE();
    }
#endif
#if defined(TIM16_BASE)
    if (htim_pwm->Instance == TIM16) {
        __HAL_RCC_TIM16_CLK_ENABLE();
    }
#endif
#if defined(TIM17_BASE)
    if (htim_pwm->Instance == TIM17) {
        __HAL_RCC_TIM17_CLK_ENABLE();
    }
#endif
#if defined(TIM18_BASE)
    if (htim_pwm->Instance == TIM18) {
        __HAL_RCC_TIM18_CLK_ENABLE();
    }
#endif
#if defined(TIM19_BASE)
    if (htim_pwm->Instance == TIM19) {
        __HAL_RCC_TIM19_CLK_ENABLE();
    }
#endif
#if defined(TIM20_BASE)
    if (htim_pwm->Instance == TIM20) {
        __HAL_RCC_TIM20_CLK_ENABLE();
    }
#endif
#if defined(TIM21_BASE)
    if (htim_pwm->Instance == TIM21) {
        __HAL_RCC_TIM21_CLK_ENABLE();
    }
#endif
#if defined(TIM22_BASE)
    if (htim_pwm->Instance == TIM22) {
        __HAL_RCC_TIM22_CLK_ENABLE();
    }
#endif
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
    // Enable TIM clock
#if defined(TIM1_BASE)
    if (htim_pwm->Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_DISABLE();
    }
#endif
#if defined(TIM2_BASE)
    if (htim_pwm->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
#endif
#if defined(TIM3_BASE)
    if (htim_pwm->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_DISABLE();
    }
#endif
#if defined(TIM4_BASE)
    if (htim_pwm->Instance == TIM4) {
        __HAL_RCC_TIM4_CLK_DISABLE();
    }
#endif
#if defined(TIM5_BASE)
    if (htim_pwm->Instance == TIM5) {
        __HAL_RCC_TIM5_CLK_DISABLE();
    }
#endif
#if defined(TIM6_BASE)
    if (htim_pwm->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_DISABLE();
    }
#endif
#if defined(TIM7_BASE)
    if (htim_pwm->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_DISABLE();
    }
#endif
#if defined(TIM8_BASE)
    if (htim_pwm->Instance == TIM8) {
        __HAL_RCC_TIM8_CLK_DISABLE();
    }
#endif
#if defined(TIM9_BASE)
    if (htim_pwm->Instance == TIM9) {
        __HAL_RCC_TIM9_CLK_DISABLE();
    }
#endif
#if defined(TIM10_BASE)
    if (htim_pwm->Instance == TIM10) {
        __HAL_RCC_TIM10_CLK_DISABLE();
    }
#endif
#if defined(TIM11_BASE)
    if (htim_pwm->Instance == TIM11) {
        __HAL_RCC_TIM11_CLK_DISABLE();
    }
#endif
#if defined(TIM12_BASE)
    if (htim_pwm->Instance == TIM12) {
        __HAL_RCC_TIM12_CLK_DISABLE();
    }
#endif
#if defined(TIM13_BASE)
    if (htim_pwm->Instance == TIM13) {
        __HAL_RCC_TIM13_CLK_DISABLE();
    }
#endif
#if defined(TIM14_BASE)
    if (htim_pwm->Instance == TIM14) {
        __HAL_RCC_TIM14_CLK_DISABLE();
    }
#endif
#if defined(TIM15_BASE)
    if (htim_pwm->Instance == TIM15) {
        __HAL_RCC_TIM15_CLK_DISABLE();
    }
#endif
#if defined(TIM16_BASE)
    if (htim_pwm->Instance == TIM16) {
        __HAL_RCC_TIM16_CLK_DISABLE();
    }
#endif
#if defined(TIM17_BASE)
    if (htim_pwm->Instance == TIM17) {
        __HAL_RCC_TIM17_CLK_DISABLE();
    }
#endif
#if defined(TIM18_BASE)
    if (htim_pwm->Instance == TIM18) {
        __HAL_RCC_TIM18_CLK_DISABLE();
    }
#endif
#if defined(TIM19_BASE)
    if (htim_pwm->Instance == TIM19) {
        __HAL_RCC_TIM19_CLK_DISABLE();
    }
#endif
#if defined(TIM20_BASE)
    if (htim_pwm->Instance == TIM20) {
        __HAL_RCC_TIM20_CLK_DISABLE();
    }
#endif
#if defined(TIM21_BASE)
    if (htim_pwm->Instance == TIM21) {
        __HAL_RCC_TIM21_CLK_DISABLE();
    }
#endif
#if defined(TIM22_BASE)
    if (htim_pwm->Instance == TIM22) {
        __HAL_RCC_TIM22_CLK_DISABLE();
    }
#endif
}

/**
 * @brief  This function will set the PWM to the required value
 * @param  port : the gpio port to use
 * @param  pin : the gpio pin to use
 * @param  clock_freq : frequency of the tim clock
 * @param  value : the value to push on the PWM output
 * @retval None
 */
void pwm_start(PinName pin, uint32_t PWM_freq, uint32_t value, TimerCompareFormat_t resolution) {}

/**
 * @brief  This function will disable the PWM
 * @param  port : the gpio port to use
 * @param  pin : the gpio pin to use
 * @retval None
 */
void pwm_stop(PinName pin) {}
#endif /* HAL_TIM_MODULE_ENABLED && !HAL_TIM_MODULE_ONLY */

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
