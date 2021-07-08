#include "SPI.h"

typedef enum {
    IDLE = 0,
    ICM20600,
    MS5611,
    HMC5983
} device_e;


//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
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

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

	SPI2_ReadWriteByte(0xff);//��������		 

}

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 

void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
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
