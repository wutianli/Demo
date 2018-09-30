#include "encryption.h"

HD_U8 hd_sm1_import_key(HD_U8 *txbuf)
{
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 ek_crc[1];
	HD_U8 ak_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rxbuf[HD_SM_RXBUF_LEN];
	HD_U8 akbuf[HD_SM1_KEY_LEN] = {0};
	HD_U8 data_flag[1];
	HD_U8 re_val;
	
	data_flag[0] = 0x55;
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xd4;
	cmd_buf[2] = 0x01;
	cmd_buf[3] = 0x00;
	cmd_buf[4] = 0x10;

	ek_crc[0] = get_crc7(txbuf, HD_SM1_KEY_LEN);
	ak_crc[0] = get_crc7(akbuf, HD_SM1_KEY_LEN); 

	hd_spi_write(cmd_buf, rxbuf, HD_SM_CMD_LEN);

	re_val = hd_rcv_ins(insbuf, rxbuf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(data_flag, rxbuf, 1);

	hd_spi_write(txbuf, rxbuf, HD_SM1_KEY_LEN);
	hd_spi_write(ek_crc, rxbuf, 1);
	
	re_val = hd_rcv_sw(swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	cmd_buf[2] = 0x02;

	hd_spi_write(cmd_buf, rxbuf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(insbuf, rxbuf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(data_flag, rxbuf, 1);

	hd_spi_write(akbuf, rxbuf, HD_SM1_KEY_LEN);
	hd_spi_write(ak_crc, rxbuf, 1);

	re_val = hd_rcv_sw(swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	return HD_SM_OK;
}
EXPORT_SYMBOL(hd_sm1_import_key);

HD_U8 hd_sm1_import_iv(HD_U8 *txbuf)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rxbuf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 iv_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 data_flag[1];
	HD_U8 re_val;
	
	data_flag[0] = 0x55;
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xd4;
	cmd_buf[2] = 0x04;
	cmd_buf[3] = 0x00;
	cmd_buf[4] = 0x10;
	
	iv_crc[0] = get_crc7(txbuf, HD_SM1_IV_LEN);
	
	hd_spi_write(cmd_buf, rxbuf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(insbuf, rxbuf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(data_flag, rxbuf, 1);
	
	hd_spi_write(txbuf, rxbuf, HD_SM1_IV_LEN);
	hd_spi_write(iv_crc, rxbuf, 1);
	
	re_val = hd_rcv_sw(swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	return HD_SM_OK;
}
EXPORT_SYMBOL(hd_sm1_import_iv);

HD_U8 hd_sm1_encrypt(HD_U8 *key_buf, HD_U8 *iv_buf, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 data_flag[1];
	HD_U8 tx_data[1];
	HD_U32 rxlen;
	HD_U8 re_val;

	data_flag[0] = 0x55;
	tx_data[0] = 0xaa;
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0xa0;
	cmd_buf[1] = 0xe0;
	cmd_buf[2] = 0x80;
	cmd_buf[3] = ((tx_len >> 8) & 0xff);
	cmd_buf[4] = tx_len & 0xff;
	
	re_val = hd_sm1_import_key(key_buf);
	if(re_val != HD_SM_OK)
		return re_val;
	
	re_val = hd_sm1_import_iv(iv_buf);
	if(re_val != HD_SM_OK)
		return re_val;
	
	data_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(data_flag, rx_buf, 1);
	hd_spi_write(txbuf, rx_buf, tx_len);
	hd_spi_write(data_crc, rx_buf, 1);
	hd_spi_write(tx_data, rx_buf, 1);
	
	re_val = hd_rcv_ins(insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(tx_data, &rx_buf[0], 1);
	hd_spi_write(tx_data, &rx_buf[1], 1);
	
	rxlen = (rx_buf[0] << 8) | rx_buf[1];
	//printk("rxlen = %d\n", rxlen);
	
	hd_spi_read(tx_data, rxbuf, rxlen);
	rx_crc[0] = get_crc7(rxbuf, rxlen - 1);
	
	re_val = hd_rcv_sw(swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen - 1])
	{
		printk("hd_sm1_encrypt rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	rx_len[0] = rxlen - 1;
	
	return HD_SM_OK;
}
EXPORT_SYMBOL(hd_sm1_encrypt);

HD_U8 hd_sm1_decrypt(HD_U8 *key_buf, HD_U8 *iv_buf, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 data_flag[1];
	HD_U8 tx_data[1];
	HD_U16 rxlen;
	HD_U8 re_val;
	
	data_flag[0] = 0x55;
	tx_data[0] = 0xaa;
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0xa0;
	cmd_buf[1] = 0xe0;
	cmd_buf[2] = 0x81;
	cmd_buf[3] = ((tx_len >> 8) & 0xff);
	cmd_buf[4] = tx_len & 0xff;
	
	re_val = hd_sm1_import_key(key_buf);
	if(re_val != HD_SM_OK)
		return re_val;
	
	re_val = hd_sm1_import_iv(iv_buf);
	if(re_val != HD_SM_OK)
		return re_val;
	
	data_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(data_flag, rx_buf, 1);
	hd_spi_write(txbuf, rx_buf, tx_len);
	hd_spi_write(data_crc, rx_buf, 1);
	hd_spi_write(tx_data, rx_buf, 1);
	
	re_val = hd_rcv_ins(insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(tx_data, &rx_buf[0], 1);
	hd_spi_write(tx_data, &rx_buf[1], 1);
	
	rxlen = (rx_buf[0] << 8) | rx_buf[1];
	
	hd_spi_read(tx_data, rxbuf, rxlen);
	rx_crc[0] = get_crc7(rxbuf, rxlen - 1);
	
	re_val = hd_rcv_sw(swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen - 1])
	{
		printk("hd_sm1_decrypt rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	rx_len[0] = rxlen - 1;
	
	return HD_SM_OK;
}
EXPORT_SYMBOL(hd_sm1_decrypt);


