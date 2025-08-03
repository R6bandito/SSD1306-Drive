#ifndef __OLED_DRI_H
#define __OLED_DRI_H

#include "stm32f10x.h"                  // Device header
#include <string.h>


/*
		@notice: ���������궨�� (USE_I2C_HAREWARE) (USE_I2C_SOFTWARE)�ֱ��������ò�ͬ��I2CͨѶ��ʽ.
			USE_I2C_HAREWARE : �����øú꣬��ע�͵� USE_I2C_SOFTWAREʱ����ʹ��Ӳ��I2Cͨ��. (Ĭ��)
			USE_I2C_SOFTWARE : �����øú꣬��ע�͵� USE_I2C_HAREWAREʱ����ʹ�����ģ��I2Cͨ��.
			ע�⣺��ֹͬʱ����	USE_I2C_HAREWARE  USE_I2C_SOFTWARE.
*/

/* **************************  */
 #define USE_I2C_HAREWARE 

// #define USE_I2C_SOFTWARE 
/* ************************* */



/*
		@notice:	���к궨�彫���� USE_I2C_SOFTWARE ����ʱ��Ч.
			I2C_Soft_SCL_PortSource , I2C_Soft_SDA_PortSource :ͨ��ʱ����,������GPIO�˿ڵĴ���.ͨ�������Ӧ�˿ڵ�ע�������ö�Ӧ��ͨ�Ŷ˿�.
				(���֧�ֵ�GPIOC).
			I2C_Soft_SCL_PinSource , I2C_Soft_SDA_PinSource :Ϊ��ӦGPIO�ڵ����ţ�ʹ��ʱӦ�������Ϊ�û�ʵ��ʹ�õ�����.
			
			Ĭ�����: PB6 -> I2C_SCL  PB7 -> I2C_SDA.
*/
#ifdef USE_I2C_SOFTWARE
/* *************************************************************  */
	#define SLAVE_ADDRESS  0x78  // OLED�豸��ַ.
	
	#define I2C_Soft_SCL_PortSource  GPIOB
	
//  #define I2C_Soft_SCL_PortSource  GPIOA

//	#define I2C_Soft_SCL_PortSource  GPIOC
	
	#define I2C_Soft_SDA_PortSource GPIOB
	
//	#define I2C_Soft_SDA_PortSource GPIOA

//	#define I2C_Soft_SDA_PortSource GPIOC
	
	#define I2C_Soft_SCL_PinSource  GPIO_Pin_6
	
	#define I2C_Soft_SDA_PinSource  GPIO_Pin_7
/* *************************************************************  */
#endif // USE_I2C_SOFTWARE



/*
		@notice:	���к궨�彫���� USE_I2C_HAREWARE ����ʱ��Ч.
			I2C_SCL_SDA_PortSource :	Ӳ��I2C�����õ�GPIO�˿�.
			I2C_SDA_PinSource : ͨ����������.
			I2C_SCL_PinSource : ʱ��������.
			
		ע��: Ӳ��I2C��֧�� I2C1. ���ʵ��Ӧ����I2C1��Ӧ��GPIO�˿�������. 
*/

#ifdef USE_I2C_HAREWARE
/* *************************************************************  */
	// �豸ʶ���ַ.
	#define  SLAVE_ADDRESS  0x78  

	// Ӳ��I2C���Ŷ���.
	#define I2C_SCL_SDA_PortSource  GPIOB
	
	#define I2C_SDA_PinSource GPIO_Pin_7
	
	#define I2C_SCL_PinSource GPIO_Pin_6
/* *************************************************************  */
#endif // USE_I2C_HAREWARE


#define HEX_SHOW_TYPE_INTEGRITY  0xAA
#define HEX_SHOW_TYPE_SIMPLE  0xBB

/*
	@�����ַӳ��.
*/
#define SET_CONTRAST_CONTROL  0x81  // �Աȶ�����.
#define SET_DISPLAY_ON  0xAF  	// ������ʾ.
#define SET_DISPLAY_OFF  0xAE  	// �ر���ʾ.
#define SET_MULTIPLEX_RATIO  0xA8		// ��·����������. 
#define SET_DISPLAY_OFFSET  0xD3		// ��ʾƫ������.
#define SET_DISPLAY_CLK_DIV__OSC_FREQ  0xD5  // ʱ�ӷ�Ƶ��/����Ƶ������.
#define SET_DISPALY_START_LINE0  0x40  // ��ʾ��ʼ������Ϊ0(��0�п�ʼ��ʾ).
#define SET_SENGMENT_REMAP_OFF  0xA1  //������ʾģʽ(�ر�ˮƽ��ת). 
#define SET_SENGMENT_REMAP_ON  0xA0   // ����ˮƽ��ת��ʾ.
#define SET_SENGMENT_VERTICAL_NORMAL  0xC8 // ������ʾģʽ(�رմ�ֱ��ת).
#define SET_SENGMENT_VERTICAL_ON  0xC0 // ������ֱ��ת.
#define SET_COMPINS_HARDWARE_CONFIG  0xDA  // COM����Ӳ������.
#define SET_PRE_CHARGE_PERIOD  0xD9 		// Ԥ�����������.
#define SET_VCOMH_DESELECT_LEVEL  0xDB  // ����VCOMH��ѹ����.
#define SET_ENTIRE_DISPLAY_RAM  0xA4  // ����RAM��ʾ.
#define SET_ENTIRE_DISPLAY_ON  0xA5		// ������RAM��ʾ.
#define SET_NORMAL_DISPLAY  0xA6  // ������ʾ.
#define SET_INVERSE_DISPLAY  0xA7 // ������ʾ.
#define SET_CHARGE_PUMP  0x8D   // ���ó���.
#define SET_ADDRESSING_MODE 0x20  // Ѱַģʽ����.

/*
	@notice: OLED���ýṹ�壬���øýṹ��������ÿɵ���OLED���ȣ���ʾ��ת����������ʾ�ȹ���.
						�ɴ���NULL����ʹ��Ĭ�ϲ�����������.
*/
typedef struct oled_type {
	
	uint8_t DispalyOffset;
	uint8_t AddressMode;
	FunctionalState SegmentRemap;
	uint8_t Contrast;
	FunctionalState VerticalDis;
	FunctionalState Inverse;
	FunctionalState EntireDis;
	
} OLED_InitTypeDef;

#define Address_Mode_Horizontal  0x00
#define Address_Mode_Vertical  0x01
#define Address_Mode_Page  0x02

typedef struct {
	char hzgbk[2]; // ���ֵ�GBK����

	uint8_t font[32]; // �������ݣ�16x16��������

} OLED_ChineseFontType;


void OLED_Init(OLED_InitTypeDef *OLED_InitStructure); // OLED��ʼ��.

void OLED_ClearScreen(void); // OLED����.

void OLED_DisplayChar(uint8_t Lines, uint8_t Columns, char w_char); // ��ʾ�ַ�.

void OLED_DisplayStr(uint8_t Lines, uint8_t Columns, char* str); // ��ʾ�ַ���.

void OLED_DisplayNum(uint8_t Lines, uint8_t Columns, uint64_t Num); // ��ʾ����.

void OLED_DisplaySignedNum(uint8_t Lines, uint8_t Columns, int64_t Num); // ��ʾ����(����).

void OLED_DisplayHexNum_4Byte(uint8_t Lines, uint8_t Columns, uint32_t HexNum,  uint8_t Mode); // ��ʾ����(ʮ������)(4�ֽ�).

void OLED_DisplayHexNum_1Byte(uint8_t Lines, uint8_t Columns, uint8_t HexNum, uint8_t Mode); // ��ʾ����(ʮ������)(1�ֽ�).

void OLED_DisplayBinNum(uint8_t Lines, uint8_t Columns, uint8_t Num);  // ��������ʾ.

void OLED_DisplayChinese(uint8_t Lines, uint8_t Columns, const char *cnstr);


#endif // __OLED_DRI_H
