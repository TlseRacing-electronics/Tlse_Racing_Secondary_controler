#include "../Inc/sensor.h"
int capt1;
void init_ADC(ADC_HandleTypeDef *hadc1, ADC_HandleTypeDef *hadc2, ADC_HandleTypeDef *hadc3)
{
    HAL_ADC_Start(hadc1);
    HAL_ADC_PollForConversion(hadc1, HAL_MAX_DELAY);

    HAL_ADC_Start(hadc2);
    HAL_ADC_PollForConversion(hadc2, HAL_MAX_DELAY);

    HAL_ADC_Start(hadc3);
    HAL_ADC_PollForConversion(hadc3, HAL_MAX_DELAY);
}



uint16_t read_ADC(ADC_HandleTypeDef *hadc)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = 1;
    sConfig.Rank = ADC_REGULAR_RANK_1;



    uint16_t adcValue = HAL_ADC_GetValue(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);


    return adcValue;
}

uint16_t read_ADC(ADC_HandleTypeDef *hadc2)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = 1;
    sConfig.Rank = ADC_REGULAR_RANK_1;



    uint16_t adcValue = HAL_ADC_GetValue(hadc2);
    HAL_ADC_PollForConversion(hadc2, HAL_MAX_DELAY);


    return adcValue;
}



uint16_t read_ADC(ADC_HandleTypeDef *hadc3)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = 1;
    sConfig.Rank = ADC_REGULAR_RANK_1;



    uint16_t adcValue = HAL_ADC_GetValue(hadc3);
    HAL_ADC_PollForConversion(hadc3, HAL_MAX_DELAY);


    return adcValue;
}


//3,3 to 4096
//12 bit -->32bit divided by 1000 =12 bit value *float(1048.576)

int16_t errorshutdown(ADC_HandleTypeDef *hadc1, ADC_HandleTypeDef *hadc2)
{
    if (hadc1>=3900 && hadc2<=3900)
    {
        capt1 = 1;
    }else
    {
        capt1 = 0;
    }
}


uint32_t motorvalue(uint16_t captmoteur,ADC_HandleTypeDef *hadc1)
{
    hadc1 = captmoteur;
    uint32_t ret= (captmoteur *(1048.576));
    return ret;
}

