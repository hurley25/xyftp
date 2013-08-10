/*
 * =====================================================================================
 *
 *       Filename:  runtime.h
 *
 *    Description:  服务器运行时的一些函数
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 22时20分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef RUNTIME_H_
#define RUNTIME_H_

// 声明客户连接的数据缓冲区指针
extern xyftp_buffer_t *conn_buff_global;

// 声明线程池指针
extern thread_pool_t *thread_pool_global;

// 初始化运行状态
bool xyftp_init();

// 清理服务器占据资源
void xyftp_destroy();

// 重置一个数据缓冲区
bool xyftp_reset_one_buff(xyftp_buffer_t *buff);

// 增大一个数据缓冲区
bool xyftp_resize_one_buff(xyftp_buffer_t *buff);

// 判断当前执行用户是否为 root
bool xyftp_is_root();

// 修改根目录为用户配置，且转换到一般用户去执行
bool xyftp_chroot();

// 当前系统时间打印函数
void xyftp_print_time();

#endif 	// RUNTIME_H_
