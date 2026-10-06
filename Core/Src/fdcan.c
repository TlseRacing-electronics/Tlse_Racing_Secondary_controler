/*
All functions for CAN communication 

*/

#include "../Inc/fdcan.h"

extern FDCAN_HandleTypeDef hfdcan1; // À adapter selon votre configuration CubeMX

void FDCAN_Init(FDCAN_HandleTypeDef *hfdcan1)
{

}




/**
  * @brief  Envoie une trame CAN générique au contrôleur DTI
  * @param  packet_id : Identifiant de la commande (ex: 0x01 pour Set Current)
  * @param  node_id   : Adresse de l'appareil (ex: 0xFF pour envoyer à tous)
  * @param  pData     : Pointeur vers le tableau d'octets à envoyer
  * @param  len       : Nombre d'octets à envoyer (typiquement 4 pour les commandes DTI)
  * @retval HAL_StatusTypeDef : HAL_OK si l'ajout dans la FIFO réussit
*/
HAL_StatusTypeDef DTI_Send_Generic_Frame(uint32_t packet_id, uint8_t node_id, uint8_t *pData, uint8_t len)
{
    FDCAN_TxHeaderTypeDef TxHeader;

    // L'ID étendu est construit en décalant le Packet ID de 8 bits et en ajoutant le Node ID[cite: 36, 39]
    TxHeader.Identifier = (packet_id << 8) | node_id;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    
    // Conversion de la longueur en entier vers la constante FDCAN_DLC correspondante
    switch(len)
    {
        case 0: TxHeader.DataLength = FDCAN_DLC_BYTES_0; break;
        case 1: TxHeader.DataLength = FDCAN_DLC_BYTES_1; break;
        case 2: TxHeader.DataLength = FDCAN_DLC_BYTES_2; break;
        case 3: TxHeader.DataLength = FDCAN_DLC_BYTES_3; break;
        case 4: TxHeader.DataLength = FDCAN_DLC_BYTES_4; break;
        case 5: TxHeader.DataLength = FDCAN_DLC_BYTES_5; break;
        case 6: TxHeader.DataLength = FDCAN_DLC_BYTES_6; break;
        case 7: TxHeader.DataLength = FDCAN_DLC_BYTES_7; break;
        case 8: TxHeader.DataLength = FDCAN_DLC_BYTES_8; break;
        default: return HAL_ERROR; // Longueur non supportée en CAN classique
    }

    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN; // Le contrôleur DTI utilise le CAN 2.0B classique
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    // Transmission du message
    return HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, pData);
}



HAL_StatusTypeDef sendData(uint32_t packet_id, uint8_t node_id, uint32_t data, uint8_t len)
{
    len = 4;
    uint8_t data_array[4];
    data_array[0] = (data >> 24) & 0xFF;
    data_array[1] = (data >> 16) & 0xFF;
    data_array[2] = (data >> 8) & 0xFF;
    data_array[3] = data & 0xFF;
    return DTI_Send_Generic_Frame(packet_id, node_id, data_array, len);
}