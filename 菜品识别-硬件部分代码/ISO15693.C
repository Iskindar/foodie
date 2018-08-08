#include <reg52.h>
#include <string.h>
#include "main.h"
#include "slrc632.h"
#include "iso15693.h" 


extern struct TranSciveBuffer{unsigned char MfCommand;
                              unsigned int  MfLength;
                              unsigned char MfData[64];
                             };

//ËùÓÐº¯ÊýµÄ²ÎÊý²Î¼ûISO15693-3/10.3
//·µ»ØÊý¾ÝÔÚ´æÈërespÊ±È¥µôÁËflag×Ö½Ú
//µ±¸ÄÐ´¿¨Æ¬ÉÏÊý¾ÝÊ±£¨write or lock)Èç²Ù×÷µÄÊÇTI¿¨Æ¬ÉèflagsµÄbit6 Option_flag = 1£¬Èç²Ù×÷µÄÊÇI.CODE SLI¿¨Æ

char ISO15693_Inventory (unsigned char flags,
                         unsigned char AFI, 
                         unsigned char masklengh, 
                         unsigned char *uid, 
                         unsigned char *resp)
{
    unsigned char SndCNT, cnt, status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
        
    ClearBitMask(RegCoderControl, 0x80);
            
    MfComData.MfCommand = PCD_TRANSCEIVE;	
    MfComData.MfData[0] = flags;
    MfComData.MfData[1] = ISO15693_INVENTORY;
    SndCNT = 2;
    if (flags & 0x10)
    {
        MfComData.MfData[SndCNT] = AFI;		
        SndCNT++;
    }
    MfComData.MfData[SndCNT] = masklengh;
    SndCNT++;
    if (masklengh%8)
    {    cnt = masklengh/8 + 1;    }
    else
    {    cnt = masklengh/8;        }
    if (cnt)
    {    memcpy(&MfComData.MfData[SndCNT], uid, cnt);    }
    MfComData.MfLength  = cnt + SndCNT;

    status = ISO15693_Transceive(pi);
   
    if (status == MI_OK)
    {
        if (MfComData.MfLength != 0x50)
        {    status = MI_BITCOUNTERR;    }
        else
        {    memcpy(resp, &MfComData.MfData[1], 9);    }
    }     
    return status;
}

/////////////////////////////////////////////////////////////////////
//ISO15693 INVENTORY_16
/////////////////////////////////////////////////////////////////////                           
char ISO15693_Inventory16(unsigned char flags,
                          unsigned char AFI, 
                          unsigned char masklengh, 
                          unsigned char *uid, 
                          unsigned char *resplen, 
                          unsigned char *resp)
{
    unsigned char idata SndCNT, cnt, status, status1, i,j;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
	  unsigned int xdata timecount=0;
//	  unsigned char vRet;
    pi = &MfComData;
    status = MI_COM_ERR;
    *resplen = 0;
    SetBitMask(RegChannelRedundancy, 0x04);   
    ClearBitMask(RegCoderControl, 0x80);
    
                
    MfComData.MfCommand = PCD_TRANSMIT;	
    MfComData.MfData[0] = flags;
    MfComData.MfData[1] = ISO15693_INVENTORY;
    SndCNT = 2;
    if (flags & 0x10)
    {
        MfComData.MfData[SndCNT] = AFI;		
        SndCNT++;
    }
    MfComData.MfData[SndCNT] = masklengh;
    SndCNT++;
    if (masklengh%8)
    {    cnt = masklengh/8 + 1;    }
    else
    {    cnt = masklengh/8;        }
    if (cnt)
    {   
 		   memcpy(&MfComData.MfData[SndCNT], uid, cnt);  
		}
    MfComData.MfLength  = cnt + SndCNT;

    status1 = ISO15693_Transceive(pi);
	

    j = 0;
    for (i=0; i<16; i++)
    {
         pi = &MfComData;
         ClearBitMask(RegChannelRedundancy, 0x04);
         SetBitMask(RegCoderControl, 0x80);
         SetBitMask(RegTxControl, 0x10);
         MfComData.MfCommand = PCD_TRANSCEIVE;
         MfComData.MfLength = 0;

         status1 = ISO15693_Transceive( pi );  


         if ((status1 == MI_OK) && (MfComData.MfLength == 0x50))
         {
					// vRet = Save_UID(&MfComData.MfData[2]);
//					 if(vRet ==0)
//					 {
//						 status = MI_OK;
//					 }
	         //if(*resplen>24)
					 {
						 status = MI_OK;
					 }
					 
			     *resplen += 9;
		       memcpy(resp+9*j, &MfComData.MfData[1], 9);	
           j++; 					 
         }
         if (*resplen >= 36)
         {   break;    }
	}

	

	return status;
}


/////////////////////////////////////////////////////////////////////
//ISO15693_READ Single/Multiple Block(s)
/////////////////////////////////////////////////////////////////////
/*char ISO15693_Read_sm (unsigned char flags, 
                       unsigned char *uid, 
                       unsigned char blnr, 
                       unsigned char nbl, 
                       unsigned char *resplen, 
                       unsigned char *resp)
{
    unsigned char idata cnt, status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    *resplen = 0;
        
    ClearBitMask(RegCoderControl, 0x80);
        
    if(nbl)
    {    nbl--;	}
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfData[0] = flags;
    if (nbl)
    {    MfComData.MfData[1] = ISO15693_READ_MULTIPLE_BLOCKS;    }
    else
    {    MfComData.MfData[1] = ISO15693_READ_SINGLE_BLOCK;       }
    cnt = 2;
    if ((flags & 0x20) && !(flags & 0x10)) // flags & 0x20 - Addressflag 
				   // request flags & 0x10 - Selectflag request
    {
        memcpy (&MfComData.MfData[cnt], uid, 8);
        cnt = cnt + 8;
    }
    MfComData.MfData[cnt] = blnr;
    if (nbl)
    {
        cnt++;
        MfComData.MfData[cnt] = nbl;
    }

    MfComData.MfLength = cnt + 1;

    status = ISO15693_Transceive( pi );
	
    if (status == MI_OK)
    {
        *resplen = MfComData.MfLength/8 - 1;
        memcpy(resp, &MfComData.MfData[1], *resplen);
    }
    return status;
}*/


