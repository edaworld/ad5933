#include "key_scan.h"

key stcKey;
extern unsigned char ucKey1, ucKey2; //!!!


void Key_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启按键端口（PA）的时钟*/
    RCC_APB2PeriphClockCmd(CLK_KEY0, ENABLE);
    GPIO_InitStructure.GPIO_Pin = PIN_KEY0;
    // IO 口做输入口时，不用设置输出频率
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(PORT_KEY0, &GPIO_InitStructure);


    RCC_APB2PeriphClockCmd(CLK_KEY1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = PIN_KEY1;
    // IO 口做输入口时，不用设置输出频率
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(PORT_KEY1, &GPIO_InitStructure);

}


void init_key(void)
{
    Key_GPIO_Config();

    stcKey.buf1[0] = KEY_IN_NEFF;
    stcKey.buf1[1] = KEY_IN_NEFF;
    stcKey.buf1[2] = KEY_IN_NEFF;
    stcKey.buf2[0] = KEY_IN_NEFF;
    stcKey.buf2[1] = KEY_IN_NEFF;
    stcKey.buf2[2] = KEY_IN_NEFF;
    stcKey.state[0] = KEY_ST_IDLE;
    stcKey.state[1] = KEY_ST_IDLE;
    stcKey.state[2] = KEY_ST_IDLE;
    stcKey.time10ms[0]   = 0;
    stcKey.time10ms[1]   = 0;
    stcKey.time10ms[2]     = 0;
    stcKey.time1s[0]     = 0;
    stcKey.time1s[1]     = 0;
    stcKey.time1s[2]       = 0;
    stcKey.evt_cdown10[0] = 0;
    stcKey.evt_cdown10[1] = 0;
    stcKey.evt_cdown10[2]  = 0;
    stcKey.evt_up[0]     = 0;
    stcKey.evt_up[1]     = 0;
    stcKey.evt_up[2]       = 0;
    stcKey.ignore[0]     = 0;
    stcKey.ignore[1]     = 0;
    stcKey.ignore[2]       = 0;
    stcKey.interval = 10;
}




void key_scan(void)
{
    unsigned char new_st[3];

    key_gen_buf();

    new_st[0] = key_gen_state(0);
    new_st[1] = key_gen_state(1);
    //new_st[2] = key_gen_state(2);

    key_gen_evt(0, new_st[0]);
    key_gen_evt(1, new_st[1]);
    //key_gen_evt(2,new_st[2]);

    //--here stcKey.evt_up/stcKey.evt_cdown10 is for other procedure using---//
}

void key_gen_buf(void)
{
    stcKey.buf2[0] = stcKey.buf1[0];
    stcKey.buf2[1] = stcKey.buf1[1];
    //stcKey.buf2[2] = stcKey.buf1[2];
    stcKey.buf2[2] = 0;

    stcKey.buf1[0] = GPIO_ReadInputDataBit(PORT_KEY0, PIN_KEY0);
    stcKey.buf1[1] = GPIO_ReadInputDataBit(PORT_KEY1, PIN_KEY1);
    //stcKey.buf1[0] = GPIO_ReadInputDataBit(PORT_KEY0, PIN_KEY0) >> 0;
    //stcKey.buf1[1] = GPIO_ReadInputDataBit(PORT_KEY1, PIN_KEY1) >> 1;
    //stcKey.buf1[2] = GPIO_ReadInputDataBit(PORT_KEY2, PIN_KEY2) >> 4;
    stcKey.buf1[2] = 0;
}
unsigned char key_gen_state(unsigned char num)
{
    return ( stcKey.buf1[num] == KEY_IN_EFF && stcKey.buf2[num] == KEY_IN_EFF );
}

void key_gen_evt(unsigned char i, unsigned char newst )
{
    if( i >= KEY_CONST_NUM)
        return;

    if( stcKey.state[i] == KEY_ST_IDLE )
    {
        stcKey.state[i] = newst;
        stcKey.time10ms[i] = 0;
        stcKey.time1s[i]   = 0;
    }
    else//key_st_down
    {
        if( newst == KEY_ST_DOWN )//record the time
        {
            stcKey.time10ms[i]++;
            if( stcKey.time10ms[i] == 99 )
            {
                stcKey.time10ms[i] = 0;
                stcKey.time1s[i]++;
                if( stcKey.time1s[i] == 10 )
                {
                    stcKey.time1s[i]       = 0;
                    stcKey.evt_cdown10[i]  = 1;
                    stcKey.ignore[i]       = 1;
                }
            }
        }
        else//new_st==key_st_idle
        {
            stcKey.state[i]    = newst;
            if(stcKey.ignore[i] == 0)//if const down evt, the np evt do not gen
                stcKey.evt_up[i]   = 1;
            else
                stcKey.ignore[i]   = 0;

            //-----------add for main process-------------------------//
            ucKey1 = stcKey.evt_up[0];
            ucKey2 = stcKey.evt_up[1];
            stcKey.evt_up[0] = 0;
            stcKey.evt_up[1] = 0;
        }
    }//key_st_down
}
