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
int do_user(user_env_t *uner_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_pass(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
}

int do_quit(user_env_t *user_env, ftp_cmd_t *cmd)
{
	return 0;
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
