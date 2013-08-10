/*
 * =====================================================================================
 *
 *       Filename:  do_cmd.c
 *
 *    Description:  命令解析处理
 *
 *        Version:  1.0
 *        Created:  2013年08月10日 10时28分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "xyftp.h"

// 各个命令的解析函数，从函数名称就能看出来用途，不解释
int do_user(user_env_t *user_env, ftp_cmd_t *cmd)
{
	xyftp_buffer_t *conn_buff = &conn_buff_global[user_env->conn_fd];
	ftp_cmd_t recv_cmd;

	// 暂时只有匿名用户，其他用户的登录暂不支持
	if (config_global.anonymous_enable == false) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_LOGIN])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
		}
		return -1;
	} else if (user_env->is_login_in == true) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_S_ANONYMOUS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return -1;
		}
	} else if (strcmp(cmd->arg, "anonymous") == 0) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_NEED_PASS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return -1;
		}
		strcpy(user_env->user_name, "anonymous");
		assert(conn_buff->len >= 0 && conn_buff->len < conn_buff->size);
		bzero(conn_buff->buff, conn_buff->len);
		conn_buff->len = read(user_env->conn_fd, conn_buff->buff, conn_buff->size);
		if (conn_buff->len <= 0) {
			xyftp_print_info(LOG_INFO, "Read Data From Client Error!");
			return -1;
		}
		int cmd_index = xyftp_anaylse_buff(&recv_cmd, conn_buff);
		if (cmd_index == 1 && strcmp(recv_cmd.arg, "anonymous") == 0) {
			user_env->is_login_in = true;
			if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_S_ANONYMOUS])) {
				xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
				return -1;
			}
			return 0;
		} else {
			if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_NO_USER_PASS])) {
				xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
				return -1;
			}
		}
	} else {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_OTHER_USER])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return -1;
		}
	}

	return 0;
}

int do_pass(user_env_t *user_env, ftp_cmd_t *cmd)
{
	if (user_env->is_login_in == true) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_S_ANONYMOUS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return -1;
		}
	} else {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_NO_USER_PASS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return -1;
		}
	}

	return 0;
}

int do_quit(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return -1;
}

int do_rein(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_cwd(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_cdup(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_rnfr(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_rnto(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_abor(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_dele(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_rmd(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_mkd(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_pwd(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_list(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_nlst(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_port(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_type(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_pasv(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_retr(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_stor(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_appe(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_help(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_noop(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

// 命令解析函数指针的数组
p_do_cmd_func do_cmd_func[MAX_FTP_COMMAND] = {
			do_user, do_pass, do_quit, do_rein,
			do_cwd, do_cdup, do_rnfr, do_rnto,
			do_abor, do_dele, do_rmd, do_mkd,
			do_pwd, do_list, do_nlst, do_port,
			do_type, do_pasv, do_retr, do_stor,
			do_appe, do_help, do_noop
};
