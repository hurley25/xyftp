/*
 * =====================================================================================
 *
 *       Filename:  ftp.h
 *
 *    Description:  FTP 协议解析处理相关函数
 *
 *        Version:  1.0
 *        Created:  2013年08月07日 10时45分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef FTP_H_
#define FTP_H_

// 定义单用户连接过程中可能的各个状态
typedef enum client_state {
	state_conn      = 0, 	// 刚连接
	state_try_login = 1, 	// 未验证
	state_login     = 2, 	// 已登录成功
	state_data      = 3, 	// 满足数据发送要求
	state_quit      = 4, 	// 用户退出
	state_close     = 5 	// 连接关闭
}client_state_t;

// FTP 命令最长也就 4 个字母
#define MAX_CMD 5

// 最长的参数是路径
#define MAX_ARG MAX_PATH

// 定义FTP命令结构体
typedef struct ftp_cmd{
	char	cmd[MAX_CMD];
	char	arg[MAX_ARG];
}ftp_cmd_t;

// 客户处理线程的入口函数
void *xyftp_thread_job_entry(void *arg);

// 向客户端发送一条消息
inline bool xyftp_send_client_msg(int conn_fd, char *msg);

// 解析读取到的内容
client_state_t xyftp_parse_com(user_env_t *user_env, xyftp_buffer_t *conn_buff);

#endif 	// FTP_H_
