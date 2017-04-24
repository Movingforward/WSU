/*****************************************************
* image file booter	t.c			     *
*	CS 460				   	     *
*	Zac Reeves				     *
*						     *
*****************************************************/
#include "ext2.h"
#define BLK 1024
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;
char buf1[BLK], buf2[BLK];

prints(char *s)
{
 while(*s){
  putc(*s++);
	} //call putc(c) to print the string s;
}

gets(char s[ ])
{
 int i = 0;
  while((s[i] = getc()) != '\r') //call getc() to input a string into s[ ]
  {
   putc(s[i]);
   i++;
  }
 s[i] = 0;
}

u16 getblk(u16 blk, char *buf)
{
	readfd( blk/18, (blk / 9) % 2, (blk*2)%18, buf);
}

u16 search(INODE *ip, char *name)
{
	int i; DIR *dp; char c;
	for(i=0; i<12; i++)//DIR has at most 12 direct blocks
	 {
	  if( (u16)ip->i_block[i]){
	    getblk((u16)ip->i_block[i], buf2);
	    dp = (DIR *)buf2;
 	    while((char *)dp < &buf2[BLK]){
	      c = dp->name[dp->name_len]; //Save last byte
	      dp->name[dp->name_len] = 0; //Make name into a string
	      prints(dp->name); putc(' '); //Show dp->name string
	      if (strcmp(dp->name, name) == 0){
			prints("\n\r");
			return ((u16)dp->inode);
		}
		dp->name[dp->name_len] = c; //Restore last byte
		dp = (char *)dp + dp->rec_len;
	    }
	  }
	  error(); //error if can't find file name
	 }
}

main()
{
 char *cp, *name[2], filename[64];
 u16 i, ino, blk, iblk;
 u32 *up;
 GD *gp;
 INODE *ip;
 DIR *dp;
 name[0] = "boot"; name[1] = filename;
 prints("bootname: ");
 gets(filename);
 prints("\n\r");
 if(filename[0]==0) name[1]="mtx";
 getblk(2, buf1); //read blk#2 to get group descriptor 0
 gp = (GD *)buf1;
 iblk = (u16)gp->bg_inode_table; //inodes begin block
 getblk(iblk, buf1); //read first inode block
 ip = (INODE *)buf1 + 1; //ip->root inode #2
 for(i=0; i<2; i++){
    ino = search(ip, name[i]) - 1;
    if(ino < 0) error(); //if search() return 0
    getblk(iblk+(ino/8), buf1); //read inode block of ino
    ip = (INODE *)buf1 + (ino % 8);
 }
 if((u16)ip->i_block[12]) //read indirect block into buf2, if any
   getblk((u16)ip->i_block[12], buf2);
 setes(0x1000); //set ES to loading segment
 for(i=0; i<12; i++){ //load direct blocks
     getblk((u16)ip->i_block[i], 0);
     inces();
 }
 if((u16)ip->i_block[12]){ //load indirect blocks, if any
    up = (u32 *)buf2;
    while(*up++){
      getblk((u16)up, 0);
      inces();
   }
 }
} 
