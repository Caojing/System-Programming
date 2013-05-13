#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define __le32	int 
#define __le16	short
#define __u8	char
#define __u16	short
#define __u32	int

struct ext2_super_block {
          __le32  s_inodes_count;         /* Inodes count */
          __le32  s_blocks_count;         /* Blocks count */
          __le32  s_r_blocks_count;       /* Reserved blocks count */
          __le32  s_free_blocks_count;    /* Free blocks count */
          __le32  s_free_inodes_count;    /* Free inodes count */
          __le32  s_first_data_block;     /* First Data Block */
          __le32  s_log_block_size;       /* Block size */
          __le32  s_log_frag_size;        /* Fragment size */
          __le32  s_blocks_per_group;     /* # Blocks per group */
          __le32  s_frags_per_group;      /* # Fragments per group */
          __le32  s_inodes_per_group;     /* # Inodes per group */
          __le32  s_mtime;                /* Mount time */
          __le32  s_wtime;                /* Write time */
          __le16  s_mnt_count;            /* Mount count */
          __le16  s_max_mnt_count;        /* Maximal mount count */
          __le16  s_magic;                /* Magic signature */
          __le16  s_state;                /* File system state */
	  __le16  s_errors;               /* Behaviour when detecting errors */
          __le16  s_minor_rev_level;      /* minor revision level */
          __le32  s_lastcheck;            /* time of last check */
          __le32  s_checkinterval;        /* max. time between checks */
          __le32  s_creator_os;           /* OS */
          __le32  s_rev_level;            /* Revision level */
          __le16  s_def_resuid;           /* Default uid for reserved blocks */
          __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
           * These fields are for EXT2_DYNAMIC_REV superblocks only.
           *
           * Note: the difference between the compatible feature set and
           * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
		


          *know about, it should refuse to mount the filesystem.
           * e2fsck's requirements are more strict; if it doesn't know
           * about a feature in either the compatible or incompatible
           * feature set, it must abort and not try to meddle with
           * things it doesn't understand...
           */
          __le32  s_first_ino;            /* First non-reserved inode */
          __le16   s_inode_size;          /* size of inode structure */
          __le16  s_block_group_nr;       /* block group # of this superblock     */
          __le32  s_feature_compat;       /* compatible feature set */
          __le32  s_feature_incompat;     /* incompatible feature set */
          __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
          __u8    s_uuid[16];             /* 128-bit uuid for volume */
          char    s_volume_name[16];      /* volume name */
          char    s_last_mounted[64];     /* directory where last mounted */
          __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
	  __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
  };
  
  /*
   * Structure of a blocks group descriptor
  */
  struct ext2_group_desc
  {
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
         __le32  bg_reserved[3];
 };
 /*
  * Constants relative to the data blocks
 */
 #define EXT2_NDIR_BLOCKS                12
 #define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
 #define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
 #define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
 #define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
/*
  * Structure of an inode on the disk
*/
 struct ext2_inode {
         __le16  i_mode;         /* File mode */
         __le16  i_uid;          /* Low 16 bits of Owner Uid */
         __le32  i_size;         /* Size in bytes */
         __le32  i_atime;        /* Access time */
         __le32  i_ctime;        /* Creation time */
         __le32  i_mtime;        /* Modification time */
         __le32  i_dtime;        /* Deletion Time */
         __le16  i_gid;          /* Low 16 bits of Group Id */
         __le16  i_links_count;  /* Links count */
         __le32  i_blocks;       /* Blocks count */
         __le32  i_flags;        /* File flags */
         union {
                 struct {
                         __le32  l_i_reserved1;
                 } linux1;
                 struct {
                         __le32  h_i_translator;
                 } hurd1;
                 struct {
                        __le32  m_i_reserved1;
                 } masix1;
         } osd1;                         /* OS dependent 1 */
	__le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
 }; 
#define EXT2_NAME_LEN 255
struct ext2_dir_entry_2 {
         __le32  inode;                  /* Inode number */
         __le16  rec_len;                /* Directory entry length */
         __u8    name_len;               /* Name length */
         __u8    file_type;
         char    name[EXT2_NAME_LEN];    /* File name */
};

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#endif

int inode_count = 0;
int block_count = 0;
int group_block = 0;
int inode_size = 0;
int block_size = 0;
char *p = NULL;

void myputns(char *buf, int len)
{
	int i = 0;
	
	for(i = 0; i < len; i++)
		putchar(buf[i]);
}

struct ext2_super_block *get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void *get_block(int block)
{
	return (void *)(p + block * block_size);
}

struct ext2_inode *get_inode(int inode)
{
	int inode_table_block;
	
	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;
	
	inode--;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
        struct ext2_dir_entry_2 *pdir;

	pdir = (struct ext2_dir_entry_2 *)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINT(pdir->inode);
		PRINT(pdir->rec_len);
		PRINT(pdir->name_len);
		myputns(pdir->name, pdir->name_len);
		putchar('\t');

		len += pdir->rec_len;

		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
	
	putchar('\n');

}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir;
	int len = 0;

	pdir = (struct ext2_dir_entry_2	*)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINTP(filename);
		PRINTP(pdir->name);
		PRINTP(pdir->name_len);
		PRINT(strlen(filename));
		
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}

}

void cat(char *filename)
{
	struct ext2_inode *inode;
	int i = 0;

	i = namei(filename);

	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf(" %s not found\n", filename);

	return ;
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd = 0;
	int tmp;

	if(argc >= 2)
		filename = argv[1];
	
	fd = open(filename, O_RDWR);
	
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << (get_super_block()->s_log_block_size);

	PRINT(inode_size); 
	PRINT(block_size);

	group_block = get_super_block()->s_first_data_block + 1;
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_table);

	PRINT(get_inode(2)->i_block[0]);

#if 0
	printf("<<<<<<<<<<1.ls()>>>>>>>>>>>>>>>\n");
	printf("<<<<<<<<<<2.cat()>>>>>>>>>>>>>>\n");

	scanf("%d", &tmp);
	printf("myshell: tmp\n");
	switch(tmp){


		case 1:	
				ls();
//		case 2:
//				cat();
		default:
				break;
	}
#endif	
	while(1)
	{
		char buf[64];
		char cmd[32];
		char arg[32];
		int ret = 0;

		printf("myshell # ");

		fgets(buf, 64, stdin);
		ret = sscanf(buf, "%s %s", cmd, arg);

		if(ret == 0)
			continue;

		PRINTP(cmd);
		PRINTP(arg);
	
		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
		
	}
	
	munmap(p, 1024*1024);
	return 0;
}
