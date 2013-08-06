/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  程序入口函数
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时03分15秒
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

#include "connect.h"
#include "runtime.h"
#include "config.h"

int main(int argc, char *argv[])
{
	// 读取配置文件
	if (!xyftp_read_config()) {
		exit(-1);
	}

	// 初始化服务器资源
	if(!xyftp_init()) {
		exit(-1);
	}

	// 进入网络连接处理的主循环
	if (!xyftp_accept_client()) {
		exit(-1);
	}

	return EXIT_SUCCESS;
}
