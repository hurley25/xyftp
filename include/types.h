/*
 * =====================================================================================
 *
 *       Filename:  types.h
 *
 *    Description:  一些类型的定义
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时04分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef enum _bool {
	false = 0,
	true = 1
}bool;

// 定义服务器数据缓冲区类型
typedef struct  xyftp_buffer {
	char *buff;
	int  size;
	int  len;
}xyftp_buffer_t;

#endif 	// TYPES_H_
