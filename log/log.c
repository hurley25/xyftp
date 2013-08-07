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

#include "xyftp.h"

// 定义错误日志缓冲区大小
#define ERR_BUFF 1024

// 处理输出信息
void xyftp_print_info(int level, char *output_info)
{
	char err_buff[ERR_BUFF];

#ifndef FTP_DEBUG
	if (errno == 0) {
		syslog(level, "%s\n", output_info);
	} else {
		strerror_r(errno, err_buff, ERR_BUFF);
		syslog(level, "%s - %s", output_info, err_buff);
	}
#else
	time_t 	  timep;
	struct	  tm *p;
	
	time(&timep);
	p = localtime(&timep);

	if (errno == 0) {
		fprintf(stderr, "%02d:%02d:%02d Debug Info: %s\n",
				p->tm_hour, p->tm_min, p->tm_sec, output_info);
	} else {
		strerror_r(errno, err_buff, ERR_BUFF);
		fprintf(stderr, "%02d:%02d:%02d Debug Info: %s - %s\n",
				p->tm_hour, p->tm_min, p->tm_sec, output_info, err_buff);
	}
#endif	
}

