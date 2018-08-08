#include "reg52.h"
#include <intrins.h>
#include <string.h>
#include "main.h"
#include <absacc.h>
#include "slrc632.h"
#include "iso15693.h" 

#define FSD 64
#define Gpbase	              0x7F00                   //感应模块地址 		P2^7
#define outportb(addr,d)	XBYTE[addr]=d			 //写入addr地址值D	/		绝对地址
#define inportb(addr)		XBYTE[addr]			  ///读出addr地址值
extern struct TranSciveBuffer{unsigned char MfCommand;
                              unsigned int  MfLength;
                              unsigned char MfData[64];
                             };







unsigned char ReadRawRC(unsigned char Address)
{
	return inportb(Gpbase+Address);		   }


void  WriteRawRC(unsigned char Address, unsigned char value)
{  
	outportb(Gpbase+Address,value);
}






/////////////////////////////////////////////////////////////////////
//复位并初始化RC632
//注意:RC500上电后应延时500ms才能可靠初始化
/////////////////////////////////////////////////////////////////////
char PcdReset()
{
   char status = MI_OK;
   char n = 0xFF;
   unsigned int i = 3000;

   RC632_CE=0;
   RC632_RST=0;
   DelayMs(100);
   RC632_RST=1;
   DelayMs(100);
   RC632_RST=0;
   DelayMs(100);

   while (i!=0 && n)
   {
      n = ReadRawRC(RegCommand);
      i--;
   }

   if (i != 0)
   {
      WriteRawRC(RegPage,0x80);
      n = 0x80;
      while ( (i!=0) && (n&0x80) )
      {
          n = ReadRawRC(RegCommand);
          i--;
      }
      if (i==0 || (n&0xFF))
      {   status = MI_RESETERR;   }
   }
   else
   {    status = MI_RESETERR;     }
   
   if (status == MI_OK)
   {    WriteRawRC(RegPage,0x0);  }
   
   return status;
}

//////////////////////////////////////////////////////////////////////
//设置RC632的工作方式 
//////////////////////////////////////////////////////////////////////
char PcdConfigISOType(unsigned char type)
{
    if (type == '1')
   {
       ClearBitMask(RegControl,0x08);

       WriteRawRC(RegTxControl,0x48);
       WriteRawRC(RegCwConductance,0x3F);
       WriteRawRC(RegModConductance,0x05);       // must be measured for 15% Modulation Index
       WriteRawRC(RegCoderControl,0x2F);         // 52.97kHz, 1-256 coding
       WriteRawRC(RegModWidth,0x3F);             // Modulation width: 9.44us
       WriteRawRC(RegModWidthSOF,0x3F);          // Standard mode: 9.44us
       WriteRawRC(RegTypeBFraming,0x00);
       
       WriteRawRC(RegRxControl1,0x88);
       WriteRawRC(RegDecoderControl,0x34);
       WriteRawRC(RegBitPhase,0xcd);
       WriteRawRC(RegRxThreshold,0x88);
       WriteRawRC(RegBPSKDemControl,0);
       WriteRawRC(RegRxControl2,0x41);
       WriteRawRC(RegClockQControl,0x00);

       WriteRawRC(RegRxWait,0x08);
       WriteRawRC(RegChannelRedundancy,0x2C);    // Enable CRC TX/RX
       WriteRawRC(RegCRCPresetLSB,0xFF);
       WriteRawRC(RegCRCPresetMSB,0xFF);
       WriteRawRC(RegTimeSlotPeriod,0x00);
       WriteRawRC(RegMfOutSelect,0x00);         // enable SIGOUT = envelope
       WriteRawRC(RFU27,0x00);   	      

       WriteRawRC(RegFIFOLevel,0x38);
       WriteRawRC(RegTimerClock,0x0B);
       WriteRawRC(RegTimerReload,0x00);
       WriteRawRC(RegTimerControl,0x02);         // TStopRxEnd=0,TStopRxBeg=1,
       WriteRawRC(RFU2E,0x00);
       WriteRawRC(RFU2F,0x00);
       DelayMs(1);
       PcdAntennaOn();
	}
  
   else{ return -1; }
   return MI_OK;
}


/////////////////////////////////////////////////////////////////////
//置RC632寄存器位
//input:reg=寄存器地址
//      mask=置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
   char tmp = 0x0;
   tmp = ReadRawRC(reg);
   WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//清RC632寄存器位
//input:reg=寄存器地址
//      mask=清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
   char tmp = 0x0;
   tmp = ReadRawRC(reg);
   WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/////////////////////////////////////////////////////////////////////
//设置RC632定时器
//input:tmolength=设置值
/////////////////////////////////////////////////////////////////////
/*
void PcdSetTmo(unsigned char tmoLength)
{
   switch(tmoLength)
   {  
      case 0:                         // (0.302 ms) FWI=0
         WriteRawRC(RegTimerClock,0x07); // TAutoRestart=0,TPrescale=128
         WriteRawRC(RegTimerReload,0x21);// TReloadVal = 'h21 =33(dec) 
         break;
      case 1:                         // (0.604 ms) FWI=1
         WriteRawRC(RegTimerClock,0x07); // TAutoRestart=0,TPrescale=128
         WriteRawRC(RegTimerReload,0x41);// TReloadVal = 'h41 =65(dec) 
         break;
      case 2:                         // (1.208 ms) FWI=2
         WriteRawRC(RegTimerClock,0x07); // TAutoRestart=0,TPrescale=128
         WriteRawRC(RegTimerReload,0x81);// TReloadVal = 'h81 =129(dec) 
         break;
      case 3:                         // (2.416 ms) FWI=3
         WriteRawRC(RegTimerClock,0x09); // TAutoRestart=0,TPrescale=4*128
         WriteRawRC(RegTimerReload,0x41);// TReloadVal = 'h41 =65(dec) 
         break;
      case 4:                         // (4.833 ms) FWI=4
         WriteRawRC(RegTimerClock,0x09); // TAutoRestart=0,TPrescale=4*128
         WriteRawRC(RegTimerReload,0x81);// TReloadVal = 'h81 =129(dec) 
         break;
      case 5:                         // (9.666 ms) FWI=5
         WriteRawRC(RegTimerClock,0x0B); // TAutoRestart=0,TPrescale=16*128
         WriteRawRC(RegTimerReload,0x41);// TReloadVal = 'h41 =65(dec) 
         break;
      case 6:                         // (19.33 ms) FWI=6
         WriteRawRC(RegTimerClock,0x0B); // TAutoRestart=0,TPrescale=16*128
         WriteRawRC(RegTimerReload,0x81);// TReloadVal = 'h81 =129(dec) 
         break;
      case 7:                         // (38.66 ms) FWI=7
         WriteRawRC(RegTimerClock,0x0D); // TAutoRestart=0,TPrescale=64*128
         WriteRawRC(RegTimerReload,0x41);// TReloadVal = 'h41 =65(dec) 
         break;
      case 8:                         // (77.32 ms) FWI=8
         WriteRawRC(RegTimerClock,0x0D); // TAutoRestart=0,TPrescale=64*128
         WriteRawRC(RegTimerReload,0x81);// TReloadVal = 'h81 =129(dec) 
         break;
      case 9:                         // (154.6 ms) FWI=9
         WriteRawRC(RegTimerClock,0x0F); // TAutoRestart=0,TPrescale=256*128
         WriteRawRC(RegTimerReload,0x41);// TReloadVal = 'h41 =65(dec) 
         break;
      case 10:                        // (309.3 ms) FWI=10
         WriteRawRC(RegTimerClock,0x0F); // TAutoRestart=0,TPrescale=256*128
         WriteRawRC(RegTimerReload,0x81);// TReloadVal = 'h81 =129(dec) 
         break;
      case 11:                        // (618.6 ms) FWI=11
         WriteRawRC(RegTimerClock,0x13); // TAutoRestart=0,TPrescale=4096*128
         WriteRawRC(RegTimerReload,0x11);// TReloadVal = 'h21 =17(dec) 
         break;
      case 12:                        // (1.2371 s) FWI=12
         WriteRawRC(RegTimerClock,0x13); // TAutoRestart=0,TPrescale=4096*128
         WriteRawRC(RegTimerReload,0x21);// TReloadVal = 'h41 =33(dec) 
         break;
      case 13:                        // (2.4742 s) FWI=13
         WriteRawRC(RegTimerClock,0x13); // TAutoRestart=0,TPrescale=4096*128
         WriteRawRC(RegTimerReload,0x41);// TReloadVal = 'h81 =65(dec) 
         break;
      case 14:                        // (4.9485 s) FWI=14
         WriteRawRC(RegTimerClock,0x13); // TAutoRestart=0,TPrescale=4096*128
         WriteRawRC(RegTimerReload,0x81);// TReloadVal = 'h81 =129(dec) 
         break;
      case 15:                        // (4.9485 s) FWI=14
         WriteRawRC(RegTimerClock,0x9); // TAutoRestart=0,TPrescale=4096*128
         WriteRawRC(RegTimerReload,0x0ff);// TReloadVal = 'h81 =129(dec) 
         break;
      default:                       // 
         WriteRawRC(RegTimerClock,0x19); // TAutoRestart=0,TPrescale=128
         WriteRawRC(RegTimerReload,tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
         break;
   }     
   WriteRawRC(RegTimerControl,0X06);
}
  */

/////////////////////////////////////////////////////////////////////
//  通过RC632和ISO14443卡通讯
//input: pi->MfCommand = RC632命令字
//       pi->MfLength  = 发送的数据长度
//       pi->MfData[]  = 发送数据
//output:status        = 错误字
//       pi->MfLength  = 接收的数据BIT长度
//       pi->MfData[]  = 接收数据
/////////////////////////////////////////////////////////////////////
char ISO15693_Transceive(struct TranSciveBuffer *pi)
{
   bit recebyte = 0;
   char status  = MI_COM_ERR;
   unsigned char n,waitFor,TimerReload;
   unsigned int i;
   switch (pi->MfCommand)
   {
      case PCD_TRANSMIT:
         waitFor = 0x10;
         break;
      default:
         waitFor = 0x28;
         recebyte=1;
         break;
   }
   switch (pi->MfData[1])
   {
        case ISO15693_STAY_QUIET:
           TimerReload = 0x04;    // 2048/fc => 0x01 = 151 us 
           break;
        case ISO15693_SELECT:
        case ISO15693_RESET_TO_READY:
		   TimerReload = 0x0F;    // 2048/fc => 0x01 = 151 us 
           break;
        case ISO15693_LOCK_AFI:
        case ISO15693_LOCK_DSFID:
        case ISO15693_LOCK_BLOCK:
        case ISO15693_WRITE_SINGLE_BLOCK:
        case ISO15693_WRITE_MULTIPLE_BLOCKS:
        case ISO15693_WRITE_AFI:
        case ISO15693_WRITE_DSFID:
           TimerReload = 0x29;    // 2048/fc => 0x01 = 151 us 
           break;
        case ISO15693_READ_SINGLE_BLOCK:
           TimerReload = 0x17;    // 2048/fc => 0x01 = 151 us 
           break;
        case ISO15693_INVENTORY:
           TimerReload = 0x1F;    // 2048/fc => 0x01 = 151 us 
           break;
        case ISO15693_GET_SYSTEM_INFO:
           TimerReload = 0x25;    // 2048/fc => 0x01 = 151 us 
           break;
        case ISO15693_GET_MULTIPLE_BLOCK_SECURITY:
           TimerReload = 0x40;//(0x04+(cmd[cmdlen-1]+0x01))*0x02 + 0x04 + 0x01;    // 2048/fc => 0x01 = 151 us 
           break;														  // (0x04 + cmd[cmdlen - 1])*0x02 -> Time for all receivedBytes, 0x04 -> Time bevore and after response, 0x01 additional
        case ISO15693_READ_MULTIPLE_BLOCKS:
           TimerReload = 0x40;//(0x04+0x04*(cmd[cmdlen-1]+0x01))*0x02 + 0x04 + 0x01;    // 2048/fc => 0x01 = 151 us 
           break;
        default:
           TimerReload = 0x86; // 2048/fc => 0x01 = 151 us 
           break;
   }
   
   WriteRawRC(RegPage,0x00);
   WriteRawRC(RegFIFOLevel,0x1A);	 
       
   SetBitMask(RegChannelRedundancy, 0x04);
   WriteRawRC(RegTimerReload, TimerReload);
   WriteRawRC(RegTimerControl, 0x06);

   SetBitMask(RegControl,0x01);
     
   WriteRawRC(RegCommand, 0x00);
   WriteRawRC(RegInterruptEn, 0x81);	 
   WriteRawRC(RegInterruptRq, 0x3F);
   WriteRawRC(RegInterruptEn, 0x38 | 0x80);
       
   for (i=0; i<pi->MfLength; i++)
   {
       WriteRawRC(RegFIFOData,pi->MfData[i]);
   }

   WriteRawRC(RegCommand, PCD_TRANSCEIVE);	  // start to send command to label

   i = 0x6500;
   do
   {
       n = ReadRawRC(RegInterruptRq);
       i--;
   }
   while ( (i!=0) && !(n&waitFor) );
      
   if (!recebyte)
   {
       if (n & 0x10) 
       {    status = MI_OK;    }
       WriteRawRC(RegInterruptEn,0x10);
       WriteRawRC(RegTimerControl,0x00);
       WriteRawRC(RegCommand,PCD_IDLE); 
       ClearBitMask(RegCoderControl, 0x80);
       return status;
   }
   else
   {
      if ((i!=0) && (n&0x08))
      {
         if ( !(ReadRawRC(RegErrorFlag) & 0x0C) )
         {
             n = ReadRawRC(RegFIFOLength);
             pi->MfLength = n*8;
             for (i=0; i<n; i++)
             {
                 pi->MfData[i] = ReadRawRC(RegFIFOData);
             }
             if (pi->MfData[0] == 0)
             {   status = MI_OK;   }
         }
      }

      WriteRawRC(RegInterruptEn,0x10);
      WriteRawRC(RegTimerControl,0x00);
      WriteRawRC(RegCommand,PCD_IDLE); 
      ClearBitMask(RegCoderControl, 0x80);
      return status;
   }
}

/*
/////////////////////////////////////////////////////////////////////
//读RC632EEPROM
//input :startaddr=EEPROM地址（低位在前）
//       length=读字节数
//output:readdata=读出的数据
/////////////////////////////////////////////////////////////////////
char PcdReadE2(unsigned int startaddr,unsigned char length,unsigned char *readdata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    MfComData.MfCommand = PCD_READE2;
    MfComData.MfLength  = 3;
    MfComData.MfData[0] = startaddr&0xFF;
    MfComData.MfData[1] = (startaddr >> 8) & 0xFF;
    MfComData.MfData[2] = length;

    status = PcdComTransceive(pi);

    if (status == MI_OK)
    {   memcpy(readdata, &MfComData.MfData[0], length);    }
    return status;
}

/////////////////////////////////////////////////////////////////////
//写RC632EEPROM
//input :startaddr=EEPROM地址（低位在前）
//       length=写字节数
//       writedata=要写入的数据
/////////////////////////////////////////////////////////////////////
char PcdWriteE2(unsigned int startaddr,unsigned char length,unsigned char *writedata)
{
    char status;
    struct TranSciveBuffer MfComData;    
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    MfComData.MfCommand = PCD_WRITEE2;
    MfComData.MfLength  = length+2;
    MfComData.MfData[0] = startaddr&0xFF;
    MfComData.MfData[1] = (startaddr >> 8) & 0xFF;
    memcpy(&MfComData.MfData[2], writedata, length);    

    status = PcdComTransceive(pi);
    return status;
}
*/

/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
char PcdAntennaOn()
{
    unsigned char i;
    i = ReadRawRC(RegTxControl);
    if (i & 0x03)
    {   return MI_OK;	}
    else
    {
        SetBitMask(RegTxControl, 0x03);
        return MI_OK;
    }
	
    
}

/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
/*char PcdAntennaOff()
{
    ClearBitMask(RegTxControl, 0x03);
    return MI_OK;
}

*/
