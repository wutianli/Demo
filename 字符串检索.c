  	char rbuf[210];
	char *file_name = "/tmp/bm.cfg";
	int fp;
	char *p = NULL;
	char *ipbuf = "147.253.148.2";

	system("ifconfig bm0 > /tmp/bm.cfg 2>&1");
	
	fp = open(file_name, 2);
	if(fp == NULL)
	{
		printf("cann`t open bm.cfg\n");
		return -1;
	}
	
	ret = read(fp, rbuf, 200);
	if(ret < 0)
	{
		printf("read err!\n");
		return -1;
	}
			
	p = strstr(rbuf, ipbuf);
	
	if(NULL == p)
	{
		close(fp);
		printf("no bm0 or IP\n");
		return -1;
	}
	
	close(fp);
