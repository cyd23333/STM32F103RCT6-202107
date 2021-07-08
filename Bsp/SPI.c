#include "SPI.h"

typedef enum {
    IDLE = 0,
    ICM20600,
    MS5611,
    HMC5983
} device_e;


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}

static device_e get_device(uint8_t address)
{
    device_e device;

    if (address == 0x68) {
        device = ICM20600;

    } else if (address == 0x1e) {
        device = HMC5983;

    } else {
        device = IDLE;
    }

    return device;
}

static void cs(device_e device, uint8_t state)
{
    GPIO_TypeDef* port = 0;
    uint16_t pin = 0;

    switch (device) {
        case IDLE:
            break;

        case ICM20600:
            port = GPIOB;
            pin = GPIO_Pin_12;
            break;

        case HMC5983:
            break;

        default:
            break;
    }
	if(state)
		GPIO_SetBits(port,pin);
	else
		GPIO_ResetBits(port,pin);
}

static uint8_t readwritebyte(uint8_t write)
{
    uint8_t read = 0;

//    HAL_SPI_TransmitReceive(&hspi4, &write, &read, 1, 1000);
	read = SPI2_ReadWriteByte(write);

    return read;
}

int spi4_read(uint8_t address, uint8_t reg, uint8_t *buf, uint8_t len)
{
    device_e device;

    device = get_device(address);

    // for most devices except MS5611, bit7 is R/W bit.
    // when 0, the data is written into the device.
    // when 1, the data from the device is read.
    if (device != MS5611) {
        reg |= 1 << 7;
    }

    // for HMC5983, bit6 is M/S bit.
    // when 0, the register address will remain unchanged in multiple read/write commands.
    // when 1, the register address will be auto incremented in multiple read/write commands.
    if (device == HMC5983 && len > 1) {
        reg |= 1 << 6;
    }

    // lower CS
    cs(device, 0);

    readwritebyte(reg);

    while (len--) {
        *buf++ = readwritebyte(0xff);
    }

    // higher CS
    cs(device, 1);

    return 0;
}

int spi4_write(uint8_t address, uint8_t reg, uint8_t *buf, uint8_t len)
{
    device_e device;

    device = get_device(address);

    // for HMC5983, bit6 is M/S bit.
    // when 0, the register address will remain unchanged in multiple read/write commands.
    // when 1, the register address will be auto incremented in multiple read/write commands.
    if (device == HMC5983 && len > 1) {
        reg |= 1 << 6;
    }

    // lower CS
    cs(device, 0);

    readwritebyte(reg);

    while (len--) {
        readwritebyte(*buf++);
    }

    // higher CS
    cs(device, 1);

    return 0;
}
//PB12  SPI2_CS   NCS 
//PB13  SPI2_SCK  CLK
//PB14  SPI2_MISO SDO
//PB15  SPI2_MOSI SDI
void SPI_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI2, ENABLE); //使能SPI外设

	SPI2_ReadWriteByte(0xff);//启动传输		 

}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 

void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE);
}

int read(char *path, uint8_t *buf, int len)
{
    int ret = -1;

    switch (path[0]) {

        case 's':
            // spi bus
            ret = spi4_read(path[1], path[2], buf, len);
//            ret = spi5_read(path[1], path[2], buf, len);
            break;

        default:
            break;
    }

    return ret;
}

int write(char *path, uint8_t *buf, int len)
{
    int ret = -1;

    switch (path[0]) {

        case 's':
            ret = spi4_write(path[1], path[2], buf, len);
//            ret = spi5_write(path[1], path[2], buf, len);
            break;

        default:
            break;
    }
    return ret;
}
