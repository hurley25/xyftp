/*
 * =====================================================================================
 *
 *       Filename:  runtime.c
 *
 *    Description:  服务器运行时的一些函数
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 22时18分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "log.h"
#include "runtime.h"

// 客户连接的数据缓冲区指针
xyftp_buffer_t *conn_buff;

// 初始化运行状态
bool xyftp_init()
{
#ifndef FTP_DEBUG
	// 守护进程模式运行
	daemon(NULL, NULL);
#else
	printf("-------------------- xyftp 服务端 --------------------\n\n");
	printf("服务器于 ");
	xyftp_print_time();
	printf(" 启动\n\n当前运行中，等待客户端连接 ...\n");
#endif
	// 为客户连接的数据缓冲区申请内存
	if (!xyftp_malloc_buff()) {
		xyftp_print_info(LOG_ERR, "Alloc Buff Memery Error!");
		return false;
	}

	return true;
}

// 为客户连接的数据缓冲区申请内存
bool xyftp_malloc_buff()
{
	conn_buff = (xyftp_buffer_t *)malloc(sizeof(xyftp_buffer_t) * MAX_CONNECT_SIZE);
	if (conn_buff == NULL) {
		return false;
	}

	int i;
	for (i = 0; i < MAX_CONNECT_SIZE; i++) {
		conn_buff[i].buff = (char *)malloc(BUFF_LENGTH);
		conn_buff[i].len = 0;
		conn_buff[i].size = 0;
		if (conn_buff[i].buff == NULL) {
			return false;
		}
	}

	return true;
}

// 当前系统时间打印函数
void xyftp_print_time()
{
	char	  *wday[] = { "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" };
	time_t 	  timep;
	struct	  tm *p;
	
	time(&timep);
	p = localtime(&timep);  //取得当地时间
	printf("%02d/%02d/%02d ", 1900+p->tm_year, 1 + p->tm_mon, p->tm_mday);
	printf("%s %02d:%02d:%02d", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
}
