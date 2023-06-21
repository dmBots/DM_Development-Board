#include "bsp_dm4310.h"
#include "stm32f4xx_hal.h"

Motor_Inf mtr;
static CAN_TxHeaderTypeDef   Tx_Header;
uint8_t dm43_can_send_data[8];
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


int float_to_uint(float x, float x_min, float x_max, int bits)
{
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}
    
    
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}






void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
    if(rx_header.StdId == Master_ID)
    {

        // feedback of motor,Modify this value if you setting a different Master ID
        mtr.id = (rx_data[0])&0x0F;
        mtr.state = (rx_data[0])>>4;
        mtr.p_int=(rx_data[1]<<8)|rx_data[2];
        mtr.v_int=(rx_data[3]<<4)|(rx_data[4]>>4);
        mtr.t_int=((rx_data[4]&0xF)<<8)|rx_data[5];
        mtr.pos = uint_to_float(mtr.p_int, P_MIN, P_MAX, 16); // (-12.5,12.5)
        mtr.vel = uint_to_float(mtr.v_int, V_MIN, V_MAX, 12); // (-45.0,45.0)
        mtr.toq = uint_to_float(mtr.t_int, T_MIN, T_MAX, 12);  // (-18.0,18.0)
        mtr.Tmos = (float)(rx_data[6]);
        mtr.Tcoil = (float)(rx_data[7]);
    }
}


void ctrl_motor(CAN_HandleTypeDef* hcan,uint16_t id, Motor_MIT_Data_t* _dm43_mit_t) //MIT模式
{
    
    uint32_t send_mail_box;
    uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;
    
    pos_tmp = float_to_uint(_dm43_mit_t->p_int, P_MIN, P_MAX, 16);
    vel_tmp = float_to_uint(_dm43_mit_t->v_int, V_MIN, V_MAX, 12);
    kp_tmp  = float_to_uint(_dm43_mit_t->kp_int, KP_MIN, KP_MAX, 12);
    kd_tmp  = float_to_uint(_dm43_mit_t->kd_int, KD_MIN, KD_MAX, 12);
    tor_tmp = float_to_uint(_dm43_mit_t->t_int, T_MIN, T_MAX, 12);
	
	Tx_Header.StdId=id;
	Tx_Header.IDE=CAN_ID_STD;
	Tx_Header.RTR=CAN_RTR_DATA;
	Tx_Header.DLC=0x08;
	dm43_can_send_data[0] = (pos_tmp >> 8);
	dm43_can_send_data[1] = pos_tmp;
	dm43_can_send_data[2] = (vel_tmp >> 4);
	dm43_can_send_data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
	dm43_can_send_data[4] = kp_tmp;
	dm43_can_send_data[5] = (kd_tmp >> 4);
	dm43_can_send_data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
	dm43_can_send_data[7] = tor_tmp;
	
	HAL_CAN_AddTxMessage(&hcan1, &Tx_Header, dm43_can_send_data, &send_mail_box);
}	

void ctrl_motor2(CAN_HandleTypeDef* hcan,uint16_t id, float _pos, float _vel) //位置模式
{
//	uint8_t *pbuf,*vbuf;
//	pbuf=(uint8_t*)&_pos;
//	vbuf=(uint8_t*)&_vel;
//	
//	hcan->pTxMsg->StdId = id;
//	hcan->pTxMsg->IDE = CAN_ID_STD;
//	hcan->pTxMsg->RTR = CAN_RTR_DATA;
//	hcan->pTxMsg->DLC = 0x08;
//	hcan->pTxMsg->Data[0] = *pbuf;
//	hcan->pTxMsg->Data[1] = *(pbuf+1);
//	hcan->pTxMsg->Data[2] = *(pbuf+2);
//	hcan->pTxMsg->Data[3] = *(pbuf+3);
//	hcan->pTxMsg->Data[4] = *vbuf;
//	hcan->pTxMsg->Data[5] = *(vbuf+1);
//	hcan->pTxMsg->Data[6] = *(vbuf+2);
//	hcan->pTxMsg->Data[7] = *(vbuf+3);
//	
//	HAL_CAN_Transmit(hcan, 100);
}	

void ctrl_motor3(CAN_HandleTypeDef* hcan,uint16_t id, float _vel) //速度模式
{
//uint8_t *vbuf;
//	vbuf=(uint8_t*)&_vel;
//	
//	hcan->pTxMsg->StdId = id;
//	hcan->pTxMsg->IDE = CAN_ID_STD;
//	hcan->pTxMsg->RTR = CAN_RTR_DATA;
//	hcan->pTxMsg->DLC = 0x04;
//	hcan->pTxMsg->Data[0] = *vbuf;
//	hcan->pTxMsg->Data[1] = *(vbuf+1);
//	hcan->pTxMsg->Data[2] = *(vbuf+2);
//	hcan->pTxMsg->Data[3] = *(vbuf+3);
//	
//	HAL_CAN_Transmit(hcan, 100);
}	

void start_motor(CAN_HandleTypeDef* hcan,uint16_t id)
{
    uint32_t send_mail_box;

	Tx_Header.StdId=id;
	Tx_Header.IDE=CAN_ID_STD;
	Tx_Header.RTR=CAN_RTR_DATA;
	Tx_Header.DLC=0x08;
	dm43_can_send_data[0] = 0xFF;
	dm43_can_send_data[1] = 0xFF;
	dm43_can_send_data[2] = 0xFF;
	dm43_can_send_data[3] = 0xFF;
	dm43_can_send_data[4] = 0xFF;
	dm43_can_send_data[5] = 0xFF;
	dm43_can_send_data[6] = 0xFF;
	dm43_can_send_data[7] = 0xFC;
        
	HAL_CAN_AddTxMessage(&hcan1, &Tx_Header, dm43_can_send_data, &send_mail_box);
}	

void lock_motor(CAN_HandleTypeDef* hcan,uint16_t id)
{
    uint32_t send_mail_box;

	Tx_Header.StdId=id;
	Tx_Header.IDE=CAN_ID_STD;
	Tx_Header.RTR=CAN_RTR_DATA;
	Tx_Header.DLC=0x08;
	dm43_can_send_data[0] = 0xFF;
	dm43_can_send_data[1] = 0xFF;
	dm43_can_send_data[2] = 0xFF;
	dm43_can_send_data[3] = 0xFF;
	dm43_can_send_data[4] = 0xFF;
	dm43_can_send_data[5] = 0xFF;
	dm43_can_send_data[6] = 0xFF;
	dm43_can_send_data[7] = 0xFD;
        
	HAL_CAN_AddTxMessage(&hcan1, &Tx_Header, dm43_can_send_data, &send_mail_box);
}	


void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);


    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);



}
