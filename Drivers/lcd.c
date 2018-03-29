#include "stm32f10x.h"
                                                
#define Driver_Code_Read                        0x00
#define Ili9325_Id_Code                         0x9325

#define Driver_Output_Control                   0x01
#define NoSSAndSM                               0x0000 
#define SS                                      0x0100//S720 to S1
#define SM                                      0x0400//S720 to S1

#define LCD_Driving_Wave_Control                0x02
#define BOrC_FrameOrField_inversion             0x0400
#define BOrC_Line_inversion                     0x0600
#define EOR_Line_inversion                      0x0700

#define Entry_Mode                              0x03//0x1030
#define AM                                      0x0008
#define IOrD11                                  0x0030//Ë®Æ½ºÍ´¹Ö±×Ô¶¯Ôö¼Ó
#define ORG                                     0x0080//      
#define BGR                                     0x1000//Swap the RGB data to BGR in writing into GRAM. 
#define TRI                                     0x8000//When TRI = “1”, data are transferred to the internal RAM in 8-bit x 3 transfers mode via the 8-bit interface
#define DFM                                     0x4000//Set the mode of transferring data to the internal RAM when TRI = “1”. 

#define Resize_Control                          0x04
/*
RSZ[1:0] Sets the resizing factor.  When the RSZ bits are set for resizing, the ILI9325 writes the data according to the resizing factor so that the original image is displayed in horizontal and vertical dimensions, which are contracted 
a-Si TFT LCD Single Chip Driver  240RGBx320 Resolution and 262K color ILI9325 according to the factor respectively. See “Resizing function”. 
*/
#define Resize_Control_Default                  0x0000 

#define Display_Control1                        0x07
#define Display_Control2                        0x08
#define Display_Control3                        0x09
#define Display_Control4                        0x0A
#define RGB_Display_Interface_Control1          0x0C
#define Frame_Maker_Position                    0x0D
#define RGB_Display_Interface_Control2          0x0F

#define Power_Control1                          0x10              
#define Power_Control2                          0x11              
#define Power_Control3                          0x12             
#define Power_Control4                          0x13
#define Horizontal_GRAM_Address_Set             0x20
#define Vertical_GRAM_Address_Set               0x21
#define Write_Data_to_GRAM                      0x22//RAM write data (WD17-0) / read data (RD17-0) bits are transferred via different data bus lines according to the selected interfaces. 
#define Power_Control7                          0x29
#define Frame_Rate_and_Color_Control            0x2b

#define Gamma_Control1                          0x30
#define Gamma_Control2                          0x31
#define Gamma_Control3                          0x32
#define Gamma_Control4                          0x35
#define Gamma_Control5                          0x36
#define Gamma_Control6                          0x37
#define Gamma_Control7                          0x38
#define Gamma_Control8                          0x39
#define Gamma_Control9                          0x3C
#define Gamma_Control10                         0x3D

#define Horizontal_Address_Start_Position       0x50
#define Horizontal_Address_End_Position         0x51
#define Vertical_Address_Start_Position         0x52
#define Vertical_Address_End_Position           0x53

#define Driver_Output_Control2                  0x60
#define Base_Image_Display_Control              0x61
#define Vertical_Scroll_Control                 0x6A

#define Partial_Image_1_Display_Position        0x80
#define Partial_Image_1_Area_Start_Line         0x81
#define Partial_Image_1_Area_End_Line           0x82
#define Partial_Image_2_Display_Position        0x83
#define Partial_Image_2_Area_Start_Line         0x84
#define Partial_Image_2_Area_End_Line           0x85

#define Panel_Interface_Control_1               0x90
#define Panel_Interface_Control_2               0x92
#define Panel_Interface_Control_3               0x95
								
#define OTP_VCM_Programming_Control							0xA1	
#define OTP_PGM_CNT_Addr                        0xA2
#define OTP_Programming_ID_Key                  0xA5

void LcdGpioInit();
int LcdInit();
void LcdWriteReg(uint16_t Addr,uint16_t Data);
uint16_t LcdReadReg(uint16_t Addr);
void LcdDataGpioSetInputMode();
void LcdDataGpioSetOutputMode();
void Delay_LCD(uint16_t n);
/*******************************************************************************
* Function Name  : Delay_LCD
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_LCD(u16 n)
{
	u16 i,j;
	for (i = 0;i<n;++i)
		for(j=0;j<3000;++j);
}

int LcdInit()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_All);
	if(1 || LcdReadReg(Driver_Code_Read)==Ili9325_Id_Code){//&& LcdReadReg(OTP_PGM_CNT_Addr)==0x11
		
		
		LcdWriteReg(Driver_Output_Control  , NoSSAndSM); // set SS and SM bit		  //0x0100
		LcdWriteReg(LCD_Driving_Wave_Control, EOR_Line_inversion); // set 1 line inversion
		LcdWriteReg(Entry_Mode  , BGR|IOrD11);   // set GRAM write direction and BGR=1.
		LcdWriteReg(Resize_Control  , 0x0000);   // Resize register
		LcdWriteReg(Display_Control2  , 0x0207);   // set the back porch and front porch
		LcdWriteReg(Display_Control3  , 0x0000);   // set non-display area refresh cycle ISC[3:0]
		LcdWriteReg(Display_Control4 , 0x0000);   // FMARK function
		LcdWriteReg(RGB_Display_Interface_Control1 , 0x0000); // RGB interface setting
		LcdWriteReg(Frame_Maker_Position , 0x0000);   // Frame marker Position
		LcdWriteReg(RGB_Display_Interface_Control2 , 0x0000); // RGB interface polarity
		/**************Power On sequence ****************/
		//Diisplay Off Sequence
		LcdWriteReg(Power_Control1 , 0x0000);   // SAP, BT[3:0], AP, DSTB, SLP, STB
		LcdWriteReg(Power_Control2 , 0x0007);   // DC1[2:0], DC0[2:0], VC[2:0]
		LcdWriteReg(Power_Control3 , 0x0000); // VREG1OUT voltage
		LcdWriteReg(Power_Control4 , 0x0000);   // VDV[4:0] for VCOM amplitude
		Delay_LCD(1000);                    // Delay 200 MS , Dis-charge capacitor power voltage
		
		LcdWriteReg(Power_Control1 , 0x1690);   // SAP, BT[3:0], AP, DSTB, SLP, STB
		LcdWriteReg(Power_Control2 , 0x0227); // R11H=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
		Delay_LCD(50);      // Delay XXms
		LcdWriteReg(Power_Control3 , 0x001D); // External reference voltage= Vci;
		Delay_LCD(50);      // Delay XXms
		LcdWriteReg(Power_Control4 , 0x0800); // R13H=1D00 when R12H=009D;VDV[4:0] for VCOM amplitude
		
		LcdWriteReg(Power_Control7 , 0x0014); // R29H=0013 when R12H=009D;VCM[5:0] for VCOMH
		LcdWriteReg(Frame_Rate_and_Color_Control , 0x000B);   // Frame Rate = 96Hz
		Delay_LCD(50);      // Delay XXms
		LcdWriteReg(Horizontal_GRAM_Address_Set , 0x0000); // GRAM horizontal Address
		LcdWriteReg(Vertical_GRAM_Address_Set , 0x0000); // GRAM Vertical Address
		/* ----------- Adjust the Gamma Curve ---------- */
		LcdWriteReg(Gamma_Control1 , 0x0007);
		LcdWriteReg(Gamma_Control2 , 0x0707);
		LcdWriteReg(Gamma_Control3 , 0x0006);
		LcdWriteReg(Gamma_Control4 , 0x0704);
		LcdWriteReg(Gamma_Control5 , 0x1F04);
		LcdWriteReg(Gamma_Control6 , 0x0004);
		LcdWriteReg(Gamma_Control7 , 0x0000);
		LcdWriteReg(Gamma_Control8 , 0x0706);
		LcdWriteReg(Gamma_Control9 , 0x0701);
		LcdWriteReg(Gamma_Control10 , 0x000F);
		/* ------------------ Set GRAM area --------------- */
		LcdWriteReg(Horizontal_Address_Start_Position , 0x0000);   // Horizontal GRAM Start Address
		LcdWriteReg(Horizontal_Address_End_Position , 0x00EF);   // Horizontal GRAM End Address
		LcdWriteReg(Vertical_Address_Start_Position , 0x0000); // Vertical GRAM Start Address
		LcdWriteReg(Vertical_Address_End_Position , 0x013F); // Vertical GRAM Start Address
		LcdWriteReg(Driver_Output_Control2 , 0x2700); // Gate Scan Line		  0xA700
		LcdWriteReg(Base_Image_Display_Control , 0x0001); // NDL,VLE, REV
		LcdWriteReg(Vertical_Scroll_Control, 0x0000); // set scrolling line
		/* -------------- Partial Display Control --------- */
		LcdWriteReg(Partial_Image_1_Display_Position, 0x0000);   
		LcdWriteReg(Partial_Image_1_Area_Start_Line, 0x0000);
		LcdWriteReg(Partial_Image_1_Area_End_Line, 0x0000);
		LcdWriteReg(Partial_Image_2_Display_Position, 0x0000);
		LcdWriteReg(Partial_Image_2_Area_Start_Line, 0x0000);
		LcdWriteReg(Partial_Image_2_Area_End_Line, 0x0000);
		/* -------------- Panel Control ------------------- */
		LcdWriteReg(Panel_Interface_Control_1, 0x0010);
		LcdWriteReg(Panel_Interface_Control_2, 0x0000);
		//LcdWriteReg(R147, 0x0003);//9325Ã»ÓÐÕâ¸ö¼Ä´æÆ÷£¬¿ÉÄÜÊÇ9328µÄ
		LcdWriteReg(Panel_Interface_Control_3, 0x0110);
		//LcdWriteReg(R151, 0x0000);
		//LcdWriteReg(R152, 0x0000);
			 /* Set GRAM write direction and BGR = 1 */
			 /* I/D=01 (Horizontal : increment, Vertical : decrement) */
			 /* AM=1 (address is updated in vertical writing direction) */
		LcdWriteReg(Entry_Mode  , 0x01018);  //0x1018
		
		LcdWriteReg(Display_Control1  , 0x0173); // 262K color and display ON
	}
}

void LcdGpioInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//Ê¹ÄÜGPIOBÊ±ÖÓ
	//ÒòÎªGPIOB8,9ÊÇ¸´ÓÃ¶Ë¿Ú£¬ËùÒÔÒª¹Ø±Õ¸´ÓÃ¹¦ÄÜ¡£
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,DISABLE);//¹Ø±ÕTIM4µÄ¸´ÓÃ¹ÜµÀÕ¼ÓÃ
	//³õÊ¼»¯GPIOB9(LCD_CS)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//³õÊ¼»¯GPIOB8(LCD_RS)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//ÒòÎªGPIOB5ÊÇ¸´ÓÃ¶Ë¿Ú£¬ËùÒÔÒª¹Ø±Õ¸´ÓÃ¹¦ÄÜ¡£
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,DISABLE);//¹Ø±ÕI2C1µÄ¸´ÓÃ¹ÜµÀÕ¼ÓÃ
	//³õÊ¼»¯GPIOB5(LCD_WR)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//ÒòÎªGPIOB10ÊÇ¸´ÓÃ¶Ë¿Ú£¬ËùÒÔÒª¹Ø±Õ¸´ÓÃ¹¦ÄÜ¡£
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,DISABLE);//¹Ø±ÕUSART3µÄ¸´ÓÃ¹ÜµÀÕ¼ÓÃ
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,DISABLE);//¹Ø±ÕUSART3µÄ¸´ÓÃ¹ÜµÀÕ¼ÓÃ
	//³õÊ¼»¯GPIOB10(LCD_RD)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//Ê¹ÄÜGPIOCÊ±ÖÓ
	//³õÊ¼»¯GPIOC0-15(LCD_DATA)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}
void LcdDataGpioSetInputMode()
{
	//³õÊ¼»¯GPIOC0-15(LCD_DATA)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}
void LcdDataGpioSetOutputMode()
{
	//³õÊ¼»¯GPIOC0-15(LCD_DATA)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}
void Lcd_Cs_SetBit()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
}
void Lcd_Cs_ResetBit()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}
void Lcd_Rs_SetBit()//Ñ¡ÔñÊý¾Ý¼Ä´æÆ÷
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}
void Lcd_Rs_ResetBit()//Ñ¡Ôñ×´Ì¬¼Ä´æÆ÷
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}
void Lcd_Wr_SetBit()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
void Lcd_Wr_ResetBit()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
}
void Lcd_Rd_SetBit()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
void Lcd_Rd_ResetBit()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}
void LcdWriteReg(uint16_t Addr,uint16_t Data)
{
	Lcd_Cs_ResetBit();//Ê¹ÄÜÆ¬Ñ¡
	Lcd_Rs_ResetBit();//Ñ¡Ôñ×´Ì¬¼Ä´æÆ÷
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
	Lcd_Wr_ResetBit();//Ê¹ÄÜÐ´Èë
	LcdDataGpioSetOutputMode();
	GPIO_Write(GPIOC, Addr);//Ð´ÈëµØÖ·
	Lcd_Wr_SetBit();//½ûÖ¹Ð´Èë,·ÀÖ¹¸øÊý¾Ý¼Ä´æÆ÷Ð´ÈëÁËµØÖ·Öµ
	Lcd_Rs_SetBit();//Ñ¡ÔñÊý¾Ý¼Ä´æÆ÷
	GPIO_Write(GPIOC, Data);//Ð´ÈëÊý¾Ý
	Lcd_Wr_SetBit();//½ûÖ¹Ð´Èë
	//²Ù×÷Íê³É£¬È«²¿Òý½ÅÀ­¸ß
	Lcd_Cs_SetBit();//¹Ø±ÕÆ¬Ñ¡
	Lcd_Rs_SetBit();//À­¸ß
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
}
uint16_t LcdReadReg(uint16_t Addr)
{
	uint16_t TempData=0;
	Lcd_Cs_ResetBit();//Ê¹ÄÜÆ¬Ñ¡
	Lcd_Rs_ResetBit();//Ñ¡Ôñ×´Ì¬¼Ä´æÆ÷
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
	Lcd_Wr_ResetBit();//Ê¹ÄÜÐ´Èë
	LcdDataGpioSetOutputMode();
	GPIO_Write(GPIOC, Addr);//Ð´ÈëµØÖ·
	Lcd_Wr_SetBit();//½ûÖ¹Ð´Èë,·ÀÖ¹¸øÊý¾Ý¼Ä´æÆ÷Ð´ÈëÁËµØÖ·Öµ
	Lcd_Rs_SetBit();//Ñ¡ÔñÊý¾Ý¼Ä´æÆ÷
	Lcd_Rd_ResetBit();//Ê¹ÄÜ¶ÁÈ¡
	LcdDataGpioSetInputMode();
	TempData=GPIO_ReadOutputData(GPIOC);//¶ÁÈ¡Êý¾Ý
	Lcd_Wr_SetBit();//½ûÖ¹Ð´Èë
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈ¡
	//²Ù×÷Íê³É£¬È«²¿Òý½ÅÀ­¸ß
	Lcd_Cs_SetBit();//¹Ø±ÕÆ¬Ñ¡
	Lcd_Rs_SetBit();//À­¸ß
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
	return TempData;
}
void LcdWriteRamReady()
{
	uint16_t Addr=Write_Data_to_GRAM;
	uint16_t TempData=0;
	Lcd_Cs_ResetBit();//Ê¹ÄÜÆ¬Ñ¡
	Lcd_Rs_ResetBit();//Ñ¡Ôñ×´Ì¬¼Ä´æÆ÷
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
	Lcd_Wr_ResetBit();//Ê¹ÄÜÐ´Èë
	LcdDataGpioSetOutputMode();
	GPIO_Write(GPIOC, Addr);//Ð´ÈëµØÖ·
	Lcd_Wr_SetBit();//½ûÖ¹Ð´Èë
	//²Ù×÷Íê³É£¬È«²¿Òý½ÅÀ­¸ß
	Lcd_Cs_SetBit();//¹Ø±ÕÆ¬Ñ¡
	Lcd_Rs_SetBit();//À­¸ß
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
}
void LcdWriteRam(uint16_t Data)
{
	Lcd_Cs_ResetBit();//Ê¹ÄÜÆ¬Ñ¡
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
	Lcd_Wr_ResetBit();//Ê¹ÄÜÐ´Èë
	LcdDataGpioSetOutputMode();
	Lcd_Rs_SetBit();//Ñ¡ÔñÊý¾Ý¼Ä´æÆ÷
	GPIO_Write(GPIOC, Data);//Ð´ÈëÊý¾Ý
	Lcd_Wr_SetBit();//½ûÖ¹Ð´Èë
	//²Ù×÷Íê³É£¬È«²¿Òý½ÅÀ­¸ß
	Lcd_Cs_SetBit();//¹Ø±ÕÆ¬Ñ¡
	Lcd_Rs_SetBit();//À­¸ß
	Lcd_Rd_SetBit();//½ûÖ¹¶ÁÈë
}