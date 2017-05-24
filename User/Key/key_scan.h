#include "stm32f10x.h"

//---------------------------GPIO--------------------------------------//
// ʹ��KEY1�����Ҫʹ��KEY2�Ļ����Ѻ�KEY1_PA0ע�͵�����

#define               CLK_KEY0                      RCC_APB2Periph_GPIOA
#define               PORT_KEY0    	                GPIOA			   
#define               PIN_KEY0		                  GPIO_Pin_0


#define               CLK_KEY1                      RCC_APB2Periph_GPIOC
#define               PORT_KEY1    	                GPIOC		   
#define               PIN_KEY1		                  GPIO_Pin_13



 /*******
 *�������±��ã�����K1��K2Ĭ���ǵ͵�ƽ���������µ�ʱ���Ǹߵ�ƽ
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	1
#define KEY_OFF	0


//----------------------------------------------------------------//
#define KEY_ST_IDLE       0
#define KEY_ST_DOWN       1

#define KEY_NEFF          0             
#define KEY_EFF           1   

#define KEY_IN_NEFF       0
#define KEY_IN_EFF        1



#define KEY_CONST_NUM     3       //key number
#define SWH_MASK          0x3f    //3-0x3f, 2-0x1b, 1-0x09
//#define SWH_ENA0          0x08
//#define SWH_ENA1          0x10
//#define SWH_ENA2          0x20

//#define KEY_CONST_NUM     3

typedef struct{
  unsigned char buf1[3];
  unsigned char buf2[3];
  unsigned char state[3];
  unsigned char time10ms[3];
  unsigned char time1s[3];
  unsigned char interval;
  unsigned char ignore[3];
  unsigned char evt_up[3];
  unsigned char evt_cdown10[3];
  //unsigned char rst_flag[3];
}key;

extern key stcKey;
extern void init_key(void);
extern void key_scan(void);
extern void key_gen_buf(void);
extern unsigned char key_gen_state(unsigned char num);
extern void key_gen_evt(unsigned char i,unsigned char newst );


void Key_GPIO_Config(void);
//uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);



