#include "Bsp_Can_Init.h"
#include "Bsp_Led_Init.h"
#include "Bsp_usart_Init.h"
#include "delay.h"

   //CAN1 
   u16 current; 
   u8 res;
   u8 res1[8];  //���洮�ڽ�������
   u8 j=0; //�����ж��ۼƼ���
   u8 k=0;//�����ж��ۼƼ���
   u8 sendquery1;	
   u8 receivequery1=0;
   u8 SENDDATA1[8];    //ȡֵ��Χ:0-0xFF
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
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
#endif

  Bsp_usart_Init();
  delay_init(168);
  Bsp_Led_Init();
  Bsp_Can1_Init(CAN_SJW_1tq , CAN_BS2_6tq , CAN_BS1_7tq , 3 ,CAN_Mode_Normal);//CAN1��ʼ����ͨģʽ��������1Mkps      ����;CAN_Mode_LoopBack,��ͨ:CAN_Mode_Normal
//  Bsp_Can2_Init(CAN_SJW_1tq , CAN_BS2_6tq , CAN_BS1_7tq , 3 ,CAN_Mode_Normal);//CAN2��ʼ������ģʽ��������1Mkps   

	
 //  printf("\r\n�����͵���ϢΪ:\r\n");
 // USART_SendData(USART1,'���������ֵ������ֵ��Χ��-32768~32768');

	
	
	
	
while(1)                            //������Ҫ������ڷ��͹����ĵ���ֵ����ֵ��current
   {
   if(j==0)
	  for(u8 i=0; i<k;i++)     
       { 
	    u16 n=1;
		for(u8 m=0; m<i; m++) //n=10��i�η�
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




