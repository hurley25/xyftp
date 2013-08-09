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
void *xyftp_thread_job_entry(void *arg)
{ 	
	user_env_t user_env;
	user_env.conn_fd = (int)arg;

	xyftp_print_info(LOG_INFO, "A Job Create!");

	if (!xyftp_send_client_msg(user_env.conn_fd, ftp_send_msg[FTP_WELCOME])) {
		xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
		close(user_env.conn_fd);
		return NULL;
	}

	close(user_env.conn_fd);

	xyftp_print_info(LOG_INFO, "A Job Exit!");

	return NULL;
}

// 向客户端发送一条消息
bool xyftp_send_client_msg(int conn_fd, char *msg)
{
	if (rio_writen(conn_fd, msg, strlen(msg)) == -1) {
		return false;
	}

	return true;
}
