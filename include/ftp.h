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

// 客户处理线程的入口函数
void *xyftp_thread_job_entry(void *arg);

#endif 	// FTP_H_
