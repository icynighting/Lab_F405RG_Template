#include "Bsp_Init.h"
#include "delay.h"


int main(void)
{
     Bsp_Init();
     delay_init(168);
    while(1)
    {
 	GPIO_SetBits(GPIOC,GPIO_Pin_11);
       delay_ms(500); 
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	 delay_ms(500);
    }
}




