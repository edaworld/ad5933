/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "bsp_led.h"
extern unsigned char ucState;
unsigned char ucLed_Cnt;
/**
 * @brief  ��ʼ������LED��IO
 * @param  ��
 * @retval ��
 */
void LED_GPIO_Config(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����GPIOB��GPIOF������ʱ��*/
    RCC_APB2PeriphClockCmd( macLED1_GPIO_CLK | macLED2_GPIO_CLK | macLED3_GPIO_CLK, ENABLE);

    /*ѡ��Ҫ���Ƶ�GPIOB����*/
    GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;

    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*���ÿ⺯������ʼ��GPIOB0*/
    GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);

    /*ѡ��Ҫ���Ƶ�GPIOF����*/
    GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;

    /*���ÿ⺯������ʼ��GPIOF7*/
    GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);

    /*ѡ��Ҫ���Ƶ�GPIOF����*/
    GPIO_InitStructure.GPIO_Pin = macLED3_GPIO_PIN;

    /*���ÿ⺯������ʼ��GPIOF7*/
    GPIO_Init(macLED3_GPIO_PORT, &GPIO_InitStructure);

    /* �ر�����led��	*/
    GPIO_SetBits(macLED1_GPIO_PORT, macLED1_GPIO_PIN);

    /* �ر�����led��	*/
    GPIO_SetBits(macLED2_GPIO_PORT, macLED2_GPIO_PIN);

    /* �ر�����led��	*/
    GPIO_SetBits(macLED3_GPIO_PORT, macLED3_GPIO_PIN);

    ucLed_Cnt = 5;
}


void LED_pro(void)
{
    ucLed_Cnt--;
    if( ucLed_Cnt == 0 )
    {
        if( ucState == 0 )
        {
            LED1( OFF ); //��ɫ
            LED2( OFF );
            LED3( ON ); //��ɫ
        }
        else if( ucState == 1 )
        {
            LED1( ON );
            LED2( OFF );
            LED3( OFF );
        }
        else if( ucState == 2 )
        {
            LED1( OFF );
            LED2_TOGGLE;
            LED3( OFF );
        }
        else if( ucState == 3 )
        {
            LED1( OFF );
            LED2( ON );
            LED3( OFF );
        }

        ucLed_Cnt = 5;
    }

}

/*********************************************END OF FILE**********************/