#include "Bsp_Can_Init.h"


//ȫ�ֱ���
CanRxMsg RxMessage1;  
CanRxMsg RxMessage2; 


//CAN1��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 

u8 Bsp_Can1_Init(u8 tsjw,u8 tbs2,u8 tbs1,u8 brp,u8 mode)
{
	
   GPIO_InitTypeDef GPIO_InitStructure;
   CAN_InitTypeDef CAN_InitStructrue;
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE
   NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
   
	
	
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//���ù���
   GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
   GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
   GPIO_Init(GPIOA,&GPIO_InitStructure);

   //���Ÿ�������
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1);
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1);
	
   //CAN��Ԫ����	
   CAN_InitStructrue.CAN_ABOM=ENABLE;//����Զ����߹���
   CAN_InitStructrue.CAN_AWUM=DISABLE;//˯��ģʽͨ���������
   CAN_InitStructrue.CAN_BS1=tbs1;//��ΧCAN_BS1_1tq ~CAN_BS1_16tq
   CAN_InitStructrue.CAN_BS2=tbs2;//��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
   CAN_InitStructrue.CAN_Mode=mode;
   CAN_InitStructrue.CAN_NART=ENABLE;//��ֹ�����Զ�����
   CAN_InitStructrue.CAN_Prescaler=brp;//��Ƶϵ��(Fidv)Ϊbrp+1
   CAN_InitStructrue.CAN_RFLM=DISABLE;//���Ĳ��������µĸ��Ǿɵ�
   CAN_InitStructrue.CAN_SJW=tsjw;//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ��tsjw��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
   CAN_InitStructrue.CAN_TTCM=DISABLE;//��ʱ�䴥��ͨ��ģʽ
   CAN_InitStructrue.CAN_TXFP=DISABLE;//���ȼ��ɱ��ı�ʶ������
   CAN_Init(CAN1,&CAN_InitStructrue);
   
   //���ù�����
   CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//ʹ�ܹ�����
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO0;//������0������FIFO0
   CAN_FilterInitStructure.CAN_FilterIdHigh=0x201<<5;//32λID
   CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x7FF<<5;//ֻƥ��ǰ��11λID
   CAN_FilterInitStructure.CAN_FilterMaskIdLow=0|0x02;  //ֻ��������֡
   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//��ʶ������λģʽ(��һ���Ǳ�ʶ���б�ģʽ)
   CAN_FilterInitStructure.CAN_FilterNumber=0;//ʹ�ܵ��ǹ�����0
   CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32λmask
   CAN_FilterInit(&CAN_FilterInitStructure);
   
#if CAN1_RX0_INT_ENABLE
   CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����
  
   NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
   NVIC_Init(&NVIC_InitStructure);
#endif
  
   return 0;
}

#if CAN1_RX0_INT_ENABLE
void CAN1_RX0_IRQHandler(void)
{

 
  CAN_Receive(CAN1,CAN_FilterFIFO0,&RxMessage1);
  //for(i=0;i<8;i++)
  //printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
	
}
#endif

//can����һ������(�̶���ʽ:IDΪ0x12,��׼֡������֡)
//len:���ݳ���(���Ϊ8)
//msg:����ָ�룬���8���ֽ�
//����ֵ:0,�ɹ�;  ����,ʧ��.
u8 CAN1_Send_Msg(u8*msg, u8 len)
{
   u8 mbox; //��������
   u16 i=0;
   CanTxMsg TxMessage;

	TxMessage.DLC=len;//Ҫ���͵�����֡����(ȡֵ��Χ:0-0x8)
 //  TxMessage.ExtId=0x200;//������չλ��ʶ��(29λ)
   TxMessage.IDE=CAN_Id_Standard;     //��׼֡ (��չ֡:CAN_Id_Extended)
   TxMessage.RTR=CAN_RTR_Data;     //����֡ (Զ��֡:CAN_RTR_Remote)
   TxMessage.StdId=0x200;//��׼��ʶ��
   for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];
   mbox=CAN_Transmit(CAN1,&TxMessage);//��TxMessage����Ϣͨ��CAN1���ͳ�ȥ,�����ͨ����������ֵ��ѯ��
   i=0;
   while((CAN_TransmitStatus(CAN1,mbox)==CAN_TxStatus_Failed)&&(i<0xFFFF))
	   i++;
   if(i>=0xFFFF) return 1;
   return 0;
}


//can�ڽ������ݲ�ѯ
//buf:���ݻ�����
//����ֵ:0,�����ݱ��յ�;   ����,���յ������ݳ���
u8 CAN1_Receive_Msg(u8 *buf)
{
 	u32 i;
extern  CanRxMsg RxMessage1; 
    if (!RxMessage1.DLC)   return 0;		//û�н��յ�����,ֱ���˳� 
    
    for(i=0;i<RxMessage1.DLC;i++)
    buf[i]=RxMessage1.Data[i];  
	return RxMessage1.DLC;
}







//CAN2��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 

u8 Bsp_Can2_Init(u8 tsjw,u8 tbs2,u8 tbs1,u8 brp,u8 mode)
{
	
   GPIO_InitTypeDef GPIO_InitStructure;
   CAN_InitTypeDef CAN_InitStructrue;
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
#if CAN2_RX1_INT_ENABLE
   NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);
   
	
	
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//���ù���
   GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
   GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
   GPIO_Init(GPIOB,&GPIO_InitStructure);

   //���Ÿ�������
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2);
	
   //CAN��Ԫ����	
   CAN_InitStructrue.CAN_ABOM=DISABLE;//����Զ����߹���
   CAN_InitStructrue.CAN_AWUM=DISABLE;//˯��ģʽͨ���������
   CAN_InitStructrue.CAN_BS1=tbs1;//��ΧCAN_BS1_1tq ~CAN_BS1_16tq
   CAN_InitStructrue.CAN_BS2=tbs2;//��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
   CAN_InitStructrue.CAN_Mode=mode;
   CAN_InitStructrue.CAN_NART=ENABLE;//��ֹ�����Զ�����
   CAN_InitStructrue.CAN_Prescaler=brp;//��Ƶϵ��(Fidv)Ϊbrp+1
   CAN_InitStructrue.CAN_RFLM=DISABLE;//���Ĳ��������µĸ��Ǿɵ�
   CAN_InitStructrue.CAN_SJW=tsjw;//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ��tsjw��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
   CAN_InitStructrue.CAN_TTCM=DISABLE;//��ʱ�䴥��ͨ��ģʽ
   CAN_InitStructrue.CAN_TXFP=DISABLE;//���ȼ��ɱ��ı�ʶ������
   CAN_Init(CAN2,&CAN_InitStructrue);
   
   //���ù�����
   CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//ʹ�ܹ�����
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO0;//������14������FIFO1
   CAN_FilterInitStructure.CAN_FilterIdHigh=0x200<<5;      //����IDΪ0x201������
   CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x7FF<<5;  //����ǰ11λID
   CAN_FilterInitStructure.CAN_FilterMaskIdLow=0|0x02;     //ֻ��������֡
   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//��ʶ������λģʽ(��һ���Ǳ�ʶ���б�ģʽ)
   CAN_FilterInitStructure.CAN_FilterNumber=14;//ʹ�ܵ��ǹ�����14
   CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32λmask
   CAN_FilterInit(&CAN_FilterInitStructure);
   
#if CAN2_RX1_INT_ENABLE
   CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO1��Ϣ�Һ��ж�����
  
   NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
   NVIC_Init(&NVIC_InitStructure);
#endif
  
   return 0;
}

#if CAN2_RX1_INT_ENABLE
void CAN2_RX0_IRQHandler(void)
{

  //int i=0;
 CAN_Receive(CAN2,CAN_FilterFIFO0,&RxMessage2);
  //for(i=0;i<8;i++)
  //printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
	
}
#endif

//can����һ������(�̶���ʽ:IDΪ0x12,��׼֡������֡)
//len:���ݳ���(���Ϊ8)
//msg:����ָ�룬���8���ֽ�
//����ֵ:0,�ɹ�;  ����,ʧ��.
u8 CAN2_Send_Msg(u8*msg, u8 len)
{
   u8 mbox; //��������
   u16 i=0;
   CanTxMsg TxMessage;

   TxMessage.DLC=len;//Ҫ���͵����ݳ���
//   TxMessage.ExtId=0x12;//������չλ��ʶ��(29λ)
   TxMessage.IDE=CAN_Id_Standard;     //��׼֡
   TxMessage.RTR=CAN_RTR_Data;       //����֡
   TxMessage.StdId=0x200;//��׼��ʶ��
   for( i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];
   mbox=CAN_Transmit(CAN2,&TxMessage);//��TxMessage����Ϣͨ��CAN1���ͳ�ȥ,�����ͨ����������ֵ��ѯ��
   i=0;
   while((CAN_TransmitStatus(CAN2,mbox)==CAN_TxStatus_Failed)&&(i<0xFFFF))
	   i++;
   if(i>=0xFFFF) return 1;
   return 0;
}


//can�ڽ������ݲ�ѯ
//buf:���ݻ�����
//����ֵ:0,�����ݱ��յ�;   ����,���յ������ݳ���
u8 CAN2_Receive_Msg(u8 *buf)
{
 	u32 i;
 extern  CanRxMsg RxMessage2; 
    if (!RxMessage2.DLC) return 0;		//û�н��յ�����,ֱ���˳� 

    for(i=0;i<RxMessage2.DLC;i++)
    buf[i]=RxMessage2.Data[i];  
	return RxMessage2.DLC;
}

