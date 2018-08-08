#include <reg52.h>
#include <string.h>
#include <intrins.h>
#include "main.h"
#include "slrc632.h"
#include "iso14443a.h"
#include "iso14443b.h"
#include "iso15693.h" 
#include "LCD1602.h"
#include <absacc.h>



void delayNOP()
{
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
}


void delay_ms(unsigned int ms)
{
  unsigned char k;
  while (ms--)
  {
    for (k = 0; k < 114; k++)
      ;
  }
}


void lcd_busy()
{
  bit busy;
  busy = 1;
  while (busy)
  {
    LCDRS = 0;
    LCDRW = 1;
    LCDEN = 1;
    busy = (bit)(P0 &0x80);
    delayNOP();
  }
  LCDEN = 0;
}


void lcd_wcmd(unsigned char cmd, bit Check)
{
  if (Check)
  lcd_busy();		  //����æ���

  LCDRS = 0;
  LCDRW = 0;
  LCDEN = 1;
  P0 = cmd;
  delayNOP();
  LCDEN = 0;
}


void lcd_wdat(unsigned char dat)
{
  lcd_busy();         //����æ���
  LCDRS = 1;
  LCDRW = 0;
  LCDEN = 1;
  P0 = dat;
  delayNOP();
  LCDEN = 0;
}

void LcdInit(void)
{
  delay_ms(15);
  lcd_wcmd(0x38, 0); //16*2��ʾ��5*7����8λ����
  delay_ms(5);
  lcd_wcmd(0x38, 0); //������æ��⣬ǿ��ִ�����Ρ�
  delay_ms(5);
  lcd_wcmd(0x38, 0);
  delay_ms(5);

  lcd_wcmd(0x38, 1); //����æ���
  delay_ms(5);
  lcd_wcmd(0x0c, 1); //��ʾ�����ع��
  delay_ms(5);
  lcd_wcmd(0x06, 1); //�ƶ����
  delay_ms(5);
  lcd_wcmd(0x01, 1); //���LCD����ʾ����
  delay_ms(5);
}

void  DispHZ(unsigned char addr,unsigned char *buf,unsigned char lenofh_word)
{
	unsigned char *pc=buf;
	unsigned char ii=lenofh_word;
	//if(ii>15) ii=15;
	lcd_wcmd(addr,1);
	while(ii--)	lcd_wdat(*pc++);	
}

void LCD_UIDSHOW(unsigned char lenth)     //LCD��ʾ������Ĳ�
{
  unsigned int i;
  unsigned char  *pi;	
	pi=sBuffer;
	i=lenth;
	if(i==8)        //ֻ��һ�ſ�
	{
		lcd_wcmd(0x01, 1); //���LCD����ʾ����
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"�嵰        10Ԫ",16);
						    break;
			}
	}
	
	if(i==16)        //�����ſ�
	{
		lcd_wcmd(0x01, 1); //���LCD����ʾ����
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"�嵰        10Ԫ",16);
						    break;
			}
		switch(pi[8])
			{
						case 0x22:
								DispHZ(0x90,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x90,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x90,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x90,"�嵰        10Ԫ",16);
						    break;
			}
	}
	
	if(i==24)        //�����ſ�
	{
		lcd_wcmd(0x01, 1); //���LCD����ʾ����
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"�嵰        10Ԫ",16);
						    break;
			}
		switch(pi[8])
			{
						case 0x22:
								DispHZ(0x90,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x90,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x90,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x90,"�嵰        10Ԫ",16);
						    break;
			}
		switch(pi[16])
			{
						case 0x22:
								DispHZ(0x88,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x88,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x88,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x88,"�嵰        10Ԫ",16);
						    break;
			}
	}
	
	if(i==32)        //�����ſ�
	{
    lcd_wcmd(0x01, 1); //���LCD����ʾ����
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"�嵰        10Ԫ",16);
						    break;
			}
		switch(pi[8])
			{
						case 0x22:
								DispHZ(0x90,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x90,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x90,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x90,"�嵰        10Ԫ",16);
						    break;
			}
		switch(pi[16])
			{
						case 0x22:
								DispHZ(0x88,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x88,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x88,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x88,"�嵰        10Ԫ",16);
						    break;
			}
			
		 switch(pi[24])
			{
						case 0x22:
								DispHZ(0x98,"�����׷�     3Ԫ",16);
						    break;
						
						case 0x8E:
								DispHZ(0x98,"������������ 4Ԫ",16);
						    break;
						
						case 0x97:
								DispHZ(0x98,"�����Ĳ�     5Ԫ",16);
						    break;
						
						case 0x24:
								DispHZ(0x98,"�嵰        10Ԫ",16);
						    break;
			}
		}
}

