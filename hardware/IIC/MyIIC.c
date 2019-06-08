/**
* @brief       MyIIC 
* @harfware    stm32f1 
* @version     V1.0
* @date        19-June-2018
* @author      Junqi Liu
* 
*/

#include "MyIIC.h"
//#include "main.h"

/**
*  @brief      IIC delay
*  @param      none
*  @return     none
*  @note	   
*/
static void MyIIC_Delay(void)
{
	uint8_t i;
	/*
	CPU ��Ƶ72M 
	10 -- IICʱ��Ƶ��Ϊ205k
	*/
	for(i=0;i<15;i++)
	;
}

/**
*  @brief      IIC start
*  @param      none
*  @return     none
*  @note	   
*/
void MyIIC_Start(void)
{
	//SCL�ߵ�ƽ��SDA�½��� ->��ʼ�ź�
	MyIIC_SDA_OUT();     //sda�����
	MyIIC_SDA=1;	  	  
	MyIIC_SCL=1;
	MyIIC_Delay();
 	MyIIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MyIIC_Delay();
	MyIIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

/**
*  @brief      IIC stop
*  @param      none
*  @return     none
*  @note	   
*/
void MyIIC_Stop(void)
{
	MyIIC_SDA_OUT();//sda�����
	MyIIC_SCL=0;
	MyIIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	MyIIC_Delay();
	MyIIC_SCL=1; 
	MyIIC_SDA=1;//����I2C���߽����ź�
	MyIIC_Delay();
}

/**
*  @brief      IIC sendbyte
*  @param      uint8_t Byte �������ֽ�
*  @return     none
*  @note	   
*/
void MyIIC_SendByte(uint8_t txd)
{
	u8 t;   
	MyIIC_SDA_OUT(); 	    
    MyIIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MyIIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    MyIIC_SCL=1;
		    MyIIC_Delay(); 
		    MyIIC_SCL=0;	
		    MyIIC_Delay();
    }		
}

/**
*  @brief      IIC readbyte
*  @param      uint8_t ack ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  
*  @return     uint8_t ackuint8_t value ��ȡ������
*  @note	   �������ݵ�ʱ��Slave�豸ÿ������8������λ�����Masterϣ����������һ���ֽڣ�
MasterӦ�ûش�ACK������ʾSlave׼����һ�����ݣ�
���Master��ϣ����ȡ�����ֽڣ�
MasterӦ�ûش�NACK������ʾSlave�豸׼������Stop�źš�
*/
uint8_t MyIIC_ReadByte(uint8_t ack)
{
	
	unsigned char i,receive=0;
	MyIIC_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MyIIC_SCL=0; 
        MyIIC_Delay();
		MyIIC_SCL=1;
        receive<<=1;
        if(MyIIC_READ_SDA)receive++;   
		MyIIC_Delay(); 
    }					 
    if (!ack)
        MyIIC_NAck();//����nACK
    else
        MyIIC_Ack(); //����ACK   
    return receive;
}	

/**
*  @brief      IIC waitack
*  @param      none
*  @return     uint8_t value : 0  �ɹ�  1  ʧ��
*  @note	   
*/
uint8_t MyIIC_WaitAck(void)
{
	u8 ucErrTime=0;
	MyIIC_SDA_IN();      //SDA����Ϊ����  
	MyIIC_SDA=1;MyIIC_Delay();	   
	MyIIC_SCL=1;MyIIC_Delay();	 
	while(MyIIC_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MyIIC_Stop();
			return 1;
		}
	}
	MyIIC_SCL=0;//ʱ�����0 	   
	return 0; 
}

/**
*  @brief      IIC ack
*  @param      none
*  @return     none
*  @note	   
*/
void MyIIC_Ack(void)
{

	MyIIC_SCL=0;
	MyIIC_SDA_OUT();
	MyIIC_SDA=0;
	MyIIC_Delay();
	MyIIC_SCL=1;
	MyIIC_Delay();
	MyIIC_SCL=0;
}

/**
*  @brief      IIC nack
*  @param      none
*  @return     none
*  @note	   
*/
void MyIIC_NAck(void)
{

	MyIIC_SCL=0;
	MyIIC_SDA_OUT();
	MyIIC_SDA=1;
	MyIIC_Delay();
	MyIIC_SCL=1;
	MyIIC_Delay();
	MyIIC_SCL=0;
}



