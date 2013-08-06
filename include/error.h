/*
 * =====================================================================================
 *
 *       Filename:  error.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时08分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "log.h"

// 声明全局错误信息
extern char *err_msg[];

// 错误编号定义
#define  ERR_SYSTEM  	   0
#define  ERR_CONFIG_FILE   1  	// 配置文件格式错误

// 处理输出信息
void xyftp_print_info(int level, int err_no);

#endif 	// ERROR_H_
