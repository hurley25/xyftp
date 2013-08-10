/*
 * =====================================================================================
 *
 *       Filename:  parse_cmd.h
 *
 *    Description:  解析 PDF 命令行
 *
 *        Version:  1.0
 *        Created:  2013年08月09日 23时27分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef PARSE_CMD_H_
#define PARSE_CMD_H_

// 最多计划支持 23 个命令
#define MAX_FTP_COMMAND 23

// 解析读取到的内容
client_state_t xyftp_parse_cmd(user_env_t *user_env, xyftp_buffer_t *conn_buff);

// 从缓冲区中解析出命令
int xyftp_anaylse_buff(ftp_cmd_t *recv_cmd, xyftp_buffer_t *conn_buff);

#endif 	// PARSE_CMD_H_
