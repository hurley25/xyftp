/*
 * =====================================================================================
 *
 *       Filename:  xyftp.h
 *
 *    Description:  项目的公共头文件
 *
 *        Version:  1.0
 *        Created:  2013年08月07日 12时53分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef XYFTP_H_
#define XYFTP_H_

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>

#include "types.h"
#include "connect.h"
#include "thread.h"
#include "thread_pool.h"
#include "runtime.h"
#include "config.h"
#include "rio.h"
#include "log.h"
#include "ftp.h"
#include "status.h"
#include "parse_cmd.h"

// 服务器名称
#define SERVER_NAME "xyftp Server"

// 服务器端口
#ifdef FTP_DEBUG
	#define SERV_PORT 9527
#else
	#define SERV_PORT 21
#endif

// 服务器最大连接数
#define MAX_CONNECT_SIZE 1024

// 服务端最大支持用户数
#define MAX_CONNECT_USER 100

// 默认线程池大小
#define THREAD_POOL_SIZE 10

// 线程池每次增加粒度
#define THREAD_POOL_ADD_SIZE 10

// listen() 的 backlog 队列长度
#define LISTEN_SIZE 20

// 接收缓冲区默认尺寸
#define BUFF_LENGTH 1024

// 接收缓冲区增加粒度(经验值)
#define BUFF_ADD_LENGTH 128

#endif 	// XYFTP_H_

