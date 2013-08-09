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

#ifdef FTP_DEBUG
	xyftp_print_info(LOG_INFO, "A Job Create!");
#endif
	// 客户端连接刚建立状态
	client_state_t client_state = state_conn;

	// 只要用户不处于已退出状态就保持状态机循环
	while (client_state != state_close) {
		switch (client_state) {
		case state_conn:
			if (!xyftp_send_client_msg(user_env.conn_fd, ftp_send_msg[FTP_WELCOME])) {
				xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
				return NULL;
			}
			client_state = state_try_login;
			break;
		case state_try_login:
			client_state = state_quit;
			break;
		case state_login:
			break;
		case state_data:
			break;
		case state_quit:
			if (!xyftp_send_client_msg(user_env.conn_fd, ftp_send_msg[FTP_BYE])) {
				xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
				return NULL;
			}
			client_state = state_close;
			break;
		default:
			break;
		}
	}

	close(user_env.conn_fd);

#ifdef FTP_DEBUG
	xyftp_print_info(LOG_INFO, "A Job Exit!");
#endif

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
