#include "encryption.h"

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
	
	printf("txdata  txlen:%d txdate:", strlen(txbuf));
	for (i=0; i < 16; i++)
	{
		printf("%2x ", txbuf[i]);
	}
	printf("\n");
	
	re_val = hd_sm1_encrypt(spi_fd, sm1_buf.key_buf, sm1_buf.iv_buf, sm1_buf.txbuf, 16, sm1_buf.rxbuf, sm1_buf.rx_len);
	if(re_val != HD_SM_OK)
	{
		printf("hd_sm1 test fail\n");
		return re_val;
	}
	printf("encrypt rxlen:%d rxdate:", sm1_buf.rx_len[0]);
	for (i=0; i < sm1_buf.rx_len[0]; i++)
	{
		printf("%2x ", sm1_buf.rxbuf[i]);
	}
	printf("\n");
	
	re_val = hd_sm1_decrypt(spi_fd, sm1_buf.key_buf, sm1_buf.iv_buf, sm1_buf.rxbuf, sm1_buf.rx_len[0], buf, len);
	if(re_val != HD_SM_OK)
	{
		printf("hd_sm1 test fail\n");
		return re_val;
	}
	
	printf("decrypt rxlen:%d rxdate:", len[0]);
	for (i=0; i < len[0]; i++)
	{
		printf("%2x ", buf[i]);
	}
	printf("\n");
	
	printf("****************************************\n");
	if(memcmp(txbuf, buf, 16))
	{
		printf("hd_sm1 test fail\n");
	}
	else
	{
		printf("hd_sm1 test ok\n");
	}
	printf("****************************************\n");
	
	return HD_SM_OK;
	
}


int sm2_import_key_test()
{
	HD_U8 p2 = 0x01;
	int i;
	HD_U8 re_val;
	
	HD_U8 pri_key_txbuf[32] = {0x1,0x3,0x5,0x7,0x2,0x4,0x6,0x8,0x9,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32};
	
	HD_U8 pub_key_txbuf[64] = {0x7,0x6,0x5,0x4,0x3,0x2,0x1,0x8,0x9,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32,0x1,0x3,0x5,0x7,0x2,
	0x4,0x6,0x8,0x9,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
	0x27,0x28,0x29,0x32,0x31,0x30};
	
	//HD_U8 *pri_key_rxbuf = NULL;
	
	HD_U8 pri_key_rxbuf[256];
	HD_U8 pub_key_rxbuf[256];
	
	re_val = hd_sm2_import_key(spi_fd, p2, pri_key_txbuf, pub_key_txbuf);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_import_key_test fail\n");
		return re_val;
	}
	
	re_val = hd_sm2_export_key(spi_fd, p2, pri_key_rxbuf, pub_key_rxbuf);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_import_key_test fail\n");
		return re_val;
	}	

	printf("pub_key_txbuf:");
	for(i = 0; i < 64; i++)
	{
		printf("%2x ", pub_key_txbuf[i]);
	}
	printf("\n");
	
	printf("pub_key_rxbuf:");
	for(i = 0; i < 64; i++)
	{
		printf("%2x ", pub_key_rxbuf[i]);
	}
	printf("\n");
	
	printf("pri_key_txbuf:");
	for(i = 0; i < 32; i++)
	{
		printf("%x ", pri_key_txbuf[i]);
	}
	printf("\n");
	
	printf("pri_key_rxbuf:");
	for(i = 0; i < 32; i++)
	{
		printf("%x ", pri_key_rxbuf[i]);
	}
	printf("\n");
	
	printf("****************************************\n");
	if((memcmp(pub_key_txbuf, pub_key_rxbuf, 64)) || (memcmp(pri_key_txbuf, pri_key_rxbuf, 32)))
	{
		printf("sm2_import_key_test fail\n");
	}
	else
	{
		printf("sm2_import_key_test ok\n");
	}
	printf("****************************************\n");
	
	return 0;
}

int sm2_sign_test()
{
	unsigned char tx_data[32] = {0x1,0x3,0x5,0x7,0x2,0x4,0x6,0x8,0x9,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32};
	
	unsigned char rxbuf[256];
	unsigned char buf[256];
	unsigned char verify_buf[256];
	unsigned int rxlen[1];
	unsigned int len[1];
	HD_U8 p2 = 0x01;
	int i;
	HD_U8 re_val;
	
	re_val = hd_sm2_create_key_pair(spi_fd, p2);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_sign_test fail\n");
		return re_val;
	}
	
	re_val = hd_sm3_hash(spi_fd, tx_data, 32, rxbuf, rxlen);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_sign_test fail\n");
		return re_val;
	}
	
	printf("hd_sm3_hash:");
	for (i=0; i < 32; i++)
	{
		printf("%2x ", rxbuf[i]);
	}
	printf("\n");
	
	re_val = hd_sm2_sign(spi_fd, p2, rxbuf, 32, buf, len);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_sign_test fail\n");
		return re_val;
	}
	
	printf("hd_sm2_sign:");
	for (i=0; i < 64; i++)
	{
		printf("%2x ", buf[i]);
	}
	printf("\n");
	
	memcpy(verify_buf, rxbuf, 32);
	memcpy(verify_buf + 32, buf, 64);
/*
	for (i=0; i < 96; i++)
	{
		printf("%x ", verify_buf[i]);
	}
	printf("\n");
*/	
	re_val = hd_sm2_verify(spi_fd, p2, verify_buf, 96);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_sign_test fail\n");
		return re_val;
	}
	
	printf("****************************************\n");
	printf("sm2_sign_test ok\n");
	printf("****************************************\n");
	
	return HD_SM_OK;
}


int sm2_encrypt_test()
{

	unsigned char sm2_data[32] = {0x7,0x6,0x5,0x4,0x3,0x2,0x1,0x8,0x9,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32};
	
	unsigned char rxbuf[256];
	unsigned char buf[256];
	unsigned int rxlen[1];
	unsigned int len[1];
	HD_U8 p2 = 0x01;
	int i;
	HD_U8 re_val;
	
	printf("sm2_encrypt txlen:%d txdate:", sizeof(sm2_data));
	for (i=0; i < 32; i++)
	{
		printf("%2x ", sm2_data[i]);
	}
	printf("\n");
	
	re_val = hd_sm2_create_key_pair(spi_fd, p2);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_encrypt_test fail\n");
		return re_val;
	}
	
	re_val = hd_sm2_encrypt(spi_fd, p2, sm2_data, 32, rxbuf, rxlen);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_encrypt_test fail\n");
		return re_val;
	}
	
	re_val = hd_sm2_decrypt(spi_fd, p2, rxbuf, rxlen[0], buf, len);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_encrypt_test fail\n");
		return re_val;
	}
	
	printf("sm2_decrypt rxlen:%d rxdate:", len[0]);
	for (i=0; i < len[0]; i++)
	{
		printf("%2x ", buf[i]);
	}
	printf("\n");
	
	printf("****************************************\n");
	if(memcmp(sm2_data, buf, 32))
	{
		printf("sm2_encrypt_test fail\n");
	}
	else
	{
		printf("sm2_encrypt_test ok\n");
	}
	printf("****************************************\n");
	
	return HD_SM_OK;
}

int sm2_certificate_request_test()
{
	unsigned char request_data[] = "C=CN,ST=jiangsu,L=nanjing,O=GDD,OU=nari,CN=test001";
	
	unsigned char request_rxbuf[512];
	unsigned char buf[256];
	unsigned int rxlen[1];
	unsigned int len[1];
	HD_U8 p1 = 0x00;
	HD_U8 p2 = 0x01;
	HD_U8 p3;
	int i;
	HD_U8 re_val;
	
	p3 = strlen(request_data);
	
	re_val =  hd_sm2_create_key_pair(spi_fd, p2);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_certificate_request_test fail\n");
		return re_val;
	}
	
	re_val = hd_sm2_certificate_request(spi_fd, p1, p2, p3, request_data, request_rxbuf, rxlen);
	if(re_val != HD_SM_OK)
	{
		printf("sm2_certificate_request_test fail\n");
		return re_val;
	}
	
	printf("certificate_request rxlen:%d rxdate:", rxlen[0]);
	for (i=0; i < rxlen[0]; i++)
	{
		printf("%2x ", request_rxbuf[i]);
	}
	printf("\n");
	
	printf("****************************************\n");
	printf("sm2_certificate_request_test ok\n");
	printf("****************************************\n");
	
	return 0;
}

int main(int argc, char *argv[]) 
{ 
	int re_val;
	
	spi_init();
	
	re_val = sm1_test();
	if(re_val != 0)
		printf("sm1_test err\n");
		
	sm2_import_key_test();
	sm2_sign_test();
	sm2_encrypt_test();
	
	sm2_certificate_request_test();
	
	close(spi_fd);
	
	return 0;
}