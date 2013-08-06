/*
 * =====================================================================================
 *
 *       Filename:  error.c
 *
 *    Description:  错误处理
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时05分57秒
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
#include <errno.h>
#include "error.h"

// 声明全局错误信息
char *err_msg[] = {
/*  0  */ 	"System Error!", 
/*  1  */ 	"Config File Format Error!"
};

#define ERR_BUFF 1024

// 处理输出信息
void xyftp_print_info(int level, int err_no)
{
	char err_buff[ERR_BUFF];
#ifndef FTP_DEBUG
	if (err_no != ERR_SYSTEM) {
		xyftp_write_log(level, err_msg[err_no]);
	} else {
		// 多线程的可重入版 strerror
		strerror_r(errno, err_buff, ERR_BUFF);
		xyftp_write_log(level, err_buff, ERR_BUFF);
	}
#else
	if (err_no != ERR_SYSTEM) {
		fprintf(stderr, "Debug Info: %s\n", err_msg[err_no]);
	} else {
		strerror_r(errno, err_buff, ERR_BUFF);
		fprintf(stderr, "Debug Info: %s\n", err_buff);
	}
#endif	
}

