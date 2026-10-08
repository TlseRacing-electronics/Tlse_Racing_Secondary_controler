/*
All functions for CAN communication

*/

#include "../Inc/fdcan.h"

extern FDCAN_HandleTypeDef hfdcan1; // Adjust according to your CubeMX configuration

/**
 * @brief  Sends a generic CAN frame to the DTI controller
 * @param  packet_id : Command identifier (e.g. 0x01 for Set Current)
 * @param  node_id   : Device address (e.g. 0xFF to send to all devices)
 * @param  pData     : Pointer to the byte array to send
 * @param  len       : Number of bytes to send (typically 4 for DTI commands)
 * @retval HAL_StatusTypeDef : HAL_OK if the frame is successfully added to the FIFO
 */
HAL_StatusTypeDef DTI_Send_Generic_Frame(uint32_t packet_id, uint8_t node_id, uint8_t *pData, uint8_t len)
{
    FDCAN_TxHeaderTypeDef TxHeader;

    // The extended ID is built by shifting the packet ID by 8 bits and adding the node ID
    TxHeader.Identifier = (packet_id << 8) | node_id;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;

    // Convert the byte length to the matching FDCAN_DLC constant
    switch (len)
    {
    case 0:
        TxHeader.DataLength = FDCAN_DLC_BYTES_0;
        break;
    case 1:
        TxHeader.DataLength = FDCAN_DLC_BYTES_1;
        break;
    case 2:
        TxHeader.DataLength = FDCAN_DLC_BYTES_2;
        break;
    case 4:
        TxHeader.DataLength = FDCAN_DLC_BYTES_4;
        break;
    case 8:
        TxHeader.DataLength = FDCAN_DLC_BYTES_8;
        break;
    default:
        return HAL_ERROR; // Length not supported
    }

    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN; // The DTI controller uses classic CAN 2.0B
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    // Transmit the message
    return HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, pData);
}

/**
 * @brief  Sends a CAN frame with no data to the DTI controller
 * @param  packet_id : Command identifier (e.g. 0x01 for Set Current)
 * @param  node_id   : Device address (e.g. 0xFF to send to all devices)
 * @retval HAL_StatusTypeDef : HAL_OK if the frame is successfully added to the FIFO
 * @note do not use this function not sure if it will work
 */
HAL_StatusTypeDef sendData0(uint32_t packet_id, uint8_t node_id)
{
    uint8_t data_array[0];

    return DTI_Send_Generic_Frame(packet_id, node_id, data_array, 0);
}

/**
 * @brief  Sends a CAN frame with a single byte of data to the DTI controller
 * @param  packet_id : Command identifier (e.g. 0x01 for Set Current)
 * @param  node_id   : Device address (e.g. 0xFF tosend to all devices)
 * @param  data      : The single byte of data to send
 * @retval HAL_StatusTypeDef : HAL_OK if the frame is successfully added to the FIFO
 */
HAL_StatusTypeDef sendData8(uint32_t packet_id, uint8_t node_id, uint8_t data)
{
    uint8_t data_array[1] = {data};

    return DTI_Send_Generic_Frame(packet_id, node_id, data_array, 1);
}
// size of 2
HAL_StatusTypeDef sendData16(uint32_t packet_id, uint8_t node_id, uint16_t data)
{
    uint8_t data_array[2];
    data_array[0] = (data >> 8) & 0xFF;
    data_array[1] = data & 0xFF;
    return DTI_Send_Generic_Frame(packet_id, node_id, data_array, 2);
}
// size of 4
HAL_StatusTypeDef sendData32(uint32_t packet_id, uint8_t node_id, uint32_t data)
{
    uint8_t data_array[4];
    data_array[0] = (data >> 24) & 0xFF;
    data_array[1] = (data >> 16) & 0xFF;
    data_array[2] = (data >> 8) & 0xFF;
    data_array[3] = data & 0xFF;
    return DTI_Send_Generic_Frame(packet_id, node_id, data_array, 4);
}
// size of 8
HAL_StatusTypeDef sendData64(uint32_t packet_id, uint8_t node_id, uint64_t data)
{
    uint8_t data_array[8];
    data_array[0] = (data >> 56) & 0xFF;
    data_array[1] = (data >> 48) & 0xFF;
    data_array[2] = (data >> 40) & 0xFF;
    data_array[3] = (data >> 32) & 0xFF;
    data_array[4] = (data >> 24) & 0xFF;
    data_array[5] = (data >> 16) & 0xFF;
    data_array[6] = (data >> 8) & 0xFF;
    data_array[7] = data & 0xFF;
    return DTI_Send_Generic_Frame(packet_id, node_id, data_array, 8);
}

HAL_StatusTypeDef DTI_Receive_And_Split(Frame_data_t *frame)
{
    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    // Récupération du message depuis la FIFO 0 matérielle
    if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
        // Le contrôleur DTI utilise exclusivement des identifiants étendus (29 bits)[cite: 39]
        if (RxHeader.IdType == FDCAN_EXTENDED_ID)
        {
            frame->packet_id = (RxHeader.Identifier >> 8) & 0x1FFFFF;
            frame->node_id = RxHeader.Identifier & 0xFF;
            frame->dlc_length = (RxHeader.DataLength >> 16) & 0x0F;
            if (frame->dlc_length > 8)
                frame->dlc_length = 8;

            // PARTIE 3 : Récupération du tableau de Données[cite: 39]
            for (uint8_t i = 0; i < 8; i++)
            {
                if (i < frame->dlc_length)
                {
                    frame->data[i] = RxData[i];
                }
                else
                {
                    frame->data[i] = 0x00; // Remplissage avec des zéros si trame courte
                }
            }
        }
    }

    return HAL_ERROR; // Aucun message ou format incompatible
}

void getData(Frame_data_t *frame, DTI_Telemetry_t *telemetry, valSensor_t *sensors)
{
    switch (frame->node_id)
    {
    case DTI_NODE_ID: // DTI Controller
        switch (frame->packet_id)
        {
        case ERPM_DUTY_IN_V:
            telemetry->input_voltage = (frame->data[0] << 8) | frame->data[1];
            telemetry->duty_cycle = (frame->data[2] << 8) | frame->data[3];
            telemetry->erpm = (frame->data[4] << 24) | (frame->data[5] << 16) | (frame->data[6] << 8) | frame->data[7];
            break;
        case AC_DC_CURRENT:
            telemetry->ac_current = (frame->data[0] << 8) | frame->data[1];
            telemetry->dc_current = (frame->data[2] << 8) | frame->data[3];
            break;
        case CONTROL_MOTOR_TEMP_FAULT_CODE:
            telemetry->fault_code = frame->data[0];
            telemetry->motor_temp = (frame->data[1] << 8) | frame->data[2];
            telemetry->controller_temp = (frame->data[3] << 8) | frame->data[4];
            break;
        case ID_IQ_VALUES:
            telemetry->id_current = (frame->data[0] << 24) | (frame->data[1] << 16) | (frame->data[2] << 8) | frame->data[3];
            telemetry->iq_current = (frame->data[4] << 24) | (frame->data[5] << 16) | (frame->data[6] << 8) | frame->data[7];
            break;
        default:
            break;
        }
        break;
    case SECONDARY_NODE_ID: // Pedals Sensor
        sensors->apps0 = (frame->data[0] << 8) | frame->data[1];
        sensors->apps1 = (frame->data[2] << 8) | frame->data[3];
        sensors->brake = (frame->data[4] << 8) | frame->data[5];
        break;
    }
}
