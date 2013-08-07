/*
 * =====================================================================================
 *
 *       Filename:  connect.c
 *
 *    Description:  连接处理
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 11时06分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "xyftp.h"

// 服务器等待连接函数
bool xyftp_accept_client()
{
	int listen_fd;
	int accept_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		xyftp_print_info(LOG_ERR, "Create Socket Error, What The Fuck !?");
		return false;
	}
	
	int optval = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0) {
		xyftp_print_info(LOG_ERR, "Set Socket SO_REUSEADDR Error!");
		return false;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		xyftp_print_info(LOG_ERR, "Bind Scoket Error!");
		return false;
	}

	if (listen(listen_fd, LISTEN_SIZE) < 0) {
		xyftp_print_info(LOG_ERR, "Listen Socket Error!");
		return false;
	}

	socklen_t client_addr_len = sizeof(client_addr);
	while (true) {
		bzero(&client_addr, sizeof(client_addr));
		accept_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (accept_fd < 0) {
			xyftp_print_info(LOG_ERR, "Accept Socket Error!");
			return false;
		}

		char info_buf[100];
		sprintf(info_buf, "New Connection Create. Client IP : %s\n", inet_ntoa(client_addr.sin_addr));
		xyftp_print_info(LOG_INFO, info_buf);

		// 传值，注意在线程函数内部读取值
		if (!xyftp_get_thread((void *)accept_fd)) {
			xyftp_print_info(LOG_ERR, "Get Thread Error!");
		}
	}
}
