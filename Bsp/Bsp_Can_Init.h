#ifndef __BSP_CAN_INIT
#define __BSP_CAN_INIT

#include "stm32f4xx.h"

#define CAN1_RX0_INT_ENABLE   1   //0,��ʹ��; 1,ʹ��
#define CAN2_RX1_INT_ENABLE   1  //0,��ʹ��; 1,ʹ��



u8 Bsp_Can1_Init(u8 tsjw,u8 tbs2,u8 tbs1,u8 brp,u8 mode);
u8 CAN1_Send_Msg(u8*mag, u8 len);
u8 CAN1_Receive_Msg(u8 *buf);

u8 Bsp_Can2_Init(u8 tsjw,u8 tbs2,u8 tbs1,u8 brp,u8 mode);
u8 CAN2_Send_Msg(u8*mag, u8 len);
u8 CAN2_Receive_Msg(u8 *buf);


#endif



