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

#include "xyftp.h"

// 客户连接的数据缓冲区指针
xyftp_buffer_t *conn_buff_global;

// 线程池指针
thread_pool_t *thread_pool_global;

// 为客户连接的数据缓冲区申请内存
static bool xyftp_malloc_buff();

// 为客户连接的数据缓冲区释放内存
static bool xyftp_free_buff();

// 初始化运行状态
bool xyftp_init()
{
	// 读取配置文件
	if (!xyftp_read_config()) {
		xyftp_print_info(LOG_ERR, "Read Config File Error!");
		return false;
	}

#ifndef FTP_DEBUG
	// 守护进程模式运行
	if (daemon(NULL, NULL) == -1) {
		xyftp_print_info(LOG_ERR, "Into Deamon Mode Error!");
		return false;
	}
#else
	printf("\n-------------------- " SERVER_NAME " --------------------\n\n");
	xyftp_print_time();
	printf(" Started\n\nWaiting ...\n\n");
#endif
	// 为客户连接的数据缓冲区申请内存
	if (!xyftp_malloc_buff()) {
		xyftp_print_info(LOG_ERR, "Alloc Buff Memery Error!");
		return false;
	}
	
	// 初始化线程池
	if ((thread_pool_global = thread_pool_init(THREAD_POOL_SIZE, THREAD_POOL_SIZE)) == NULL) {
		xyftp_print_info(LOG_ERR, "Thread Pool Init Error!");
		return false;
	}

	return true;
}

// 销毁服务器占据资源
void xyftp_destroy()
{
	// 释放线程数据接收缓存
	xyftp_free_buff();

	// 释放线程池资源
	thread_pool_destroy(thread_pool_global);
}

// 为客户连接的数据缓冲区申请内存
static bool xyftp_malloc_buff()
{
	conn_buff_global = (xyftp_buffer_t *)malloc(sizeof(xyftp_buffer_t) * MAX_CONNECT_SIZE);
	if (conn_buff_global == NULL) {
		return false;
	}

	int i;
	for (i = 0; i < MAX_CONNECT_SIZE; i++) {
		conn_buff_global[i].buff = (char *)malloc(BUFF_LENGTH);
		conn_buff_global[i].len = 0;
		conn_buff_global[i].size = 0;
		if (conn_buff_global[i].buff == NULL) {
			return false;
		}
	}

	return true;
}

// 为客户连接的数据缓冲区释放内存
static bool xyftp_free_buff()
{
	int i;
	for (i = 0; i < MAX_CONNECT_SIZE; i++) {
		if (conn_buff_global[i].buff != NULL) {
			free(conn_buff_global[i].buff);
		}
	}
	free(conn_buff_global);

	return true;
}

// 重置一个数据缓冲区
bool xyftp_reset_one_buff(xyftp_buffer_t *buff)
{
	if (buff != NULL) {
		buff->len = 0;
		return true;
	}

	return false;
}

// 增大一个数据缓冲区
bool xyftp_resize_one_buff(xyftp_buffer_t *buff)
{
	if (buff != NULL) {
		buff->buff = (char *)realloc(buff->buff, buff->size + BUFF_ADD_LENGTH);
		if (buff != NULL) {
			buff->size += BUFF_ADD_LENGTH;
			return true;
		}
	}

	return false;
}

// 当前系统时间打印函数
void xyftp_print_time()
{
	char	  *wday[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	time_t 	  timep;
	struct	  tm *p;
	
	time(&timep);
	p = localtime(&timep);
	printf("%02d/%02d/%02d ", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
	printf("%s %02d:%02d:%02d", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
}
