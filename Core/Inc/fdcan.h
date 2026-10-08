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




const uint32_t DTI_NODE_ID = 0xFF; // Broadcast address for all devices
const uint32_t MAIN_NODE_ID = 0x01; // Address for the DTI controller
const uint32_t SECONDARY_NODE_ID = 0x02; // Packet ID for DTI commands


const uint32_t ERPM_DUTY_IN_V = 0;
const uint32_t AC_DC_CURRENT = 1;
const uint32_t CONTROL_MOTOR_TEMP_FAULT_CODE = 2;
const uint32_t ID_IQ_VALUES = 3;
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
const uint32_t PEDALS_SENSOR_PACKET_ID = 0;


typedef struct {
    uint32_t packet_id;  // 1. Identifiant de la commande/donnée
    uint8_t  node_id;    // 2. Adresse matérielle de l'émetteur
    uint8_t  data[8];    // 3. Les données utiles du message
    uint8_t  dlc_length; // Longueur réelle des données reçues
} Frame_data_t;


typedef struct {
    // Packet 0x00
    int16_t input_voltage;    
    int16_t duty_cycle;     
    int32_t erpm;
    
    // Packet 0x01
    int16_t ac_current;       // Courant AC (A)
    int16_t dc_current;       // Courant DC (A)
    
    // Packet 0x02
    uint8_t fault_code;       // Code d'erreur (0x00 = OK)
    int16_t motor_temp;       // Température Moteur (°C)
    int16_t controller_temp;  // Température Contrôleur (°C)
    
    // Packet 0x03
    int32_t id_current;       // Courant Id FOC (A)
    int32_t iq_current;       // Courant Iq FOC (A)
} DTI_Telemetry_t;

typedef struct {
    uint16_t apps0;          // Pedal position sensor 0 (0-4095)
    uint16_t apps1;          // Pedal position sensor 1 (0-409
    uint16_t brake;          // Brake position sensor (0-4095)
} valSensor_t;
//  Functions prototypes

HAL_StatusTypeDef DTI_Send_Generic_Frame(uint32_t packet_id, uint8_t node_id, uint8_t *pData, uint8_t len);

HAL_StatusTypeDef sendData0(uint32_t packet_id, uint8_t node_id);
HAL_StatusTypeDef sendData8(uint32_t packet_id, uint8_t node_id, uint8_t data);
HAL_StatusTypeDef sendData16(uint32_t packet_id, uint8_t node_id, uint16_t data);
HAL_StatusTypeDef sendData32(uint32_t packet_id, uint8_t node_id, uint32_t data);
HAL_StatusTypeDef sendData64(uint32_t packet_id, uint8_t node_id, uint64_t data);




#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H */
