#include "bsP_i2c_ee.h"
#include "AD5933.h"
#define AD5933_MCLK 16.776  //=536870912/MCLK;
unsigned int iMode;

void Init_AD5933(void)
{
    int i, j, k;
    //---this paramter is very important, it decides the collect rate. format: Freq / 1024
    AD5933_Set_Freq_Start(200000);//100Khz, 100K/1024 = 100Hz, data update rate is 100Hz!!!!!!
    AD5933_Set_Freq_Add(1);//����Ƶ������Ϊ1
    AD5933_Set_Freq_Num(10);//������������Ҳ����ɨ���е�Ƶ�ʵ���

    AD5933_Set_Mode( AD5933_Standby, AD5933_OUTPUT_2V, AD5933_Gain_1, AD5933_IN_MCLK, AD5933_NReset );
    AD5933_Set_Mode_Rst();
    AD5933_Set_Mode_SysInit();

    //delay for enough time
    for(i = 0; i < 1000; i++)
        for(j = 0; j < 1000; j++)
            k = 0;
}

void Fre_To_Hex(float fre, u8 *buf)
{
    u32 dat;
    dat = (536870912 / (double)(AD5933_MCLK * 1000000)) * fre;
    buf[0] = dat >> 16;
    buf[1] = dat >> 8;
    buf[2] = dat;
}
/*********************************************************************************************************
*������: AD5933_Set_Freq_Start
*����˵��:82��83��84������ʼƵ��
*�β�:
*����ֵ: 
**********************************************************************************************************/
void AD5933_Set_Freq_Start( unsigned int freq )//
{
    unsigned char buf[3];
    Fre_To_Hex( freq, buf );
    I2C_EE_ByteWrite( 0x82, buf[0]);
    I2C_EE_ByteWrite( 0x83, buf[1]);
    I2C_EE_ByteWrite( 0x84, buf[2]);
}
/*********************************************************************************************************
*������: AD5933_Set_Freq_Add
*����˵��:����Ƶ������
*�β�:
*����ֵ: 
**********************************************************************************************************/
void AD5933_Set_Freq_Add( unsigned int afreq )//85,86,87����Ƶ������
{
    unsigned char buf[3];

    Fre_To_Hex( afreq, buf );
    I2C_EE_ByteWrite( 0x85, buf[0]);
    I2C_EE_ByteWrite( 0x86, buf[1]);
    I2C_EE_ByteWrite( 0x87, buf[2]);
}
/*********************************************************************************************************
*������: AD5933_Set_Freq_Num
*����˵��:����������
*�β�:
*����ֵ: 
**********************************************************************************************************/
void AD5933_Set_Freq_Num( unsigned int num )//88,89����������
{
    unsigned char buf[2];
    buf[0] = num >> 8 ;
    buf[1] = num;
    I2C_EE_ByteWrite( 0x88, buf[0]);
    I2C_EE_ByteWrite( 0x89, buf[1]);
}
/*********************************************************************************************************
*������: AD5933_Set_Mode
*����˵��:���ù���ģʽ�����������ѹ��PGA���棬ʱ��ѡ�񼰸�λ��
*�β�:
*����ֵ: 
**********************************************************************************************************/
void AD5933_Set_Mode( unsigned int ctrl, unsigned int out, unsigned int gain, unsigned int clk, unsigned int rst )//80,81���ƼĴ���
{
    iMode = ctrl | out | gain | clk | rst;
    I2C_EE_ByteWrite( 0x80, iMode >> 8 );
    I2C_EE_ByteWrite( 0x81, iMode      );
}

void AD5933_Set_Mode_Rst(void)//��λ������D4λ�Ǹ�λ����λ
{
    iMode  = ( iMode & 0xffef) | AD5933_Reset;
    I2C_EE_ByteWrite( 0x81, iMode      );
}
/*********************************************************************************************************
*������: AD5933_Set_Mode_Standby
*����˵��:���ù���ģʽ����ʼ��/����/����Ƶ��/�ظ�Ƶ��/����/ʡ��/����
*�β�:
*����ֵ: 
**********************************************************************************************************/
void AD5933_Set_Mode_Standby(void)
{
    iMode  = ( iMode & 0x0fff) | AD5933_Standby;
    I2C_EE_ByteWrite( 0x80, iMode >> 8  );
}

void AD5933_Set_Mode_SysInit(void)
{
    iMode  = ( iMode & 0x0fff) | AD5933_SYS_Init;
    I2C_EE_ByteWrite( 0x80, iMode >> 8  );
}

void AD5933_Set_Mode_Freq_Start(void)
{
    iMode  = ( iMode & 0x0fff) | AD5933_Begin_Fre_Scan;
    I2C_EE_ByteWrite( 0x80, iMode >> 8  );
}

void AD5933_Set_Mode_Freq_UP(void)
{
    iMode  = ( iMode & 0x0fff) | AD5933_Fre_UP;
    I2C_EE_ByteWrite( 0x80, iMode >> 8  );
}

void AD5933_Set_Mode_Freq_Repeat(void)
{
    iMode  = ( iMode & 0x0fff) | AD5933_Fre_Rep;
    I2C_EE_ByteWrite( 0x80, iMode >> 8  );
}

void AD5933_Set_Mode_Freq_Temp(void)
{
    iMode  = ( iMode & 0x0fff) | AD5933_Get_Temp;
    I2C_EE_ByteWrite( 0x80, iMode >> 8  );
}
/*********************************************************************************************************
*������: AD5933_Get_DFT_ST
*����˵��:��ȡ״̬���¶Ȳ�����Ч/ʵֵ����ֵ��Ч/Ƶ��ɨ�����
*�β�:
*����ֵ: 
**********************************************************************************************************/
unsigned char  AD5933_Get_DFT_ST(void)
{
    unsigned char temp;
    temp = I2C_EE_ByteRead( 0x8F );
    return temp;
}
/*********************************************************************************************************
*������: AD5933_Get_Real
*����˵��:��ȡʵ��
*�β�:
*����ֵ: 
**********************************************************************************************************/
unsigned int AD5933_Get_Real(void)
{
    unsigned char high, low;
    unsigned int real;

    high = I2C_EE_ByteRead( 0x94 );
    low = I2C_EE_ByteRead( 0x95 );
    real = (high << 8) | low;
    return real;
}
/*********************************************************************************************************
*������: AD5933_Get_Img
*����˵��:��ȡ�鲿
*�β�:
*����ֵ: 
**********************************************************************************************************/
unsigned int AD5933_Get_Img(void)
{
    unsigned char high, low;
    unsigned int img;

    high = I2C_EE_ByteRead( 0x96 );
    low = I2C_EE_ByteRead( 0x97 );
    img = (high << 8) | low;
    return img;
}
