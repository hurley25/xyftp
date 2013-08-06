/*
 * =====================================================================================
 *
 *       Filename:  log.c
 *
 *    Description:  日志处理
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时05分37秒
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

#include "log.h"

// 处理输出信息
void xyftp_print_info(int level, char *info)
{
	switch (level) {
	case LOG_INFO:
#ifndef FTP_DEBUG
	syslog(LOG_INFO, "log_info: %s", info);
#else
	fprintf(stderr, "log_info: %s\n", info);
#endif
		break;
	case LOG_WARNING:
#ifndef FTP_DEBUG
	syslog(LOG_WARNING, "log_info: %s", info);
#else
	fprintf(stderr, "log_warning: %s\n", info);
#endif
		break;
	case LOG_ERR:
#ifndef FTP_DEBUG
	syslog(LOG_ERR, "log_info: %s", info);
#else
	fprintf(stderr, "log_error: %s\n", info);
#endif
		break;
	default: break;
	}
}
