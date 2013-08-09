/*
 * =====================================================================================
 *
 *       Filename:  parse_cmd.c
 *
 *    Description:  解析 FTP 命令行
 *
 *        Version:  1.0
 *        Created:  2013年08月09日 23时25分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "xyftp.h"

// 从缓冲区中解析出命令
static int xyftp_anaylse_buff(ftp_cmd_t *recv_cmd, xyftp_buffer_t *conn_buff);

// 解析读取到的内容
client_state_t xyftp_parse_cmd(user_env_t *user_env, xyftp_buffer_t *conn_buff)
{
	ftp_cmd_t recv_cmd;
	if (xyftp_anaylse_buff(&recv_cmd, conn_buff) == -1 && user_env->is_login_in == false) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_NO_USER_PASS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return state_close;
		}
	} else if (xyftp_anaylse_buff(&recv_cmd, conn_buff) == -1) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_NO_USER_PASS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return state_close;
		}
	}

	return state_login;
}

// 从缓冲区中解析出命令
static int xyftp_anaylse_buff(ftp_cmd_t *recv_cmd, xyftp_buffer_t *conn_buff)
{
	return -1;
}

