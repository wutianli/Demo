#include "encryption.h"

void hd_delay(HD_U32 x)
{	
	while(--x);
}

HD_U8 hd_rcv_ins(int fd, HD_U8 *txbuf, HD_U8 *rxbuf, HD_U8 ins)
{
    HD_S32 retval; 
	HD_U32 flag = 100000;
	
    while(flag--){
		memset(rxbuf, 0, 1);
    	retval = spi_transfer(fd, txbuf, rxbuf, 1);
		//printf("ins = %x\n", *rxbuf);	
        if(*rxbuf == ins){
			//printf("ins_ok = %x\n", *rxbuf);
			return HD_SM_OK;
        }
    }
	
	printf("hd_rcv_ins fail\n");	
	return HD_SM_RCV_INS_ERR;
}

HD_U8 hd_rcv_sw(int fd, HD_U8 *txbuf, HD_U8 *rxbuf, HD_U8 *sw)
{
    HD_S32 retval; 
	HD_U32 flag = 10000;
	HD_U8 sw_flag = 10;
    txbuf[0] = 0xaa;
SW:	
    while(flag--){
		memset(rxbuf, 0, 1);
    	retval = spi_transfer(fd, txbuf, rxbuf, 1);	
        if(rxbuf[0] == sw[0]){	
            flag = 0;
        }
    }
	
	retval = spi_transfer(fd, txbuf, rxbuf+1, 1);
	if(rxbuf[1] != sw[1])
	{
		sw_flag--;
		if(sw_flag == 0)
		{
			printf("hd_rcv_sw fail\n");	
			return HD_SM_RCV_SW_ERR;
	    }
		goto SW;
	}
	
	//printf("hd_rcv_sw ok\n");
	
	return HD_SM_OK;	
}

HD_U8 hd_spi_write(HD_U32 fd, HD_U8 *txbuf, HD_U8 *rxbuf, HD_U32 len)
{
	HD_U32 i = 0;
	HD_U8 retval;
	
	for (i = 0; i < len; i++){ 	
   		 retval = spi_transfer(fd, txbuf + i, rxbuf + i, 1);
		//printf("%x ", *(txbuf+i));
	}
	//printf("\n");
	
	return HD_SM_OK;
}

HD_U8 hd_spi_read(HD_U32 fd, HD_U8 *txdata, HD_U8 *rxbuf, HD_U32 len)
{
	HD_U32 i=0;
	HD_U8 retval;
	  
	for (i=0; i<len; i++){
   		 retval = spi_transfer(fd, txdata, rxbuf + i, 1);
		 //printf("%x ", *(rxbuf+i));
	}
	
	return HD_SM_OK;
}

