###函数接口
	###open
		-int open(const char * pathname, int flags, ...)
		-int open(const char * pathname, int flags, mode_t mode(文件权限));
		##以写的方式打开时候必须明确指出O_TRUNC才会截断文件，否则在原来数据上面改。
		##调用成功返回文件描述符，失败则返回-1；
	###read
		-ssize_t read(int fd, void *buf, size_t count(请求读取字节数));
		-读出来数据存在缓冲区，同时文件中的读写位置向后移。
	###阻塞和非阻塞
		-阻塞是读的超过限制时候他就会发生阻塞，不退出也不进行
		-非阻塞是读得超过额定的时候他就把没超过的读出来，然后退出
		
