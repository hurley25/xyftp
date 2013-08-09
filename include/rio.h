/*
 * =====================================================================================
 *
 *       Filename:  rio.h
 *
 *    Description:  RIO（Robust I/O） 函数的声明头文件
 *
 *        Version:  1.0
 *        Created:  2013年08月09日 15时37分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef RIO_H_
#define RIO_H_

/*
 * Using __builtin_constant_p(x) to ignore cases where the return
 * value is always the same.  This idea is taken from a similar patch
 * written by Daniel Walker.
 */
# ifndef likely
#  define likely(x)	(__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 1))
# endif
# ifndef unlikely
#  define unlikely(x)	(__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 0))
# endif

// RIO 无缓冲的 I/O 函数
size_t rio_readn(int fd, void *usrbuf, size_t n);
size_t rio_writen(int fd, void *usrbuf, size_t n);

#endif // RIO_H_
