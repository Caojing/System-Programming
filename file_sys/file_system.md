####ext2文件
	-ext2文件系统将整个分区划分为若干个同样大小的块组
	-每个块组包括一下几个部分
		-超级块（super block）（0）
		-块组描述副表（2-5）
		-块位图（6）
		-inode位图（7）
		-inode表（8-23）
		-数据块（24-1023）
	-启动块大小为lkb，也就是（0x0--0x400）
	-文件系统最小单位是块
	-dd if=/dev/zero of=fs count=256 bs=4k
	-mke2fs fs（格式化）
	-hexdump -d fs
	-mkdir tmp
	-sudo mount -o loop(常规文件不是块设备文件mount -t ) fs tmp/(将fs挂载到tmp下)
	-cd tmp(出现lost+found)（失物招领）	
	-启动块之后才是ext2文件系统的开始
###ext2文件重要结论
	-由超级块得到块组描述符地址
		-（struct ext2_group_desc *）get_block(group_size)
		-group_size == get_super_block()->s_first_data_block + 1;
		-get_block() == return (void *）(p + block * block_size);
	-由块组描述符得到inode表的地址：(struct ext2_inode *)get_inode(int inode)
		-int inode_table_block == (struct ext2_group_desc *)get_block(group_size)->bg_inode_table;
		-inode --;
		-return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
	-由inode表得到数据段地址：
		-pdir = （struct ext2_dir_entry_2 *）（get_block(get_inode(2)->i_block【0】)
###虚拟文件系统（VFS）
	 	看起来用起来都一样，因为把它放在抽象层了。
		-输入重定向
			fd = open(filename, O_RDONLY);
			dup2(fd, 0);
			close(fd);
		-输出重定向
			fd = open(filename, O_WRONLY);
			dup2(fd, 1);
			close(fd);
