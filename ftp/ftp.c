/*
 * =====================================================================================
 *
 *       Filename:  ftp.c
 *
 *    Description:  FTP 协议解析处理相关函数
 *
 *        Version:  1.0
 *        Created:  2013年08月07日 10时44分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "xyftp.h"

// 客户处理线程的入口函数
void *xyftp_thread_entry(void *arg)
{ 	
	int conn_fd = (int)arg;
	int pipe_fd[2];

	if (pipe(pipe_fd) == -1) {
		xyftp_print_info(LOG_ERR, "Create Pipe Error!");
		pthread_exit(0);
	}

	splice(conn_fd, NULL, pipe_fd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
	splice(pipe_fd[0], NULL, conn_fd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);

	close(conn_fd);

	xyftp_print_info(LOG_INFO, "A Thread Exit!");

	return NULL;
}
