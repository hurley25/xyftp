/*
 * =====================================================================================
 *
 *       Filename:  do_cmd.h
 *
 *    Description:  命令解析处理
 *
 *        Version:  1.0
 *        Created:  2013年08月10日 10时29分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef DO_CMD_H_
#define DO_CMD_H_

// 命令处理的函数指针
typedef int (*p_do_cmd_func)(user_env_t *, ftp_cmd_t *);

// 命令解析函数指针的数组
extern p_do_cmd_func do_cmd_func[];

#endif 	// DO_CMD_H_
