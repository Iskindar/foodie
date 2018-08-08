//指定PCD接收缓冲值
#ifndef FSDI
#define FSDI 4
#endif
/////////////////////////////////////////////////////////////////////
//89C52特殊寄存器定义
/////////////////////////////////////////////////////////////////////
sfr16    RCAP2LH    =    0xCA;
sfr16    T2LH       =    0xCC;



sbit    sond     		=P3^3;
sbit    dog   			=P1^5;
sbit    redled			=P1^1;		
sbit    blueled			=P1^0;
sbit    RC632_CE		=P2^7;
sbit    RC632_RST    	=P3^4;
sbit    BUSY            =P0^7;

sbit    LCDRS           =P2^0;
sbit    LCDRW           =P2^1;
sbit    LCDEN           =P2^2;
sbit    LCDLIGHT        =P2^6;
sbit    LCDRST          =P2^5;


/////////////////////////////////////////////////////////////////////
//RC500的EEPROM定义
/////////////////////////////////////////////////////////////////////
#define  ICDEV_RC500_ADR 0x30
/////////////////////////////////////////////////////////////////////
//延时常数及波特率定义


#define OSC_FREQ                22118400L	 
#define BOUND115200             256 - (OSC_FREQ/192L)/115200L   // 255
#define BOUND57600              256 - (OSC_FREQ/192L)/57600L    // 254
#define BOUND38400              256 - (OSC_FREQ/192L)/38400L    // 253
#define BOUND28800              256 - (OSC_FREQ/192L)/28800L    // 252
#define BOUND19200              256 - (OSC_FREQ/192L)/19200L    // 250
#define BOUND14400              256 - (OSC_FREQ/192L)/14400L    // 248
#define BOUND9600               256 - (OSC_FREQ/192L)/9600L     // 244
#define BOUND4800               256 - 24
#define RCAP2_50us              65536L - OSC_FREQ/240417L
#define RCAP2_1ms               65536L - OSC_FREQ/12021L
#define RCAP2_10ms              65536L - OSC_FREQ/1200L
#define RCAP2_1s                65536L - OSC_FREQ/12L
/////////////////////////////////////////////////////////////////////
//和上位机的通讯错误代码定义
/////////////////////////////////////////////////////////////////////
#define  FAULT10  10                                      //通用错误
#define  FAULT11  11                                      //不支持该命令
#define  FAULT12  12                                      //命令参数错
#define  FAULT13  13                                      //无卡
#define  FAULT14  14                                      //射频基站损坏

#define  FAULT20  20                                      //寻卡
#define  FAULT21  21                                      //卡复位失败
#define  FAULT22  22                                      //密码验证失败
#define  FAULT23  23                                      //读卡失败
#define  FAULT24  24                                      //写卡失败

/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem();  
void BEEPOPEN(void);
void BEEPCLOSE(void);                                 
void Rc632Ready();                                         

//void AnswerOk(unsigned char *answerdata,unsigned int answernum);          
//void AnswerErr(char faultcode);                            
void DelayMs(unsigned int j);                                      
void Delay_50us(unsigned char j);                                  

//void ComSetBound();
//void ComGetHardModel();
char ComM632PcdConfigISOType(unsigned char CardType);
//void ComPcdAntenna(unsigned char Command);


void cpldog();
void txs232(unsigned char len);
void txbyte(char by);
void ComISO15693_Inventory16();
void delay(unsigned int i);

extern unsigned char idata sBuffer[];
extern unsigned char idata second;
extern unsigned char idata UID[32];
extern unsigned char xdata UIDFlag;

	



