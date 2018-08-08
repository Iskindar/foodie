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
  lcd_busy();		  //进行忙检测

  LCDRS = 0;
  LCDRW = 0;
  LCDEN = 1;
  P0 = cmd;
  delayNOP();
  LCDEN = 0;
}


void lcd_wdat(unsigned char dat)
{
  lcd_busy();         //进行忙检测
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
  lcd_wcmd(0x38, 0); //16*2显示，5*7点阵，8位数据
  delay_ms(5);
  lcd_wcmd(0x38, 0); //不进行忙检测，强制执行三次。
  delay_ms(5);
  lcd_wcmd(0x38, 0);
  delay_ms(5);

  lcd_wcmd(0x38, 1); //进行忙检测
  delay_ms(5);
  lcd_wcmd(0x0c, 1); //显示开，关光标
  delay_ms(5);
  lcd_wcmd(0x06, 1); //移动光标
  delay_ms(5);
  lcd_wcmd(0x01, 1); //清除LCD的显示内容
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

void LCD_UIDSHOW(unsigned char lenth)     //LCD显示卡代表的菜
{
  unsigned int i;
  unsigned char  *pi;	
	pi=sBuffer;
	i=lenth;
	if(i==8)        //只有一张卡
	{
		lcd_wcmd(0x01, 1); //清除LCD的显示内容
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"煎蛋        10元",16);
						    break;
			}
	}
	
	if(i==16)        //有两张卡
	{
		lcd_wcmd(0x01, 1); //清除LCD的显示内容
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"煎蛋        10元",16);
						    break;
			}
		switch(pi[8])
			{
						case 0x22:
								DispHZ(0x90,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x90,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x90,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x90,"煎蛋        10元",16);
						    break;
			}
	}
	
	if(i==24)        //有三张卡
	{
		lcd_wcmd(0x01, 1); //清除LCD的显示内容
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"煎蛋        10元",16);
						    break;
			}
		switch(pi[8])
			{
						case 0x22:
								DispHZ(0x90,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x90,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x90,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x90,"煎蛋        10元",16);
						    break;
			}
		switch(pi[16])
			{
						case 0x22:
								DispHZ(0x88,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x88,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x88,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x88,"煎蛋        10元",16);
						    break;
			}
	}
	
	if(i==32)        //有四张卡
	{
    lcd_wcmd(0x01, 1); //清除LCD的显示内容
		switch(pi[0])
			{
						case 0x22:
								DispHZ(0x80,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x80,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x80,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x80,"煎蛋        10元",16);
						    break;
			}
		switch(pi[8])
			{
						case 0x22:
								DispHZ(0x90,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x90,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x90,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x90,"煎蛋        10元",16);
						    break;
			}
		switch(pi[16])
			{
						case 0x22:
								DispHZ(0x88,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x88,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x88,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x88,"煎蛋        10元",16);
						    break;
			}
			
		 switch(pi[24])
			{
						case 0x22:
								DispHZ(0x98,"二两米饭     3元",16);
						    break;
						
						case 0x8E:
								DispHZ(0x98,"西红柿炒鸡蛋 4元",16);
						    break;
						
						case 0x97:
								DispHZ(0x98,"炒空心菜     5元",16);
						    break;
						
						case 0x24:
								DispHZ(0x98,"煎蛋        10元",16);
						    break;
			}
		}
}

