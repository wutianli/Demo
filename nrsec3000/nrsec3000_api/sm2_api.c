#include "encryption.h"

HD_U8 hd_sm2_create_key_pair(HD_U32 fd, HD_U8 p2)
{
	HD_U8 swbuf[1];
	HD_U8 sw_rcv[2];
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rxbuf[16];
	HD_U8 re_val;
	
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb2;
	cmd_buf[2] = 0x00;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x00;

	hd_spi_write(fd, cmd_buf, rxbuf, HD_SM_CMD_LEN);

	re_val = hd_rcv_sw(fd, swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_export_public_key(HD_U32 fd, HD_U8 p2, HD_U8 *rxbuf)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 rxlen[1];
	HD_U8 re_val;

	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb8;
	cmd_buf[2] = 0x01;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x40;
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, tx_data, rxlen, 1);
	
	hd_spi_read(fd, tx_data, rxbuf, rxlen[0]);
	rx_crc[0] = get_crc7(rxbuf, rxlen[0] - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen[0] - 1])
	{
		printf("hd_sm2_export_public_key rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_export_private_key(HD_U32 fd, HD_U8 p2, HD_U8 *rxbuf)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 rxlen[1];
	HD_U8 re_val;

	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb8;
	cmd_buf[2] = 0x02;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x20;
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, tx_data, rxlen, 1);
	
	hd_spi_read(fd, tx_data, rxbuf, rxlen[0]);
	rx_crc[0] = get_crc7(rxbuf, rxlen[0] - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen[0] - 1])
	{
		printf("hd_sm2_export_private_key rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	
	return HD_SM_OK;
}


HD_U8 hd_sm2_export_key(HD_U32 fd, HD_U8 p2, HD_U8 *pri_key_rxbuf, HD_U8 *pub_key_rxbuf)
{
	
	hd_sm2_export_public_key(fd, p2, pub_key_rxbuf);
	
	if(pri_key_rxbuf != NULL)
	hd_sm2_export_private_key(fd, p2, pri_key_rxbuf);
	
	return HD_SM_OK;
}


HD_U8 hd_sm2_import_public_key(HD_U32 fd, HD_U8 p2, HD_U8 *txbuf)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rxbuf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 date_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xba;
	cmd_buf[2] = 0x01;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x40;
	
	date_crc[0] = get_crc7(txbuf, HD_SM2_PUBLIC_KEY_LEN);
	
	hd_spi_write(fd, cmd_buf, rxbuf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rxbuf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rxbuf, 1);
	
	hd_spi_write(fd, txbuf, rxbuf, HD_SM2_PUBLIC_KEY_LEN);
	hd_spi_write(fd, date_crc, rxbuf, 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_import_private_key(HD_U32 fd, HD_U8 p2, HD_U8 *txbuf)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rxbuf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 date_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xba;
	cmd_buf[2] = 0x02;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x20;
	
	date_crc[0] = get_crc7(txbuf, HD_SM2_PRIVATE_KEY_LEN);
	
	hd_spi_write(fd, cmd_buf, rxbuf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rxbuf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rxbuf, 1);
	
	hd_spi_write(fd, txbuf, rxbuf, HD_SM2_PRIVATE_KEY_LEN);
	hd_spi_write(fd, date_crc, rxbuf, 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_import_key(HD_U32 fd, HD_U8 p2, HD_U8 *pri_key_txbuf, HD_U8 *pub_key_txbuf)
{
	hd_sm2_import_private_key(fd, p2, pri_key_txbuf);
	
	hd_sm2_import_public_key(fd, p2, pub_key_txbuf);
	
	return HD_SM_OK;
}

HD_U8 hd_sm3_hash(HD_U32 fd, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 rxlen[1];
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb5;
	cmd_buf[2] = 0x00;
	cmd_buf[3] = ((tx_len >> 8) & 0xff);
	cmd_buf[4] = tx_len & 0xff;
	
	data_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rx_buf, 1);
	hd_spi_write(fd, txbuf, rx_buf, tx_len);
	hd_spi_write(fd, data_crc, rx_buf, 1);
	hd_spi_write(fd, tx_data, rx_buf, 1);
	
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, tx_data, rxlen, 1);
	
	hd_spi_read(fd, tx_data, rxbuf, rxlen[0]);
	rx_crc[0] = get_crc7(rxbuf, rxlen[0] - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen[0] - 1])
	{
		printf("hd_sm3_hash rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	
	rx_len[0] = rxlen[0] - 1;
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_sign(HD_U32 fd, HD_U8 p2, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 rxlen[1];
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb4;
	cmd_buf[2] = 0x00;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x20;
	
	data_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rx_buf, 1);
	hd_spi_write(fd, txbuf, rx_buf, tx_len);
	hd_spi_write(fd, data_crc, rx_buf, 1);
	hd_spi_write(fd, tx_data, rx_buf, 1);
	
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, tx_data, rxlen, 1);
	
	hd_spi_read(fd, tx_data, rxbuf, rxlen[0]);
	rx_crc[0] = get_crc7(rxbuf, rxlen[0] - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen[0] - 1])
	{
		printf("hd_sm2_sign rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	rx_len[0] = rxlen[0] - 1;
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_verify(HD_U32 fd, HD_U8 p2, HD_U8 *txbuf, HD_U32 tx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rxbuf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 date_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	HD_U8 over_flag[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb6;
	cmd_buf[2] = 0x00;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x60;
	
	date_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(fd, cmd_buf, rxbuf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rxbuf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rxbuf, 1);
	
	hd_spi_write(fd, txbuf, rxbuf, tx_len);
	hd_spi_write(fd, date_crc, rxbuf, 1);
	
	hd_spi_write(fd, over_flag, rxbuf, 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rxbuf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	return HD_SM_OK;
}

HD_U8 hd_sm2_encrypt(HD_U32 fd, HD_U8 p2, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 data_flag[1] = {0x55};
	HD_U8 rxlen[1];
	HD_U8 re_val;
	
	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb3;
	cmd_buf[2] = 0x01;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x20;
	
	data_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rx_buf, 1);
	hd_spi_write(fd, txbuf, rx_buf, tx_len);
	hd_spi_write(fd, data_crc, rx_buf, 1);
	hd_spi_write(fd, tx_data, rx_buf, 1);

	
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, tx_data, rxlen, 1);

	hd_spi_read(fd, tx_data, rxbuf, rxlen[0]);
	rx_crc[0] = get_crc7(rxbuf, rxlen[0] - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen[0] - 1])
	{
		printf("hd_sm2_encrypt rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	
	rx_len[0] = rxlen[0] - 1;

	return HD_SM_OK;
}

HD_U8 hd_sm2_decrypt(HD_U32 fd, HD_U8 p2, HD_U8 *txbuf, HD_U32 tx_len, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U8 rxlen[1];
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb3;
	cmd_buf[2] = 0x81;
	cmd_buf[3] = p2;
	cmd_buf[4] = 0x80;
	
	data_crc[0] = get_crc7(txbuf, tx_len);
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, data_flag, rx_buf, 1);
	hd_spi_write(fd, txbuf, rx_buf, tx_len);
	hd_spi_write(fd, data_crc, rx_buf, 1);
	hd_spi_write(fd, tx_data, rx_buf, 1);
	
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	hd_spi_write(fd, tx_data, rxlen, 1);

	hd_spi_read(fd, tx_data, rxbuf, rxlen[0]);
	rx_crc[0] = get_crc7(rxbuf, rxlen[0] - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen[0] - 1])
	{
		printf("hd_sm2_decrypt rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	
	rx_len[0] = rxlen[0] - 1;

	return HD_SM_OK;
}

HD_U8 hd_sm2_certificate_request(HD_U32 fd, HD_U8 p1, HD_U8 p2, HD_U8 p3, HD_U8 *txbuf, HD_U8 *rxbuf, HD_U32 *rx_len)
{
	HD_U8 cmd_buf[HD_SM_CMD_LEN];
	HD_U8 rx_buf[HD_SM_RXBUF_LEN];
	HD_U8 insbuf[1];
	HD_U8 swbuf[1];
	HD_U8 data_crc[1];
	HD_U8 rx_crc[1];
	HD_U8 sw_rcv[2];
	HD_U32 rxlen;
	HD_U8 re_val;
	
	HD_U8 data_flag[1] = {0x55};
	HD_U8 over_flag[1] = {0xaa};
	HD_U8 tx_data[1] = {0xaa};
	insbuf[0] = 0xaa;
	swbuf[0] = 0xaa;
	
	sw_rcv[0] = 0x90;
	sw_rcv[1] = 0x00;
	
	cmd_buf[0] = 0x80;
	cmd_buf[1] = 0xb7;
	cmd_buf[2] = p1;
	cmd_buf[3] = p2;
	cmd_buf[4] = p3;
	
	data_crc[0] = get_crc7(txbuf, p3);
	
	hd_spi_write(fd, cmd_buf, rx_buf, HD_SM_CMD_LEN);
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;
	
	memcpy(txbuf + strlen(txbuf), data_crc, strlen(data_crc));
	hd_spi_write(fd, data_flag, rx_buf, 1);
	hd_spi_write(fd, txbuf, rx_buf, p3 + 1);
	//hd_spi_write(fd, data_crc, rx_buf, 1);
	hd_spi_write(fd, over_flag, rx_buf, 1);
	
	re_val = hd_rcv_ins(fd, insbuf, rx_buf, cmd_buf[1]);
	if(re_val != HD_SM_OK)
		return re_val;

	hd_spi_write(fd, tx_data, &rx_buf[0], 1);
	hd_spi_write(fd, tx_data, &rx_buf[1], 1);

	rxlen = (rx_buf[0] << 8) | rx_buf[1];

	hd_spi_read(fd, tx_data, rxbuf, rxlen);
	
	rx_crc[0] = get_crc7(rxbuf, rxlen - 1);
	
	re_val = hd_rcv_sw(fd, swbuf, rx_buf, sw_rcv);
	if(re_val != HD_SM_OK)
		return re_val;
	
	if(rx_crc[0] != rxbuf[rxlen - 1])
	{
		printf("hd_sm2_certificate_request rxcrc err!\n");
		return HD_SM_RX_CRC_ERR;
	}
	
	rx_len[0] = rxlen - 1;
	
	return HD_SM_OK;
}


