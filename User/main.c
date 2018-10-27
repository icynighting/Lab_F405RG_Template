#include "Bsp_Can_Init.h"
#include "Bsp_Led_Init.h"
#include "Bsp_usart_Init.h"
#include "delay.h"

   //CAN1 
   u16 current; 
   u8 res;
   u8 res1[8];  //储存串口接收数据
   u8 j=0; //用于中断累计计数
   u8 k=0;//用于中断累计计数
   u8 sendquery1;	
   u8 receivequery1=0;
   u8 SENDDATA1[8];    //取值范围:0-0xFF
   u8 RxMessagecan1[8];  

//   //CAN2
//   u8 sendquery2;	
//   u8 receivequery2=0;  
//   u8 SENDDATA2[8];
//   u8 RxMessagecan2[8];
//  
//   SENDDATA2[0]=0X10;
//   SENDDATA2[1]=0X10;



int main(void)
{

 
	

 
   
#if (CAN1_RX0_INT_ENABLE||CAN2_RX1_INT_ENABLE)
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
#endif

  Bsp_usart_Init();
  delay_init(168);
  Bsp_Led_Init();
  Bsp_Can1_Init(CAN_SJW_1tq , CAN_BS2_6tq , CAN_BS1_7tq , 3 ,CAN_Mode_Normal);//CAN1初始化普通模式，波特率1Mkps      环回;CAN_Mode_LoopBack,普通:CAN_Mode_Normal
//  Bsp_Can2_Init(CAN_SJW_1tq , CAN_BS2_6tq , CAN_BS1_7tq , 3 ,CAN_Mode_Normal);//CAN2初始化环回模式，波特率1Mkps   

	
 //  printf("\r\n您发送的消息为:\r\n");
 // USART_SendData(USART1,'请输入电流值，电流值范围：-32768~32768');

	
	
	
	
while(1)                            //我们需要解读串口发送过来的电流值，赋值给current
   {
   if(j==0)
	  for(u8 i=0; i<k;i++)     
       { 
	    u16 n=1;
		for(u8 m=0; m<i; m++) //n=10的i次方
		 {
                n*=10; 		
		 }
	   current=(res1[i])*n;
	 }
	 
   current=res;
   SENDDATA1[0]=current>>8  ;  
   SENDDATA1[1]=current;  
	  


	 
   sendquery1=CAN1_Send_Msg(SENDDATA1,8);
   receivequery1=CAN1_Receive_Msg(RxMessagecan1);
	  if(!sendquery1)
		  GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	   if(receivequery1)
		  GPIO_ResetBits(GPIOC,GPIO_Pin_12);	   

	  	   
//   sendquery2=CAN2_Send_Msg(SENDDATA2,8);
//   receivequery2=CAN2_Receive_Msg(RxMessagecan2);
//	  if(!sendquery2)
//		  GPIO_ResetBits(GPIOC,GPIO_Pin_11);
//	   if(receivequery2)
//		  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
//	    delay_ms(500);
   
   }
}




