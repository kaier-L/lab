#include "stm32f10x.h"
#include "button.h"

Button_TypeDef button;

void App_SPI1_Init(void);
void App_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx, uint8_t *pDataRx, uint16_t Size);
void App_W25Q64_WriteEnable(void);
void App_W25Q64_WaitBusy(void);
void App_W25Q64_SaveByte(uint8_t Byte);
uint8_t App_W25Q64_LoadByte(void);
void App_Button_Init(void);
void App_OnBoardLED_Init(void);
void button_clicked_cb(uint8_t clicks);


int main()
{
    App_SPI1_Init();
    App_OnBoardLED_Init();
    App_Button_Init();

    if(App_W25Q64_LoadByte() == 0x01)
    {
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
    }
    else
    {
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
    }

    while(1)
    {
        My_Button_Proc(&button);
    }
}

void button_clicked_cb(uint8_t clicks)
{
    if(clicks == 1)
    {
        if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET)
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
            App_W25Q64_SaveByte(0x01);
        }
        else
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
            App_W25Q64_SaveByte(0x00);
        }
    }
}


void App_Button_Init(void)
{
    Button_InitTypeDef Button_InitStruct={0};

    Button_InitStruct.GPIOx=GPIOA;
    Button_InitStruct.GPIO_Pin=GPIO_Pin_0;
    Button_InitStruct.button_clicked_cb=button_clicked_cb;
    Button_InitStruct.LongPressTime=1000;
    Button_InitStruct.LongPressTickInterval=200;
    Button_InitStruct.ClickInterval=300;

    My_Button_Init(&button,&Button_InitStruct);
}

void App_SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct={0};
    SPI_InitTypeDef SPI_InitStruct={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &SPI_InitStruct);

    SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}


void App_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx, uint8_t *pDataRx, uint16_t Size)
{
    SPI_Cmd(SPIx, ENABLE);

    SPI_I2S_SendData(SPIx, pDataTx[0]);

    for(uint16_t i=0; i<Size-1; i++)
    {
        while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

        SPI_I2S_SendData(SPIx, pDataTx[i+1]);

        while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

        pDataRx[i] = SPI_I2S_ReceiveData(SPIx);
    }

    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

    pDataRx[Size-1] = SPI_I2S_ReceiveData(SPIx);

    SPI_Cmd(SPIx, DISABLE);
}


void App_W25Q64_WriteEnable(void)
{
    uint8_t cmd = 0x06;

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);

    App_SPI_MasterTransmitReceive(SPI1, &cmd, &cmd, 1);

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
}

void App_W25Q64_WaitBusy(void)
{
    uint8_t buffer[2];

    do
    {
        buffer[0] = 0x05;
        buffer[1] = 0xFF;

        GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);

        App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 2);

        GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
    }
    while(buffer[1] & 0x01);
}

void App_W25Q64_SaveByte(uint8_t Byte)
{
    uint8_t buffer[8];

    App_W25Q64_WriteEnable();

    buffer[0] = 0x20;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);

    App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 4);

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);

    App_W25Q64_WaitBusy();

    App_W25Q64_WriteEnable();

    buffer[0] = 0x02;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = Byte;

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);

    App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 5);

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);

    App_W25Q64_WaitBusy();
}


uint8_t App_W25Q64_LoadByte(void)
{
    uint8_t buffer[5];

    buffer[0] = 0x03;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0xFF;

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);

    App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 5);

    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);

    return buffer[4];
}

void App_OnBoardLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}
