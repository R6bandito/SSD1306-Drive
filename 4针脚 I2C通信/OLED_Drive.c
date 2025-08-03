#include "OLED_Drive.h"
#include "OLED_Font.h"



#ifdef USE_I2C_HAREWARE

	#define I2C_START()  I2C_GenerateSTART(I2C1, ENABLE) // ������ʼ�ź�.
	
	#define I2C_STOP()  I2C_GenerateSTOP(I2C1, ENABLE)  // ������ֹ�ź�.
	
#endif // USE_I2C_HAREWARE


#ifdef USE_I2C_SOFTWARE

	#define OLED_WRITE_SCL(x)  GPIO_WriteBit(I2C_Soft_SCL_PortSource, I2C_Soft_SCL_PinSource, (BitAction)(x))
	
	#define OLED_WRITE_SDA(x)  GPIO_WriteBit(I2C_Soft_SDA_PortSource, I2C_Soft_SDA_PinSource, (BitAction)(x))

#endif // USE_I2C_SOFTWARE

#ifdef USE_I2C_HAREWARE 


void I2C_Hard_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// I2C_SCL_PinSource -> I2C1_SCL.  I2C_SDA_PinSource -> I2C1_SDA.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PinSource | I2C_SDA_PinSource;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SCL_SDA_PortSource, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; 	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0xF0;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_Cmd(I2C1, ENABLE);
}

ErrorStatus IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint16_t Timeout = 5000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout--;
		if (Timeout == 0) return ERROR;
	}
	return SUCCESS;
}


/*
		@brief: ָ��д��.
		@parm: command  SSD1306�Ŀ���ָ��.
*/
void OLED_WriteCommand(uint8_t command)
{
	I2C_START();
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Transmitter);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, 0x00); // д����.
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, command);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_STOP();
}


/*
		@brief:  д������.
		@parm: Byte  Ҫд����ֽ�����.
*/
void OLED_WriteByte(uint8_t Byte)
{
	I2C_START();
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Transmitter);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, 0x40); // д����.
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, Byte);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_STOP();
}

#endif // USE_I2C_HAREWARE


#ifdef USE_I2C_SOFTWARE
void I2C_Soft_Init(void)
{
	if (I2C_Soft_SCL_PortSource == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	if (I2C_Soft_SCL_PortSource == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	if (I2C_Soft_SCL_PortSource == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_Soft_SCL_PinSource | I2C_Soft_SDA_PinSource;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if (I2C_Soft_SCL_PortSource == GPIOA) GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	if (I2C_Soft_SCL_PortSource == GPIOB) GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	if (I2C_Soft_SCL_PortSource == GPIOC) GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	OLED_WRITE_SCL(1);
	
	OLED_WRITE_SDA(1);
}


/*
		@brief: ���I2C������ʼ�ź�.
		@parm: NULL
*/
void I2C_Soft_Start(void)
{
	OLED_WRITE_SCL(1);
	
	OLED_WRITE_SDA(1);
	
	OLED_WRITE_SDA(0);
	
	OLED_WRITE_SCL(0);
}


/*
		@brief:	���I2C����ֹͣ�ź�.
		@parm: NULL
*/
void I2C_Soft_Stop(void)
{
	OLED_WRITE_SDA(0);
	
	OLED_WRITE_SCL(1);
	
	OLED_WRITE_SDA(1);
}


/*
		@brief:	���I2C����һ���ֽ�.
		@parm: Byte  ��Ҫ���͵��ֽ�����.
*/
void I2C_Soft_SendByte(uint8_t Byte)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		OLED_WRITE_SCL(0);
		
		if (((Byte << i )& 0x80) == 0x80)
		{
			OLED_WRITE_SDA(1);
			
			OLED_WRITE_SCL(1);
		}
		else
		{
			OLED_WRITE_SDA(0);
			
			OLED_WRITE_SCL(1);
		}
	}
}


/*
		@brief:	���I2C����Ӧ��.
		@parm: NULL.
*/
ErrorStatus I2C_Soft_RecvAck(void)
{
	uint16_t Timeout = 0;
	
	OLED_WRITE_SCL(0);
	
	OLED_WRITE_SDA(1);
	
	OLED_WRITE_SCL(1);
	
	while(GPIO_ReadInputDataBit(I2C_Soft_SDA_PortSource, I2C_Soft_SDA_PinSource))
	{
		Timeout++;
		
		if (Timeout > 1000)
		{
			OLED_WRITE_SCL(0);
			
			return ERROR;
		}
	}
	
	OLED_WRITE_SCL(0);
	
	return SUCCESS;
}


/*
		@brief:	���I2Cд������.
		@parm: command  ��Ҫд�������.
*/
void OLED_WriteCommand(uint8_t command)
{
	I2C_Soft_Start();
	
	I2C_Soft_SendByte(SLAVE_ADDRESS);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(SLAVE_ADDRESS); // �ٴγ������·���.
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; // ��Ȼδ���ͳɹ�����ֹ�˴�ͨѶ.
	}
	
	
	I2C_Soft_SendByte(0x00);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(0x00); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	
	I2C_Soft_SendByte(command);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(command); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	I2C_Soft_Stop();
}


/*
		@brief:	���I2Cд������.
		@parm: Byte  ��Ҫд�������.
*/
void OLED_WriteByte(uint8_t Byte)
{
	I2C_Soft_Start();
	
	I2C_Soft_SendByte(SLAVE_ADDRESS);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(SLAVE_ADDRESS); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	
	I2C_Soft_SendByte(0x40);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(0x40); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	
	I2C_Soft_SendByte(Byte);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(Byte); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	I2C_Soft_Stop();
}

#endif // USE_I2C_SOFTWARE


void OLED_WriteCommand_Parm(uint8_t command, uint8_t parm)
{
	OLED_WriteCommand(command);
	OLED_WriteCommand(parm);
}


/*	
		@brief: �������.
		@param: Pages  ҳ���� ֵ�� 0~7.
		@param: Column  ������ ֵ�� 0~127.	
*/
void OLED_SetCursor(uint8_t Pages, uint8_t Column)
{
	OLED_WriteCommand(0xB0 + Pages);  // д��ҳ��ַ.
	OLED_WriteCommand(0x00 | (Column & 0x0F)); // д���е�ַ(��4λ)
	OLED_WriteCommand(0x10 | ((Column & 0xF0) >> 4)); // д���е�ַ(��4λ0)
}


/*
		@brief: OLED����.
*/
void OLED_ClearScreen(void)
{
	OLED_SetCursor(0, 0);
	for(uint8_t i = 0; i < 10; i++)
	{
		for(uint8_t j = 0; j < 128; j++)
		{
			OLED_WriteByte(0x00);
		}
	}
}


/*
		@brief:  OLED��ʾһ���ַ�.
		@parm: Lines  �ַ���������.  
		@parm: Columns  �ַ���������.
		@parm: w_char  ��Ҫ��ʾ���ַ�.
*/
void OLED_DisplayChar(uint8_t Lines, uint8_t Columns, char w_char)
{
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 8);
	
	for(uint8_t i = 0; i < 8; i++)  // ��ʾ�ϰ벿��.
	{
		OLED_WriteByte(OLED_F8x16[w_char - ' '][i]);
	}
	
	OLED_SetCursor(2 * Lines - 1, (Columns - 1) * 8);
	
	for(uint8_t j = 0; j < 8; j++)  // ��ʾ�°벿��.
	{
		OLED_WriteByte(OLED_F8x16[w_char - ' '][j + 8]);
	}
}


/*
		@brief:  OLED��ʾһ���ַ���.
		@parm: Lines  �ַ�����������.  
		@parm: Columns  �ַ�����������.
		@parm: w_char  ��Ҫ��ʾ���ַ���.
*/

void OLED_DisplayStr(uint8_t Lines, uint8_t Columns, char* str)
{
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 8);
	
	for(uint8_t i = 0; str[i] != '\0'; i++) // ��ʾ�ַ����ϰ벿��.
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[str[i] - ' '][j]);
		}
	}
	
	OLED_SetCursor(2 * Lines - 1, (Columns - 1) * 8);
	
	for(uint8_t i = 0; str[i] != '\0'; i++) // ��ʾ�ַ����°벿��.
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[str[i] - ' '][j + 8]);
		}
	}
}


/*
		@brief: �������������ֵ�λ��.
		@parm: Num ��Ҫ���������.
*/
uint8_t CountNumLength(uint64_t Num)
{
	uint64_t Time = 10;
	uint8_t Count = 1;
	
	while(Num / Time != 0)
	{
		Num /= Time;
		Count++;
	}
	
	return Count;
}


/*
		@brief:  �ݵĴη�����.
		@parm: X  ����.
		@parm: Y  ָ��.
*/
uint64_t GetPow(uint8_t X, uint8_t Y)
{
	uint64_t Result = 1;
	
	while(Y --)
	{
		Result *= X;
	}
	
	return Result;
}


/*
		@brief: ��ʾ����.
		@parm: Lines 	��ʾλ��(��).
		@parm: Columns  ��ʾλ��(��).
		@parm: Num	��Ҫ��ʾ������.
*/
void OLED_DisplayNum(uint8_t Lines, uint8_t Columns, uint64_t Num)
{
	uint8_t Length = CountNumLength(Num); 
	
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 8);
	
	for(uint8_t i = 0; i < Length; i++)  // ��ʾ�ϰ벿��.
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[Num / GetPow(10, Length - i - 1) % 10 + 16][j]);
		}
	}
	
	OLED_SetCursor((2 * Lines) - 1, (Columns - 1) * 8);
	
	for(uint8_t i = 0; i < Length; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[Num / GetPow(10, Length - i - 1) % 10 + 16][j + 8]);
		}
	}
}


/*
		@brief: ��ʾ�з�������.
		@parm: Lines 	��ʾλ��(��).
		@parm: Columns  ��ʾλ��(��).
		@parm: Num	��Ҫ��ʾ������.
*/
void OLED_DisplaySignedNum(uint8_t Lines, uint8_t Columns, int64_t Num)
{
	uint64_t NumTemp;
	
	if (Num < 0)
	{
		OLED_DisplayChar(Lines, Columns++, '-');
		
		NumTemp = -Num;
	}
	else if (Num > 0)
	{
		OLED_DisplayChar(Lines, Columns++, '+');
		
		NumTemp = Num;
	}
	
	OLED_DisplayNum(Lines, Columns, NumTemp);
}


/*
		@brief: 16������ʾ(1�ֽ�).
		@parm: Lines ��ʾλ��(��).
		@parm: Columns ��ʾλ��(��).
		@parm: HexNum ����.
		@parm: Mode ��ʾģʽ.
*/

void OLED_DisplayHexNum_1Byte(uint8_t Lines, uint8_t Columns, uint8_t HexNum, uint8_t Mode)
{
	uint8_t num_low = HexNum & 0x0F;
	
	uint8_t num_high = ((HexNum >> 4) & 0x0F);
	
	num_low = (num_low < 10) ? (num_low + '0') : (num_low - 10 + 'A' );
	
	num_high = (num_high < 10) ? (num_high + '0') : (num_high - 10 + 'A' );
	
	if(Mode == HEX_SHOW_TYPE_INTEGRITY)
	{
		OLED_DisplayChar(Lines, Columns, '0');
		
		Columns++;
		
		OLED_DisplayChar(Lines, Columns, 'x');
		
		Columns++;
	}
	
	OLED_DisplayChar(Lines, Columns, num_high); // ��ʾ��4λ.
	
	OLED_DisplayChar(Lines, Columns + 1, num_low);  // ��ʾ��4λ.
}



/*
		@brief: 16������ʾ. (4�ֽ�)
		@parm: Lines ��ʾλ��(��).
		@parm: Columns ��ʾλ��(��).
		@parm: HexNum ����.
		@parm: Mode ��ʾģʽ.
*/
void OLED_DisplayHexNum_4Byte(uint8_t Lines, uint8_t Columns, uint32_t HexNum, uint8_t Mode)
{	
	uint8_t HexByte[4];
	
	HexByte[0] = (HexNum >> 24) & 0xFF; 
	
	HexByte[1] = (HexNum >> 16) & 0xFF; 
	
	HexByte[2] = (HexNum >> 8) & 0xFF;
	
	HexByte[3] = (HexNum) & 0xFF;
	
	for(uint8_t i = 0; i < 4; i++)
	{
		OLED_DisplayHexNum_1Byte(Lines, Columns, HexByte[i], Mode);
		
		if (Mode == HEX_SHOW_TYPE_INTEGRITY)
		{
			Mode = HEX_SHOW_TYPE_SIMPLE;
			
			Columns += 4;
			
			continue;
		}
		
		Columns += 2;
	}
}


/*
		@brief: ��һ���ֽ������Զ�������ʽ��ʾ.
		@parm: Lines ��ʾ��λ��(��).
		@parm: Columns ��ʾ��λ��(��).
		@parm: Num ����.
*/
void OLED_DisplayBinNum(uint8_t Lines, uint8_t Columns, uint8_t Num)
{
	char IndexArr_High[5];
	
	char IndexArr_Low[5];
	
	IndexArr_High[4] = '\0';
	
	IndexArr_Low[4] = '\0';
	
	for(uint8_t i = 0; i < 4; i++)
	{
		IndexArr_High[i] = (Num & (0x80 >> i)) ? ('1') : ('0');
	}
	
	for(uint8_t i = 0; i < 4; i++)
	{
		IndexArr_Low[i] = ((Num << 4) & (0x80 >> i)) ? ('1') : ('0');
	}
	
	OLED_DisplayStr(Lines, Columns, IndexArr_High);
	
	OLED_DisplayStr(Lines, Columns + 3, IndexArr_Low);
}


/*
		@brief: ��ȡ���ĵ�������.
		@parm: gbk  GBK����.
		@parm: chinese  ���ֽṹ��.
*/
void Get_ChineseDotMatrix(const uint8_t *gbk, OLED_ChineseFontType *chinese)
{
	// sizeof(ChineseFont) / sizeof(OLED_ChineseFontType) : ����������Ĵ�С���Ե������͵Ĵ�С���õ��ֿ������Ԫ�ظ�����
	for(uint8_t i = 0; i < sizeof(ChineseFont) / sizeof(OLED_ChineseFontType); i++)
	{
		if (ChineseFont[i].hzgbk[0] == gbk[0] && ChineseFont[i].hzgbk[1] == gbk[1])
		{
			memcpy(chinese -> font, ChineseFont[i].font, 32);

			return;
		}
	}
}


/*
		@brief: ��ʾ���������ַ�.
		@parm: Lines  ��ʾλ��(��).
		@parm: Columns  ��ʾλ��(��).
		@parm: chinese  ��ʾ�ĺ���.
*/
void OLED_DisplaySingleChinese(uint8_t Lines, uint8_t Columns, OLED_ChineseFontType *chinese)
{
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 16);
	
	for(uint8_t i = 0; i < 16; i++)  // ��ʾ�ϰ벿��.
	{
		OLED_WriteByte(chinese -> font[i]);
	}
	
	OLED_SetCursor(2 * Lines - 1, (Columns - 1) * 16);
	
	for(uint8_t j = 0; j < 16; j++)  // ��ʾ�°벿��.
	{
		OLED_WriteByte(chinese -> font[j + 16]);
	}
}


/*
		@brief: ��ʾ�����ַ���.
		@parm: Lines  ��ʾλ��(��).
		@parm: Columns  ��ʾλ��(��).
		@parm: cnstr  �����ַ���(GBK).
*/
void OLED_DisplayChinese(uint8_t Lines, uint8_t Columns, const char *cnstr)
{
	OLED_ChineseFontType chinese;

	while(*cnstr)
	{
		if ((uint8_t)*cnstr >= 0x80)  // �ж��Ƿ�Ϊ�����ַ�.
		{
			Get_ChineseDotMatrix((uint8_t *)cnstr, &chinese);

			OLED_DisplaySingleChinese(Lines, Columns, &chinese);

			cnstr += 2;

			Columns += 1;
		}

		if (Columns > 8) // ����.
		{
			Lines++;
			Columns = 1;
		}
	}
}


void struct_default_init(OLED_InitTypeDef *OLED_InitStructure)
{
	OLED_InitStructure -> AddressMode = Address_Mode_Horizontal;
	
	OLED_InitStructure -> Contrast = 0xFF;
	
	OLED_InitStructure -> DispalyOffset = 0x00;
	
	OLED_InitStructure -> EntireDis = DISABLE;
	
	OLED_InitStructure -> Inverse = DISABLE;
	
	OLED_InitStructure -> SegmentRemap = DISABLE;
	
	OLED_InitStructure -> VerticalDis = DISABLE;
	
}


void OLED_Init(OLED_InitTypeDef *OLED_InitStructure)
{
	uint32_t i, j;
	
	OLED_InitTypeDef initstructure;
	
	struct_default_init(&initstructure);
	
	if (OLED_InitStructure != NULL)
	{
		memcpy(&initstructure, OLED_InitStructure, sizeof(OLED_InitTypeDef));
	}
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
#ifdef USE_I2C_HAREWARE
	I2C_Hard_Init();
#else 
	I2C_Soft_Init();
#endif // USE_I2C_HAREWARE
	
	// �ر���ʾ.
	OLED_WriteCommand(SET_DISPLAY_OFF); 
	
	// ������ʾʱ�ӷ�Ƶ��/����Ƶ��.  1000b : ����Ƶ��; 0000 : ʱ�ӷ�Ƶϵ�� (��ΪĬ��ֵ.)
	OLED_WriteCommand_Parm(SET_DISPLAY_CLK_DIV__OSC_FREQ, 0x80);
	
	// ��·������. 0011 1111 (Ĭ��ֵ).
	OLED_WriteCommand_Parm(SET_MULTIPLEX_RATIO, 0x3F);
	
	// ��ʾƫ��. 0000 0000 (Ĭ����ƫ��).
	OLED_WriteCommand_Parm(SET_DISPLAY_OFFSET, initstructure.DispalyOffset);
	
	// ��ʾ��ʼ��.
	OLED_WriteCommand(SET_DISPALY_START_LINE0);
	
	// ����Ѱַģʽ(0x00: ˮƽѰַ).
	OLED_WriteCommand_Parm(SET_ADDRESSING_MODE, initstructure.AddressMode);
	
	// �Ƿ���ˮƽ��ת.
	if (initstructure.SegmentRemap == DISABLE)
	{
		OLED_WriteCommand(SET_SENGMENT_REMAP_OFF); 
	}
	else 
	{
		OLED_WriteCommand(SET_SENGMENT_REMAP_ON); 
	}

	// �Ƿ�����ֱ��ת.
	if (initstructure.VerticalDis == DISABLE)
	{
		OLED_WriteCommand(SET_SENGMENT_VERTICAL_NORMAL);
	}
	else
	{
		OLED_WriteCommand(SET_SENGMENT_VERTICAL_ON);
	}

	
	// COM��������(0001 0010 ΪĬ������).
	OLED_WriteCommand_Parm(SET_COMPINS_HARDWARE_CONFIG, 0x12); 
	
	// �Աȶ�����.(0x00 ~ 0xFF).
	OLED_WriteCommand_Parm(SET_CONTRAST_CONTROL, initstructure.Contrast);
	
	// Ԥ�����������.
	OLED_WriteCommand_Parm(SET_PRE_CHARGE_PERIOD, 0xF1);
	
	// ����VCOMH��ѹ����.
	OLED_WriteCommand_Parm(SET_VCOMH_DESELECT_LEVEL, 0x30);
	
	// �Ƿ����RAM��ʾ.
	if (initstructure.EntireDis == DISABLE) 	
	{
		OLED_WriteCommand(SET_ENTIRE_DISPLAY_RAM);
	}
	else 
	{
		OLED_WriteCommand(SET_ENTIRE_DISPLAY_ON);
	}

	// �Ƿ�����ʾ.
	if (initstructure.Inverse == DISABLE)
	{
		OLED_WriteCommand(SET_NORMAL_DISPLAY);		
	}
	else 
	{
		OLED_WriteCommand(SET_INVERSE_DISPLAY);
	}

	// ���ó���.
	OLED_WriteCommand_Parm(SET_CHARGE_PUMP, 0x14);
	
	// ������ʾ.
	OLED_WriteCommand(SET_DISPLAY_ON);
	
	OLED_ClearScreen();
}

