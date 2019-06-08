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
	CPU 主频72M 
	10 -- IIC时钟频率为205k
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
	//SCL高电平，SDA下降沿 ->起始信号
	MyIIC_SDA_OUT();     //sda线输出
	MyIIC_SDA=1;	  	  
	MyIIC_SCL=1;
	MyIIC_Delay();
 	MyIIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MyIIC_Delay();
	MyIIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

/**
*  @brief      IIC stop
*  @param      none
*  @return     none
*  @note	   
*/
void MyIIC_Stop(void)
{
	MyIIC_SDA_OUT();//sda线输出
	MyIIC_SCL=0;
	MyIIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	MyIIC_Delay();
	MyIIC_SCL=1; 
	MyIIC_SDA=1;//发送I2C总线结束信号
	MyIIC_Delay();
}

/**
*  @brief      IIC sendbyte
*  @param      uint8_t Byte 待发送字节
*  @return     none
*  @note	   
*/
void MyIIC_SendByte(uint8_t txd)
{
	u8 t;   
	MyIIC_SDA_OUT(); 	    
    MyIIC_SCL=0;//拉低时钟开始数据传输
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
*  @param      uint8_t ack 读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
*  @return     uint8_t ackuint8_t value 读取的数据
*  @note	   当读数据的时候，Slave设备每发送完8个数据位，如果Master希望继续读下一个字节，
Master应该回答“ACK”以提示Slave准备下一个数据，
如果Master不希望读取更多字节，
Master应该回答“NACK”以提示Slave设备准备接收Stop信号。
*/
uint8_t MyIIC_ReadByte(uint8_t ack)
{
	
	unsigned char i,receive=0;
	MyIIC_SDA_IN();//SDA设置为输入
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
        MyIIC_NAck();//发送nACK
    else
        MyIIC_Ack(); //发送ACK   
    return receive;
}	

/**
*  @brief      IIC waitack
*  @param      none
*  @return     uint8_t value : 0  成功  1  失败
*  @note	   
*/
uint8_t MyIIC_WaitAck(void)
{
	u8 ucErrTime=0;
	MyIIC_SDA_IN();      //SDA设置为输入  
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
	MyIIC_SCL=0;//时钟输出0 	   
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



