#ifndef __SENSOR_H
#define __SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
uint16_t read_ADC(ADC_HandleTypeDef *hadc2)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = 1;
    sConfig.Rank = ADC_REGULAR_RANK_1;



    uint16_t adcValue = HAL_ADC_GetValue(hadc2);
    HAL_ADC_PollForConversion(hadc2, HAL_MAX_DELAY);


    return adcValue;
}











#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_H */
