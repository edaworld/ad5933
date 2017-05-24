/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I2C EEPROM(AT24C02)���ԣ�������Ϣͨ��USART1��ӡ�ڵ��Եĳ����ն�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsP_i2c_ee.h"
#include <string.h>
#include "AD5933.h"

#include "bsp_SysTick.h"
#include "key_scan.h"
#include "bsp_led.h"

#define  EEP_Firstpage      0x00

#define  STATE_IDLE         0x00
#define  STATE_AD_INIT      0x01
#define  STATE_AD_ING       0x02
#define  STATE_AD_IDLE      0x03

#define  KEY_OK               1
#define  KEY_NG               0
//u8 I2c_Buf_Write[256];
//u8 I2c_Buf_Read[256];
void I2C_Test(void);
void State_Idle_Pro(void);
void State_Init_pro(void);
void State_AD_ing_pro(void);
void State_AD_Idle_pro(void);


unsigned char temp0, temp1, temp2;
unsigned char ucState;
unsigned char ucKey1, ucKey2;
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    //int i;
    //unsigned char state;
    //unsigned int real,img;
    /* ����1��ʼ�� */
    //	USART1_Config();
    USARTx_Config();
    /* ����SysTick Ϊ1ms�ж�һ�� */
    init_key();
    LED_GPIO_Config();
    SysTick_Init();


    //printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");
    //LED_GPIO_Config();

    /* I2C �����(AT24C02)ʼ�� */
    I2C_EE_Init();

    //printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");

    //I2C_Test();

    ucState = STATE_IDLE;
    Delay_us(5);//enable systick

    //SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    //SysTick->CTRL |=  SysTick_CTRL_TICKINT_Msk;

    while (1)
    {
        if( ucState == STATE_IDLE )
            State_Idle_Pro();
        else if( ucState == STATE_AD_INIT )
            State_Init_pro();
        else if( ucState == STATE_AD_ING )
            State_AD_ing_pro();
        else if( ucState == STATE_AD_IDLE )
            State_AD_Idle_pro();
    }//while(1)
}

void State_Idle_Pro(void)
{
    if( ucKey1 == KEY_OK )
    {
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;//�ر��ж�
        Init_AD5933();
        SysTick->CTRL |=  SysTick_CTRL_TICKINT_Msk;//ʹ���ж�
        ucState =  STATE_AD_INIT;
        ucKey1 = KEY_NG;
    }
}

void State_Init_pro(void)
{
    if( ucKey2 == KEY_OK )
    {
        ucState =  STATE_AD_ING;
        ucKey2 = KEY_NG;
    }
}

void State_AD_ing_pro(void)
{
    unsigned char temp;
    unsigned int real, img;

    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;//�ر��ж�,������I2C��Ӱ�죡��
    AD5933_Set_Mode_Freq_Start();
    SysTick->CTRL |=  SysTick_CTRL_TICKINT_Msk;//ʹ���ж�
    while( ucState == STATE_AD_ING )
    {
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;//�ر��ж�,������I2C��Ӱ�죡��
        while(1)//wait for DFT finish
        {
            temp = AD5933_Get_DFT_ST();
            if( temp & 0x02)//ʵ�����鲿��Ч
                break;
        }
        real = AD5933_Get_Real();
        img  = AD5933_Get_Img();
        AD5933_Set_Mode_Freq_Repeat();

        SysTick->CTRL |=  SysTick_CTRL_TICKINT_Msk;//ʹ���ж�

        //printf("%c", '$');
        //printf("%04X", real);
        //printf("%04X", img);
        //printf("%c", '#');
        printf("$%04X%04X#", real, img);
        Delay_us(5);//5ms
        //judege the state
        if( ucKey2 == KEY_OK )
        {
            ucState =  STATE_AD_IDLE;
            ucKey2 = KEY_NG;
        }
    }
}

void State_AD_Idle_pro(void)
{
    //judege the state
    if( ucKey2 == KEY_OK )
    {
        ucState =  STATE_AD_ING;
        ucKey2 = KEY_NG;
    }
}


void I2C_Test(void)
{
    //test for AD5933's registers!!
    I2C_EE_ByteWrite( 0x82, 0x06);
    I2C_EE_ByteWrite( 0x83, 0x55);
    I2C_EE_ByteWrite( 0x84, 0xaa);
    temp0 = I2C_EE_ByteRead( 0x82 );
    temp1 = I2C_EE_ByteRead( 0x83 );
    temp2 = I2C_EE_ByteRead( 0x84 );
    if( temp0 == 0x06 && temp1 == 0x55 && temp2 == 0xaa )
        printf("AD5933 register is OK! \r\n");
    else
        printf("AD5933 register is NG! \r\n");
}


/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��
  */
//void I2C_Test(void)
//{
//	u16 i;

//	printf("д�������\n\r");
//
//	for ( i=0; i<=255; i++ ) //��仺��
//  {
//    I2c_Buf_Write[i] = i;

//    printf("0x%02X ", I2c_Buf_Write[i]);
//    if(i%16 == 15)
//        printf("\n\r");
//   }

//  //��I2c_Buf_Write��˳�����������д��EERPOM��
//   //LED1(ON);
//	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
//	//LED1(OFF);
//
//  printf("\n\rд�ɹ�\n\r");
//
//   printf("\n\r����������\n\r");
//  //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
//    //LED2(ON);
//	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);
//   //LED2(OFF);
//
//  //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
//	for (i=0; i<256; i++)
//	{
//		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
//		{
//			printf("0x%02X ", I2c_Buf_Read[i]);
//			printf("����:I2C EEPROMд������������ݲ�һ��\n\r");
//			return;
//		}
//    printf("0x%02X ", I2c_Buf_Read[i]);
//    if(i%16 == 15)
//        printf("\n\r");
//
//	}
//  printf("I2C(AT24C02)��д���Գɹ�\n\r");
//}
/*********************************************END OF FILE**********************/
