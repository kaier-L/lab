#include"stm32f10x.h"
#include"si2c.h"
SI2C_TypeDef si2c;
int main()
{
    si2c.SCL_GPIOx=GPIOB;
    si2c.SCL_GPIO_Pin=GPIO_Pin_6;
    si2c.SDA_GPIOx=GPIOB;
    si2c.SDA_GPIO_Pin=GPIO_Pin_7;
    My_SI2C_Init(&si2c);


    uint8_t commands[]={0x00,0x8d,0x14,0xaf,0xa5};
    My_I2C_SendBytes(&si2c,0X78,commands,5);

    while(1)
    {

    }
}