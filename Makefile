#!Makefile

# ------------------------------------------------------------------------
#
# 项目的 Makefile 示例，对于此结构代码，该 Makefile 均通用。
#
# ------------------------------------------------------------------------

# 可以类似这样执行 shell 脚本，find 命令的结果作为 C_SOURCES 变量的内容
# patsubst 貌似是内置函数，它处理所有在 C_SOURCES 字列中的字（一列文件名）
# 如果它的 结尾是 '.c'，就用 '.o' 把 '.c' 取代
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc

C_FLAGS = -c -Wall -Iinclude

all: $(C_OBJECTS) 
	@echo 链接 ...
	$(CC) $(C_OBJECTS) -o xyftp

# The automatic variable `$<' is just the first prerequisite
# $@表示规则的当前目标文件名
.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.PHONY:clean
clean:
	@echo 清理临时文件
	$(RM) $(C_OBJECTS) xyftp

