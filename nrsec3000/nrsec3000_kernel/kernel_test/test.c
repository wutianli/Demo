#if 0
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/netdevice.h>
#include <linux/interrupt.h>
#include <linux/delay.h> 
#include <asm/io.h>
#include <asm/atomic.h>
#include <asm/delay.h>
#include <linux/wait.h>
#endif
#include "../kernel_spi/encryption.h"


extern HD_U8 hd_sm1_encrypt(HD_U8 *key_buf, HD_U8 *iv_buf, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len);
extern HD_U8 hd_sm1_decrypt(HD_U8 *key_buf, HD_U8 *iv_buf, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len);

int sm1_test()
{
	HD_SM1_DATA sm1_buf;

	unsigned char keybuf[17] = {0x1,0x6,0x33,0x8,0x5,0x16,0x7,0x18,0x29,0x10,0x11,0x17,0x13,0x14,0x15,0x16};
	unsigned char ivbuf[17] = {0x75,0x12,0x36,0x14,0x5,0x16,0x7,0x18,0x26,0x10,0x11,0x12,0x13,0x14,0x15,0x16};
	unsigned char txbuf[17] = {0x67,0x12,0x33,0x14,0x5,0x16,0x7,0x18,0x34,0x10,0x11,0x12,0x13,0x14,0x15,0x19};
    unsigned char rxbuf[256];
	unsigned char buf[256];
	unsigned int rxlen[1];
	unsigned int len[1];
	int i;
	HD_U8 re_val;
	
	memcpy(sm1_buf.key_buf, keybuf, strlen(keybuf));
	memcpy(sm1_buf.iv_buf, ivbuf, strlen(ivbuf));
	memcpy(sm1_buf.txbuf, txbuf, strlen(txbuf));
	
	sm1_buf.tx_len = strlen(txbuf);
	
	printk("txdata  txlen:%d txdate:", strlen(txbuf));
	for (i=0; i < 16; i++)
	{
		printk("%2x ", txbuf[i]);
	}
	printk("\n");
	
	re_val = hd_sm1_encrypt(sm1_buf.key_buf, sm1_buf.iv_buf, sm1_buf.txbuf, 16, sm1_buf.rxbuf, sm1_buf.rx_len);
	if(re_val != 0)
		return 1;
	
	printk("encrypt rxlen:%d rxdate:", sm1_buf.rx_len[0]);
	for (i=0; i < sm1_buf.rx_len[0]; i++)
	{
		printk("%2x ", sm1_buf.rxbuf[i]);
	}
	printk("\n");
	
	re_val = hd_sm1_decrypt(sm1_buf.key_buf, sm1_buf.iv_buf, sm1_buf.rxbuf, sm1_buf.rx_len[0], buf, len);
	if(re_val != 0)
		return 1;
	
	printk("decrypt rxlen:%d rxdate:", len[0]);
	for (i=0; i < len[0]; i++)
	{
		printk("%2x ", buf[i]);
	}
	printk("\n");
	
	printk("****************************************\n");
	if(memcmp(txbuf, buf, 16))
	{
		printk("hd_sm1 test fail\n");
	}
	else
	{
		printk("hd_sm1 test ok\n");
	}
	printk("****************************************\n");
	
	return 0;
}




static int test_init(void)
{
	
	int re_val = 0;
	
	re_val = sm1_test();
	if(re_val != 0)
		printk("sm1_test err\n");
	
	return 0;
}

static void test_exit(void)
{
	printk("test exit\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("Dual BSD/GPL");
