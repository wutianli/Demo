#ifndef ENCRYPTION_
#define ENCRYPTION_

#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/delay.h> 
#include <asm/io.h>
#include <asm/atomic.h>
#include <asm/delay.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/init.h> 
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/ioctl.h>
#include <linux/fs.h>


#define HD_S8  char
#define HD_U8  unsigned char
#define HD_S16 short
#define HD_U16 unsigned short
#define HD_S32 int
#define HD_U32 unsigned int


#define HD_SM_OK                  0x00
#define HD_SM_ERR                 0x01
#define HD_SM_RCV_INS_ERR         0x02
#define HD_SM_RCV_SW_ERR          0x03
#define HD_SPI_OPEN_ERR           0x04
#define HD_SPI_INIT_ERR           0x05
#define HD_SPI_TRANSFER_ERR       0x06
#define HD_SM_RX_CRC_ERR          0x07

#define HD_SM_CMD_LEN             5
#define HD_SM_RXBUF_LEN           2048
#define HD_SM1_KEY_LEN            16
#define HD_SM1_IV_LEN             16
#define HD_SM1_DATABUF_LEN        2048
#define HD_SM2_PUBLIC_KEY_LEN     64
#define HD_SM2_PRIVATE_KEY_LEN    32


typedef struct hd_sm1_data
{
    HD_U8  key_buf[HD_SM1_KEY_LEN];
	HD_U8  iv_buf[HD_SM1_IV_LEN];
	HD_U8  txbuf[HD_SM1_DATABUF_LEN];
	HD_U32 tx_len;
	HD_U8  rxbuf[HD_SM1_DATABUF_LEN];
	HD_U32 rx_len[1];
}HD_SM1_DATA, *P_HD_SM1_DATA;

unsigned char get_crc7(const unsigned char *buff, int len);
HD_U8 hd_rcv_ins(HD_U8 *txbuf, HD_U8 *rxbuf, HD_U8 ins);
HD_U8 hd_rcv_sw(HD_U8 *txbuf, HD_U8 *rxbuf, HD_U8 *sw);
HD_U8 hd_spi_write(HD_U8 *txbuf, HD_U8 *rxbuf, HD_U32 len);
HD_U8 hd_spi_read(HD_U8 *txdata, HD_U8 *rxbuf, HD_U32 len);

HD_U8 hd_sm1_import_key(HD_U8 *txbuf);
HD_U8 hd_sm1_import_iv(HD_U8 *txbuf);

HD_U8 hd_sm1_encrypt(HD_U8 *key_buf, HD_U8 *iv_buf, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len);
HD_U8 hd_sm1_decrypt(HD_U8 *key_buf, HD_U8 *iv_buf, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len);

#endif