#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

#define BLKSIZE 1024

char buf[BLKSIZE], dbuf[BLKSIZE], tbuf[256];

int fd;
int iblock;
int rootblock;

char *dev = "diskimage"; //Default device

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}

super()
{
  // read SUPER block
  get_block(fd, 1, buf);  
  sp = (SUPER *)buf;
	
  printf("\t\t SUPERBLOCK \n\n");
  // check for EXT2 magic number:
  printf("s_magic = %x\n", sp->s_magic);
  if (sp->s_magic != 0xEF53){
    printf("NOT an EXT2 FS\n");
    exit(1);
  }

  printf("EXT2 FS OK\n");

  printf("s_inodes_count = %d\n", sp->s_inodes_count);
  printf("s_blocks_count = %d\n", sp->s_blocks_count);
  printf("s_free_inodes_count = %d\n", sp->s_free_inodes_count);
  printf("s_free_blocks_count = %d\n", sp->s_free_blocks_count);

  printf("s_first_data_blcok = %d\n", sp->s_first_data_block);

  printf("s_log_block_size = %d\n", sp->s_log_block_size);

  printf("s_blocks_per_group = %d\n", sp->s_blocks_per_group);
  printf("s_inodes_per_group = %d\n", sp->s_inodes_per_group);


  printf("s_mnt_count = %d\n", sp->s_mnt_count);
  printf("s_max_mnt_count = %d\n", sp->s_max_mnt_count);

  printf("s_magic = %x\n", sp->s_magic);

  printf("s_mtime = %s", ctime(&sp->s_mtime));
  printf("s_wtime = %s", ctime(&sp->s_wtime));
  printf("\n");
}

gd()
{	
  get_block(fd, 2, buf);  
  gp = (GD *)buf;

  printf("\t\tGROUP_DESCRIPTOR\t\t\n");

  printf("bg_block_bitmap: %d\n", gp->bg_block_bitmap);
  
  printf("bg_inode_bitmap: %d\n", gp->bg_inode_bitmap);

  printf("bg_inode_table: %d\n", gp->bg_inode_table);

  printf("bg_free_blocks_count: %d\n", gp->bg_free_blocks_count);
  printf("bg_free_inodes_count: %d\n", gp->bg_free_inodes_count);

  printf("bg_used_dirs_count: %d\n", gp->bg_used_dirs_count);
  printf("\n");

  iblock = gp->bg_inode_table;
}


int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit / 8;  j = bit % 8;
  return (buf[i] & (1 << j));
}

imap()
{
  char buf[BLKSIZE];
  int  imap, ninodes;
  int  i;

  printf("\t\t IMAP \n\n");

  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;

  ninodes = sp->s_inodes_count;
  printf("ninodes = %d\n", ninodes);

  // read Group Descriptor 0
  get_block(fd, 2, buf);
  gp = (GD *)buf;

  imap = gp->bg_inode_bitmap;
  printf("imap = %d\n", imap);

  // read inode_bitmap block
  get_block(fd, imap, buf);

  for (i=0; i < ninodes; i++){
    (tst_bit(buf, i)) ?	putchar('1') : putchar('0');
    if (i && ((i+1) % 8)==0)
       printf(" ");
    if(i && ((i+1) % 40) == 0)
	printf("\n");
  }
  printf("\n\n");
}


bmap()
{
  char buf[BLKSIZE];
  int  bmap, blocks;
  int  i;
  
  printf("\t\t BMAP \n\n");  

  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;

  blocks = sp->s_blocks_count;
  printf("blocks = %d\n", blocks);

  // read Group Descriptor 0
  get_block(fd, 2, buf);
  gp = (GD *)buf;

  bmap = gp->bg_block_bitmap;
  printf("bmap = %d\n", bmap);

  // read block_bitmap block
  get_block(fd, bmap, buf);

  for (i=0; i < blocks; i++){
    (tst_bit(buf, i)) ?	putchar('1') : putchar('0');
    if (i && ((i+1) % 8)==0)
       printf(" ");
    if(i && ((i+1) % 48) == 0)
	printf("\n");
  }
  printf("\n\n");
}

inode()
{
  char buf[BLKSIZE];

  printf("\t\t INODE \n\n");

  // read GD
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  /************ print these to see what they are  ****
  printf("%8d %8d %8d %8d %8d %8d\n",
	 gp->bg_block_bitmap,
	 gp->bg_inode_bitmap,
	 gp->bg_inode_table,
	 gp->bg_free_blocks_count,
	 gp->bg_free_inodes_count,
	 gp->bg_used_dirs_count);
  ***************************************************/ 
  iblock = gp->bg_inode_table;   // get inode start block#
  printf("inode_block=%d\n", iblock);

  // get inode start block     
  get_block(fd, iblock, buf);

  ip = (INODE *)buf + 1;         // ip points at 2nd INODE
  
  printf("mode=%4x ", ip->i_mode);
  printf("uid=%d  gid=%d\n", ip->i_uid, ip->i_gid);
  printf("size=%d\n", ip->i_size);
  printf("time=%s",   ctime(&ip->i_ctime));
  printf("link=%d\n", ip->i_links_count);
  printf("i_block[0]=%d\n", ip->i_block[0]);

 /*****************************
  u16  i_mode;                      // file type|permission bits
  u16  i_uid;                       // ownerID
  u32  i_size;                      // file size in bytes
  u32  i_atime;                     // time fields  
  u32  i_ctime;
  u32  i_mtime;
  u32  i_dtime;
  u16  i_gid;                       // groupID
  u16  i_links_count;               // link count
  u32  i_blocks;                    // IGNORE
  u32  i_flags;                     // IGNORE
  u32  i_reserved1;                 // IGNORE
  u32  i_block[15];                 // IMPORTANT, but later
 ***************************/
}

dir()
{
  int i;
  int temp;
  DIR* dp;
  char* cp;
  temp = ip->i_block[0];
  get_block(fd,temp, buf);
  
  dp = (DIR *)buf;
  cp = (char*)buf;
 
  printf("\t\t DIRECTORY \n\n");
  while(dp->name_len != 0)
  {
     printf("/%s",dp->name);
     cp += dp->rec_len;
     dp = (DIR*)cp;
  } 
 printf("\n");
}

inodeBegin()
{
  char buf[BLKSIZE];

  printf("\t\t Traverse \n\n");

  // read GD
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  /************ print these to see what they are  ****
  printf("%8d %8d %8d %8d %8d %8d\n",
	 gp->bg_block_bitmap,
	 gp->bg_inode_bitmap,
	 gp->bg_inode_table,
	 gp->bg_free_blocks_count,
	 gp->bg_free_inodes_count,
	 gp->bg_used_dirs_count);
  ***************************************************/ 
  iblock = gp->bg_inode_table;   // get inode start block #
  printf("inode_block=%d\n", iblock);
  // get inode start block     
  get_block(fd, iblock, buf);

  ip = (INODE *)buf + 1;         // ip points at 2nd INODE
  
}

int traverse(char *dev)
{
  int i;
  char *cp;
  
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  iblock = gp->bg_inode_table;

  printf("inodes begin block = %d\n", iblock);

  get_block(fd, iblock, buf);

  ip = (INODE *) buf + 1;

  printf(" ============== root inode info =============\n");
  printf("mode=%4x ", ip->i_mode);
  printf("uid=%d  gid=%d\n", ip->i_uid, ip->i_gid);
  printf("size=%d\n", ip->i_size);
  printf("time=%s",   ctime(&ip->i_ctime));
  printf("link=%d\n", ip->i_links_count);
  printf("i_block[0]=%d\n", ip->i_block[0]);
  printf("=============================================\n");
  
  rootblock = ip->i_block[0];
  
  get_block(fd, rootblock, dbuf);
  
  dp = (DIR *)dbuf;
  cp = dbuf;

  printf("Inode    rec_len    name_len    name\n");

  while(cp < dbuf + BLKSIZE)  //while (cp < &dbuf[BLKSIZE]){
  {
    strncpy(tbuf, dp->name, dp->name_len);
    tbuf[dp->name_len] = 0;
    
    printf("%4d %8d %10d %11s\n", dp->inode, dp->rec_len, dp->name_len, tbuf);
      
    
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }

}

u32 search(INODE *ino, char *name)
{
  int i;
  int j;
  DIR *dp;
  char* cp;
  for(j = 0; ino->i_block[j] < 12; j++)
  {
   if(ino->i_block[j])
    get_block(fd,ino->i_block[j],buf);
  
    dp = (DIR*)buf;
    cp = (char*)buf;

    while(cp < buf + BLKSIZE)
    {
      i = strcmp(dp->name,name);
      if(i == 0)
      {
       printf("Match Found!");
       return dp->inode;
      }
      else
      {
      cp += dp->rec_len;
      dp = (DIR *)cp;
      }
    }
  if(ino->i_block[12])
    {
     indirectPrint(ino);
    }
  }
}
INODE*iget(int dev, int ino)
{
  char buf[BLKSIZE];
  INODE *ip;

  printf("get inode of ino=%d\n", ino);
  
  int block = (ino-1)/ 8 + iblock;
  int offset = (ino-1) % 8;

  get_block(dev, block, buf);
  
  return (INODE*) buf + offset;
  
  
}

void indirectPrint(INODE *ip)
{
  char ibuf[BLKSIZE];
  u32 *up;
  
  if(ip->i_block[12])
  {
    get_block(fd, ip->i_block[12], ibuf);
    up = (u32 *)ibuf;
    while(*up)
    {
    printf("%d ",*up);
    up++;
    }
  }
}

main(int argc, char *argv[ ])
{ 
  int i = 0;
  int j = 0;
  int k = 0;
  int x = 0;  
  char *name[256]; 
  char *token;
  INODE *ino;
  if (argc > 1)
    dev = argv[1];
  fd = open(dev, O_RDONLY);
  if (fd < 0)
  {
    printf("open failed\n");
    exit(1);
  }

  token = strtok(argv[2],"/");
  while(token)
  {
    name[i] = token;
    printf("token: %s\n",token);
    token = strtok(NULL, "/"); 
    printf("name[%d]: %s\n",i,name[i]);
    i++;
  }
  
 super();
 gd();
 // imap();
 // bmap();
 // inode();
 // dir();
 // inodeBegin();

 j = traverse(dev);
 get_block(fd,iblock,buf);
 ip = (INODE *)buf + 1;
 
 printf("First Block = %d\n", iblock);
 for(k = 0; k < i; k++)
 {
  if(ip->i_mode & 0x0100)
  {
   printf("i = [%d], Searching for %s\n", i, name[i]);
   x = search(ip, name[i]);
   if(x == -1)
   {
    printf("%s not found\n",name[i]);
    return;
   }
   else
   {
    printf("inumber = %d\n", x-1);

    ino = iget(fd,x);
    if(!ino)
    {
      printf("Error.");
      exit(1);
    }
   ip = ino;
   }
  }
 }
}
