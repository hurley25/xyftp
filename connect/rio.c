/*
 * =====================================================================================
 *
 *       Filename:  rio.c
 *
 *    Description:  RIO（Robust I/O） 函数的实现
 *
 *        Version:  1.0
 *        Created:  2013年08月09日 15时44分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "xyftp.h"

size_t rio_readn(int fd, void *usrbuf, size_t n)
{
	size_t nleft = n;
	size_t nread = 0;
	char   *bufp = usrbuf;

	while (nleft > 0) {
		if ((nread = read(fd, bufp, nleft)) < 0) {
			if (errno == EINTR) { 	// Interrupted by sig handler return
				nread = 0; 	// and call read() again
			} else {
				return -1; 	// errno set by read()
			}
		} else if (nread == 0) {
			break; 		// EOF
		}
		nleft -= nread;
		bufp += nread;
	}

	return (n - nleft); 	// return >= 0
}

size_t rio_writen(int fd, void *usrbuf, size_t n)
{
	size_t nleft = n;
	size_t nwritten = 0;
	char   *bufp = usrbuf;
	
	while (nleft > 0) {
		if ((nwritten = write(fd, bufp, nleft)) <= 0) {
			if (errno == EINTR) { 	// Interrupted by sig handler return
				nwritten = 0; 	// and call write() again
			} else {
				return -1; 	// errno set by write()
			}
		}
		nleft -= nwritten;
		bufp += nwritten;
	}

	return n;
}

