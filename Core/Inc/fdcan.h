/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : fdcan.h
  * @brief          : Header for fdcan.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  
  
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_H
#define __FDCAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"








// Constants for DTI commands and data packets
// --- data received from DTI ---
const uint32_t ERPM_DUTY_IN_V = 0;
const uint32_t AC_DC_CURRENT = 1;
const uint32_t CONTROL_MOTOR_TEMP_FAULT_CODE = 2;
const uint32_t ID_IQ_VALUES = 3;
// --- data sent to DTI ---
const uint32_t SET_DUTY = 0;
const uint32_t SET_CURRENT = 1;
const uint32_t SET_BRAKE_CURRENT = 2;
const uint32_t SET_RPM = 3;
const uint32_t SET_POSITION = 4;
const uint32_t SET_RELATIVE_CURRENT = 10;
const uint32_t SET_RELATIVE_BRAKE_CURRENT = 11;
const uint32_t SET_MAX_AC_CURRENT = 12;
const uint32_t SET_MAX_AC_BRAKE_CURRENT = 13;
const uint32_t SET_MAX_DC_CURRENT = 14;
const uint32_t SET_MAX_DC_BRAKE_CURRENT = 15;

// constants for node IDs not for DTI commands
const uint32_t APPS_NODE_ID = 1;
const uint32_t BRAKE_NODE_ID = 2;


#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H */
