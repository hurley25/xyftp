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

// 标准 FTP 服务器 支持的命令(按照本程序支持的先后顺序排序)
static char *ftp_command[] = {
/*    0   */		"USER", 	// 登录的用户名
/*    1   */		"PASS", 	// 密码
/*    2   */		"QUIT", 	// 退出
/*    3   */		"REIN", 	// 重新登录
/*    4   */		"CWD", 		// 改变当前目录
/*    5   */		"CDUP", 	// 返回上级目录
/*    6   */		"RNFR", 	// 指定需要改名的原文件
/*    7   */		"RNTO", 	// 指定需要改名的新文件名
/*    8   */		"ABOR", 	// 取消前一指令
/*    9   */		"DELE", 	// 在服务器上删除指定文件
/*   10   */		"RMD", 		// 在服务器上删除指定目录
/*   11   */		"MKD", 		// 在服务器上创建目录       
/*   12   */		"PWD", 		// 显示当前目录
/*   13   */		"LIST", 	// 列出当前目录下所有文件和子目录
/*   14   */		"NLST", 	// 列出当前目录下所有文件和子目录的名字
/*   15   */		"PORT", 	// IP 地址和两字节的端口
/*   16   */		"TYPE", 	// 设置数据交换的类型{A(SCII),E(BCDIC),I(mage),L(ocal byte size)}
/*   17   */		"PASV", 	// 获取数据传输端口
/*   18   */		"RETR", 	// 从服务器上找回（复制）文件
/*   19   */		"STOR", 	// 用覆盖方式上传一个文件到服务器
/*   20   */		"APPE", 	// 用追加方式上传一个文件到服务器
/*   21   */		"HELP", 	// 获取帮助
/*   22   */		"NOOP", 	// 无动作，除了来自服务器上的承认
		
// 以下命令不准备支持...直接返回 Unknow Command　　　
/*   23   */		"SMNT", 	// 挂载指定文件结构
/*   24   */		"ACCT", 	// 系统特权帐号
/*   25   */		"STRU", 	// 数据结构（F=文件，R=记录，P=页面）
/*   26   */		"MODE", 	// 传输模式（S=流模式，B=块模式，C=压缩模式）
/*   27   */		"STOU", 	// 储存文件到服务器名称上
/*   28   */		"ALLO", 	// 为服务器上的文件存储器分配字节
/*   29   */		"REST", 	// 指定重新下载的字节数
/*   30   */		"SITE", 	// 由服务器提供的站点特殊参数
/*   31   */		"SYST", 	// 返回服务器使用的操作系统
/*   32   */		"STAT" 		// 在当前程序或目录上返回信息
};

// 解析读取到的内容
client_state_t xyftp_parse_cmd(user_env_t *user_env, xyftp_buffer_t *conn_buff)
{
	ftp_cmd_t recv_cmd;
	int cmd_index = xyftp_anaylse_buff(&recv_cmd, conn_buff);

	// 未登录的时候拒绝一切非登录指令
	if (cmd_index != 0 && cmd_index != 2 && user_env->is_login_in == false) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_NO_USER_PASS])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return state_close;
		}
	// 已登录遭遇位置指令回复无法识别
	} else if (cmd_index == -1 && user_env->is_login_in == true) {
		if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_E_UNKNOW_CMD])) {
			xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			return state_close;
		}
	// 请求登录或者已登录发送命令
	} else {
		// 通过索引调用指令处理函数
		int cmd_state = do_cmd_func[cmd_index](user_env, &recv_cmd);
		// 用户请求退出
		if (cmd_state == -1) {
			if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_BYE])) {
				xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
			}
			return state_close;
		// 指令成功执行
		} else if (cmd_state == 1) {
			if (!xyftp_send_client_msg(user_env->conn_fd, ftp_send_msg[FTP_S_COMMAND])) {
				xyftp_print_info(LOG_INFO, "Write Data To Client Error!");
				return state_close;
			}
		}
		// 指令出错在指令处理函数里发送错误
		// 函数返回 0 则不处理
	}

	return state_login;
}

// 从缓冲区中解析出命令
int xyftp_anaylse_buff(ftp_cmd_t *recv_cmd, xyftp_buffer_t *conn_buff)
{
	// 命令行中空格的位置
	int blank_index = 0;

	// 最短的命令 + '\n' 也有 4 字节
	if (conn_buff->len < 4) {
		return -1;
	}

	bool has_arg = true;

	// 格式不正确不考虑粘包处理,直接反馈无法识别
	// 命令 3-4 个字节
	if (conn_buff->buff[3] == ' ') {
		blank_index = 3;
	} else if (conn_buff->buff[4] == ' ') {
		blank_index = 4;
	} else {
		if (conn_buff->buff[3] == '\r' || conn_buff->buff[3] == '\n') {
			blank_index = 3;
		} else if (conn_buff->buff[4] == '\r' || conn_buff->buff[4] == '\n') {
			blank_index = 4;
			has_arg = false;
		} else {
			return -1;
		}
	}

	// 拷贝命令
	strncpy(recv_cmd->cmd, conn_buff->buff, blank_index);
	recv_cmd->cmd[blank_index] = '\0';

	if (has_arg) {
		// 拷贝参数，按照惯例支持 \r\n 和 \n
		if (conn_buff->buff[conn_buff->len-2] == '\r' && conn_buff->buff[conn_buff->len-1] == '\n') {
			strncpy(recv_cmd->arg, conn_buff->buff+blank_index+1, conn_buff->len-blank_index-1);
			recv_cmd->arg[conn_buff->len-blank_index-1] = '\0';
		} else if (conn_buff->buff[conn_buff->len-1] == '\n') {
			strncpy(recv_cmd->arg, conn_buff->buff+blank_index+1, conn_buff->len-blank_index-2);
			recv_cmd->arg[conn_buff->len-blank_index-1] = '\0';
		} else {
			// 命令没读取完，此时最好应该判断是否断包
			return -1;
		}
	}

#ifdef FTP_DEBUG
	printf("Debug Info : A Client Send A Command. cmd : %s  arg : %s\n", recv_cmd->cmd, recv_cmd->arg);
#endif
	// 命令字母转换到大写
	int i;
	for (i = 0; i < MAX_CMD; i++) {
		recv_cmd->cmd[i] &= 0x5F;
	}

	// 比较参数并返回索引
	int index;
	for (index = 0; index < MAX_FTP_COMMAND; index++) {
		if (strcmp(ftp_command[index], recv_cmd->cmd) == 0) {
			return index;
		}		
	}

	return -1;
}

