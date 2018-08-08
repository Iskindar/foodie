#include "reg52.h"
#include <string.h>
#include <intrins.h>
#include "main.h"
#include "slrc632.h"
//#include "iso14443a.h"
//#include "iso14443b.h"
#include "iso15693.h" 
#include "LCD1602.h"
#include <absacc.h>
//硬件版本号         
//unsigned char code hardmodel[12]  = {"SL601F-0512"};  

bit g_bReceOk;                                              //正确接收到上位机指令标志
bit g_bReceAA;                                              //接收到上位机发送的AA字节标志
bit g_bRc632Ok;                                             //RC632复位正常标志


unsigned int  g_cReceNum;                                   //接收到上位机的字节数
unsigned int  idata g_cCommand;                              //接收到的命令码

unsigned char idata sBuffer[64];                         //和上位机通讯时的缓冲区
unsigned char idata UID[32];
unsigned char xdata LastUID[32];
unsigned char xdata UIDFlag=0;

unsigned char lfirst=0,timeover;
unsigned long idata seltime=0;
unsigned char idata second=0;
unsigned char xdata maxcardno=0;

struct TranSciveBuffer{unsigned char MfCommand;
                       unsigned int  MfLength;
                       unsigned char MfData[64];
                      };

void delay(unsigned int i)
{
   unsigned char j;
   while(i--) for(j=0;j<200;j++);
}



void timer0()  interrupt 1
{
	TH0 = 0xF8;//0xFD;
	TL0 = 0xCD;		//重装计数初值,定时时间为1ms
	 
	seltime++;//随机数，加1
	if(seltime>999) 
	{
		 seltime=0;
		 second++;
	}

} 

void BEEPOPEN(void)
{
    sond =  1;
    cpldog();
   
}

void BEEPCLOSE(void)
{
	  sond =0; 
}

void REDON(void)
{
    redled =  0;
}

void REDOFF(void)
{
    redled =  1;
}


void main( )
{    
   unsigned char temp;
	 unsigned char flags = 0x02;
	 unsigned char xdata readtime,k,m,n;
	 unsigned char xdata initUID[8]={0};
	 unsigned char xdata flag=0,flag2=0;
	 unsigned char cardsn;
   InitializeSystem( );
   Rc632Ready( );
   PcdConfigISOType(0x31);
	 sBuffer[6]=0xAA;
	 sBuffer[7]='A';
	 sBuffer[8]=0xAA;
	 txs232(9);
	 memset(UID,0,32);
	 memset(initUID,0,8);
	 memset(LastUID,1,32);
	 REDON();
	 BEEPCLOSE();
	 timeover=0;
	 RC632_CE=0;
   
   LcdInit();  //12864初始化

   while ( 1 )
   {  
start:		 
		 if(timeover>5)
		 {
			 REDON();
			 timeover=30;
			 
		 }
		 timeover++; 	
     for(readtime=0;readtime<4;readtime++)	
     {		 
		   delay(5);
			 temp=ISO15693_Inventory16(0x16,0x00,0x00,&sBuffer[0],&sBuffer[0], &sBuffer[1]);
			 if(temp!=MI_OK)   //如果寻卡失败继续寻卡
			 {
					 temp=ISO15693_Inventory(0x16,0x00,0x00,&sBuffer[0],&sBuffer[1]);
					 if(temp!=MI_OK)   
					 {
							 memset(UID,0,32);
						   flag = 0;
							 lfirst = 0;
						   maxcardno=0;
							 goto start;
					 }
			 }          //寻卡成功
			 /*else
				  LcdInit();*/
			 //lcd_wcmd(0x01, 1);
			 //LcdInit();  //12864初始化
			 cardsn = sBuffer[0]/9;
			 if(cardsn>maxcardno)
			 {
					maxcardno = cardsn;
			 }
			 for(k=0;k<cardsn;k++)
			 {
					flag = 0;
					flag2 = 0;
					for(m=0;m<4;m++)
					{
					    if(memcmp(&sBuffer[k*9+2],UID+m*8,8)==0)
							{
								 flag = 1;
								 m=4;
							}
					}
					if(flag) continue;
							
					for(n=0;n<4;n++)
					{
							if(memcmp(UID+n*8,initUID,8)==0)
							{
									memcpy(UID+n*8,&sBuffer[k*9+2],8);
									memcpy(sBuffer,UID+n*8,8);
									flag2 =1;
									n =4;

							}
					}
					if(flag2) continue;
				}
		 }
		 if(lfirst==1) goto start;
		 memcpy(sBuffer,UID,maxcardno*8);
		 txs232(maxcardno*8);            //上传ID号
		 lfirst = 1;
		 LCD_UIDSHOW(maxcardno*8);       //12864显示
		 
//		 for(k=0;k<maxcardno;k++)      //上传内容
//		 {
//			 flags |= 0x02 << 4;
//			 temp=ISO15693_Read_sm(flags,&UID[8*k],0x00,0x01,&sBuffer[0],&sBuffer[1]);
//			 if(temp!=MI_OK) 
//			 {
//         temp=ISO15693_Read_sm(flags,&UID[8*k],0x00,0x01,&sBuffer[0],&sBuffer[1]);
//				 if(temp!=MI_OK) 
//				 {
//					 temp=ISO15693_Read_sm(flags,&UID[8*k],0x00,0x01,&sBuffer[0],&sBuffer[1]);
//				   if(temp!=MI_OK)
//					 {
//						 temp=ISO15693_Read_sm(flags,&UID[8*k],0x00,0x01,&sBuffer[0],&sBuffer[1]);
//				     if(temp!=MI_OK)
//					   {
//						    maxcardno = 0;
//				        continue;
//						 }
//					 }
//				 }
//			 }
//			 temp = sBuffer[0]+9;
//			 memcpy(sBuffer+5,&UID[8*k],8);
//			 txs232(temp);//上传读到块0 的数据以及数据的对应的ID号
//	   }
		 maxcardno = 0;
		 BEEPOPEN();
		 REDOFF();
		 timeover=0;
		 delay(100);
		 cpldog();
		 BEEPCLOSE();
	
     }
}



/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
    //TMOD &= 0x0F;
    TMOD = 0x21;
    PCON |= 0x80;
    SCON  = 0x50;
    TH1 = TL1 = BOUND115200; 
    TR1=1;
	  TR0=1;
	  ET0=1;
    P0 = P1 = P2 = P3 = 0xFF;
	  EA=1;
    BEEPOPEN(); 
}

void cpldog(void)
{
   dog=~dog;
}

/////////////////////////////////////////////////////////////////////
//初始化RC632
/////////////////////////////////////////////////////////////////////
void Rc632Ready()
{
    char status;



    status = PcdReset();
    if(status != MI_OK)
    {
        DelayMs(10);
        status = PcdReset();
    } 
    if(status != MI_OK)
    {
        DelayMs(10);
        status = PcdReset();
    } 
      
}
void txbyte(char by)
{
	TI=0;
	SBUF=by;
	while(!TI); 
	TI=0;
}	



void txs232(unsigned char len)
{
	unsigned char chk,i=len;
	unsigned char  *pi;
	unsigned char  *pa;
	chk=0;
	//EA=0;
	TI=0;
	pi=sBuffer;
	pa=sBuffer+1; 
	while(i--)
	{
		txbyte(*pi++);
	}
	i=len-1;
	while(i--)  chk^=*pa++;	 //异或校验序列号
	txbyte(chk);
	txbyte(0x0d); //包尾0d
	return ;
}



void DelayMs(unsigned int _MS)
{
    RCAP2LH = RCAP2_1ms;
    T2LH    = RCAP2_1ms;

    ET2     = 0; 	                                   // Disable timer2 interrupt
    T2CON   = 0x04;                                        // 16-bit auto-reload, clear TF2, start timer
    TR2     = 1;
    while (_MS--)
    {   while (!TF2);TF2 = 0;  }
    TR2 = 0;
}
