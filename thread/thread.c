/*
 * =====================================================================================
 *
 *       Filename:  thread.c
 *
 *    Description:  线程相关
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时06分37秒
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
#include <pthread.h>

#include "ftp.h"
#include "thread.h"

// 创建一个具有脱离属性的线程
static int xyftp_create_thread_detached(pthread_t *thread, void *(*thread_func) (void *), void *arg)
{
	pthread_attr_t thread_attr;
			
	if (pthread_attr_init(&thread_attr)) {
		return -1;
	}
	
	if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED)) {
		return -1;
	}

	if (pthread_create(thread, &thread_attr, thread_func, arg) == -1) {
		return -1;
	}

	pthread_attr_destroy(&thread_attr);

	return 0;
}

// 获取一个线程处理客户连接
bool xyftp_get_thread(void *arg)
{
	pthread_t thread;

	if (xyftp_create_thread_detached(&thread, xyftp_thread_entry, arg) == -1) {
		return false;
	}

	return true;
}

