#include "encryption.h"

extern struct spidev_data	*glspidev;	
extern  int spidev_info(struct spidev_data *spidev,struct spi_ioc_transfer *u_xfers, unsigned n_xfers);


int spi_transfer(unsigned char *txbuf, unsigned char *rxbuf, int bytes)
{
    struct spi_ioc_transfer	xfer[1];

	memset(xfer, 0, sizeof(xfer));
    xfer[0].tx_buf = (int)txbuf;
    xfer[0].rx_buf = (int)rxbuf;
    xfer[0].len = bytes;

    spidev_info(glspidev,xfer, 1);

    return 0;

}

EXPORT_SYMBOL(spi_transfer);