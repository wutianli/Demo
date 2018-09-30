#include "encryption.h"


int spi_master_init(int fd, unsigned char mode, int speed)
{
	
	unsigned char	lsb = 0;
	unsigned char   bits = 8;
	int ret;

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);    //写模式 
    if (ret == -1) 
	{
        printf("can't set spi mode");
		return HD_SPI_INIT_ERR;
	}
 
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);    //读模式 
    if (ret == -1) 
        printf("can't get spi mode"); 
	
/*	
	if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0) {
		printf("SPI wr_lsb_fist");
		return;
	}
	
	if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
		printf("SPI rd_lsb_fist");
		return;
	}
*/
    /*
     * bits per word    //设置每个字含多少位
     */ 
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);   //写 每个字含多少位 
    if (ret == -1) 
	{
        printf("can't set bits per word");
		return HD_SPI_INIT_ERR;		
	}
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);   //读 每个字含多少位 
    if (ret == -1) 
        printf("can't get bits per word"); 

    /*
     * max speed hz     //设置速率
     */ 
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);   //写速率 
    if (ret == -1) 
	{
        printf("can't set max speed hz");
		return HD_SPI_INIT_ERR;		
	}
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);   //读速率 
    if (ret == -1) 
        printf("can't get max speed hz"); 
		
	printf("encryption spi mode %d, %d bits per word, %d Hz max\n",
		mode, bits, speed);
		
	return 0;
}

int spi_transfer(int fd, unsigned char *txbuf, unsigned char *rxbuf, int bytes)
{
    struct spi_ioc_transfer	xfer[1];
    int	status;

	memset(xfer, 0, sizeof(xfer));
	
	xfer[0].tx_buf = (int)txbuf;
	xfer[0].rx_buf = (int)rxbuf;
	xfer[0].len = bytes;

	status = ioctl(fd, SPI_IOC_MESSAGE(1), xfer);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return -1;
	}
	
	//printf("0x%.2X \n", rxbuf[0]); 
    return status;

}


int spi_init()
{
	int re_val;
	
	spi_fd = open(device, O_RDWR);  //打开设备文件 
    if (spi_fd < 0)
	{
        printf("can't open spi device\n");
		return HD_SPI_OPEN_ERR;
	}
	re_val = spi_master_init(spi_fd, mode, speed);
	if(re_val != 0)
	{
		return HD_SPI_INIT_ERR;
	}
	
	return 0;
}