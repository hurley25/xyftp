/*
 * =====================================================================================
 *
 *       Filename:  config.c
 *
 *    Description:  配置文件读取处理
 *
 *        Version:  1.0
 *        Created:  2013年08月06日 13时15分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "config.h"

#define BUF_SIZE 1024
#define KEY_SIZE 50

// 全局配置定义
xyftp_config_t config_global;

// 读取配置文件
bool xyftp_read_config()
{
#ifndef FTP_DEBUG
	char *config_file = "/etc/xyftp.conf";
#else
	char *config_file = "xyftp.conf";
#endif
	FILE *config_stream;
	char read_buff[BUF_SIZE];
	char key[KEY_SIZE];
	char value[KEY_SIZE];

	// 简单处理下配置文件
	if ((config_stream = fopen(config_file, "r")) == NULL) {
		xyftp_print_info(LOG_ERR, "Cann't open the config file!");
		return false;
	}
	
	while (fscanf(config_stream, "%s\n", read_buff) > 0) {
		if (read_buff[0] == '#') {
			continue;
		} else if (read_buff[0] == '[') {
			while (fscanf(config_stream, "%s = %s\n", key, value) == 2) {
				if (strcmp("ftp_path", key) == 0) {
					strcpy(config_global.ftp_path, value);
				} else if (strcmp("anonymous_enable", key) == 0) {
					if (strcmp("YES", value) == 0) {
						config_global.anonymous_enable = true;
					} else {
						config_global.anonymous_enable = false;
					}
				} else if (strcmp("upload_enable", key) == 0) {
					if (strcmp("YES", value) == 0) {
						config_global.upload_enable = true;
					} else {
						config_global.upload_enable = false;
					}
				} else if (strcmp("download_enable", key) == 0) {
					if (strcmp("YES", value) == 0) {
						config_global.download_enable = true;
					} else {
						config_global.download_enable = false;
					}
				} else if (strcmp("log_enable", key) == 0) {
					if (strcmp("YES", value) == 0) {
						config_global.log_enable = true;
					} else {
						config_global.log_enable = false;
					}
				}
			}
		}
	}
	
	fclose(config_stream);

	if (config_global.ftp_path[0] == '\0') {
		xyftp_print_info(LOG_ERR, "Config file Error!");
		return false;
	}

#ifdef FTP_DEBUG
	printf("Config Info:\n");
	printf("ftp_path:%s\n", config_global.ftp_path);
	printf("anonymous_enable:%d\n", config_global.anonymous_enable);
	printf("upload_enable:%d\n", config_global.upload_enable);
	printf("download_enable:%d\n", config_global.download_enable);
	printf("log_enable:%d\n", config_global.log_enable);
#endif

	return true;
}

