/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  配置文件读取处理
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 13时16分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#ifndef MAX_PATH
#define MAX_PATH 4096
#endif

#define BUF_SIZE 1024
#define KEY_SIZE 50

// 全局配置定义
typedef struct xyftp_config
{
	bool log_enable; 		// 是否记录日志
	char ftp_path[MAX_PATH]; 	// FTP 服务器默认目录
	int max_clients; 		// 允许的最大客户数目与线程池上限
	int thread_pool_size; 		// 默认的线程池大小
	int thread_pool_add_size; 	// 线程池每次增加大小
	unsigned short ftp_port; 	// FTP使用的端口号
	bool anonymous_enable; 		// 是否允许匿名登录
	bool upload_enable;		// 是否允许上传
	bool download_enable;		// 是否允许下载
	unsigned int local_umask; 	// 上传文件权限
}xyftp_config_t;

// 全局配置声明
extern xyftp_config_t config_global;

// 读取配置文件
bool xyftp_read_config();

#endif 	// CONFIG_H_
