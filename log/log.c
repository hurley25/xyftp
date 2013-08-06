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

// 写入日志信息
void xyftp_write_log(int level, char *info)
{
	switch (level) {
	case LOG_INFO:
		syslog(LOG_INFO, "log_info: %s", info);
		break;
	case LOG_WARNING:
		syslog(LOG_WARNING, "log_info: %s", info);
		break;
	case LOG_ERR:
		syslog(LOG_ERR, "log_info: %s", info);
		break;
	default: break;
	}
}

