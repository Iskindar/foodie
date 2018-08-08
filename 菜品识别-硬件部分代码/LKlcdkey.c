#include   "AT89X52.H"
#include <absacc.h>
#include <string.h>
#include <intrins.h>
#include "main.h"
#include "slrc632.h"
#include "iso14443a.h"
#include "iso14443b.h"
#include "LKlcdkey.h"


#define	NOP;	_nop_();  _nop_(); _nop_();	_nop_();
#define SOMENOP;	_nop_();_nop_();_nop_();_nop_();//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
#define	EXTGRPHFORMT		0X3E
#define	NORGRPHFORMT		0X3A
#define	NORCHARFORMT		0X38





void   CheckBusy(void)
{
    LCDRS = 0;
    BUSY = 1;
    LCDRW = 1;
    LCDEN = 1;
    //while(BUSY);
    LCDEN = 0;
}



void WriteIns(unsigned char instru)
{
    //CheckBusy();
    LCDRS = 0;
    NOP;
    LCDRW = 0;
    NOP;
    LCDEN = 1;
    NOP;
    P0 = instru;
    NOP;
    LCDEN = 0;
    P0 = 0XFF;
    return ;
}

void   pWriteIns(unsigned char insdat)
{
    CheckBusy();
    LCDRS = 0;
    NOP;
    LCDRW = 0;
    NOP;
    LCDEN = 1;
    NOP;
    P0 = insdat;
    NOP;
    LCDEN = 0;
    NOP;
    P0 = 0XFF;
    return ;
}



void pWriteData(unsigned char cdata)
{
    CheckBusy();
    LCDRS = 1;
    NOP;
    LCDRW = 0;
    NOP;
    LCDEN = 1;
    NOP;
    P0 = cdata;
    NOP;
    LCDEN = 0;
    NOP;
    P0 = 0XFF;
    return ;
}
/*

void LcdInit(void)
{

WriteIns(NORCHARFORMT);
WriteIns(6);
cpldog();
WriteIns(1);
WriteIns(0x0c);
}




void   CheckBusy(void)
{
LCDRS=0;
SOMENOP;
BUSY=1;
SOMENOP;
LCDRW=1;
SOMENOP;
LCDEN=1;
SOMENOP;
while(BUSY);
LCDEN=0;
SOMENOP;
}


void   pWriteIns(unsigned char insdat)
{
//CheckBusy();
LCDEN=1;
SOMENOP;
LCDRS=0;
SOMENOP;
LCDRW=0;
SOMENOP;
P0=insdat;
LCDEN=0;
SOMENOP;
LCDEN=1;
SOMENOP;

P0=0XFF;
return ;
}



void pWriteData(unsigned char cdata)
{
//CheckBusy();
LCDEN=1;
SOMENOP;
LCDRS=1;
SOMENOP;
LCDRW=0;
SOMENOP;
P0=cdata;
LCDEN=0;
SOMENOP;

LCDEN=1;
SOMENOP;

P0=0XFF;
return ;
}

*/

void LcdInit(void)
{
    cpldog();
    delay(50);
    WriteIns(0x30);
    delay(6);
    WriteIns(0x30);
    delay(1);
    WriteIns(0x30);
    WriteIns(0x3c);
    WriteIns(0x38);
    WriteIns(1);
    WriteIns(2);
    //WriteIns(0x0c);


}




void  DispHZ(unsigned char addr, unsigned char *buf, unsigned char lenofh_word)
{
    unsigned char *pc = buf;
    unsigned char ii = lenofh_word;
    if(ii > 15) ii = 15;
    pWriteIns(addr);
    while(ii--)	pWriteData(*pc++);
}


void DispClrLine(unsigned char linenb)
{
    unsigned char ii = 15;
    pWriteIns(0x80 + linenb * 0x10);
    while(ii--)	pWriteData(' ');
}

void DispLine(unsigned char *line1, unsigned char *line2)
{
    DispClrLine(0);
    DispHZ(0x80, line1, 15);
    DispClrLine(1);
    DispHZ(0x90, line2, 15);

}

void DispLogo(void)
{
    DispHZ(0x80, "直接读卡显示  ", 15);	//{"    诚义电子   "};
}


