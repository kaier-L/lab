#include"stm32f10x.h"
#include"I2C.h"
void My_I2C1_Init();

int main()
{
    My_I2C1_Init();
    uint8_t commands[]={0x00,0x8d,0x14,0xaf,0xa5};
    My_I2C_SendBytes(I2C1,0X78,commands,5);

    while(1)
    {

    }
}
void My_I2C1_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitTypeDef s1={0};
    s1.GPIO_Pin=GPIO_Pin_6;
    s1.GPIO_Mode=GPIO_Mode_AF_OD;
    s1.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&s1);
    GPIO_InitTypeDef s2={0};
    s2.GPIO_Pin=GPIO_Pin_7;
    s2.GPIO_Mode=GPIO_Mode_AF_OD;
    s2.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&s2);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,DISABLE);

    I2C_InitTypeDef s3;
    s3.I2C_ClockSpeed=400000;
    s3.I2C_Mode=I2C_Mode_I2C;
    s3.I2C_DutyCycle=I2C_DutyCycle_2;
    I2C_Init(I2C1,&s3);

    I2C_Cmd(I2C1,ENABLE);



}
