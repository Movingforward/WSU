// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define MAX 256

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

int server_sock, r;
int SERVER_IP, SERVER_PORT; 


// clinet initialization code

int client_init(char *argv[])
{
  printf("======= client init ==========\n");

  printf("1 : get server info\n");
  hp = gethostbyname(argv[1]);
  if (hp==0){
     printf("unknown host %s\n", argv[1]);
     exit(1);
  }

  SERVER_IP   = *(long *)hp->h_addr;
  SERVER_PORT = atoi(argv[2]);

  printf("2 : create a TCP socket\n");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0){
     printf("socket call failed\n");
     exit(2);
  }

  printf("3 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = SERVER_IP;
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("4 : connecting to server ....\n");
  r = connect(server_sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(1);
  }

  printf("5 : connected OK to \007\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s  IP=%s  PORT=%d\n", 
          hp->h_name, inet_ntoa(SERVER_IP), SERVER_PORT);
  printf("---------------------------------------------------------\n");

  printf("========= init done ==========\n");
}

main(int argc, char *argv[ ])
{
  int n;
  char line[MAX], ans[MAX];
  char buf[1024];
  int fdO;
  char cwd[128];
  char *name[MAX];

  if (argc < 3){
     printf("Usage : client ServerName SeverPort\n");
     exit(1);
  }

  client_init(argv);
  // sock <---> server
  printf("********  processing loop  *********\n");
  printf("********************** menu *********************\n");
  printf("* get  put  ls   cd   pwd   mkdir   rmdir   rm  *\n");
  printf("* lcat     lls  lcd  lpwd  lmkdir  lrmdir  lrm  *\n");
  printf("*************************************************\n");
  while (1){
    printf("input a line : ");
    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);
    
    
    // Send ENTIRE line to server
    n = write(server_sock, line, MAX);
    printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

 /*   if (strcmp(line, "get test.c") == 0)
    {
     fdO = creat(buf, 0644);
     open(fdO, O_WRONLY);
    // while((n = read(server_sock, buf, 1024)))
    // {
      write(fdO, buf, 1024);
    // }
     printf("Copied file to Client\n");    
    } 
    	   else if(strcmp(name[0], "ls") == 0)
	   {
       // strcat(cwd, "/");
	   // strcat(cwd, name[1]);
        
        DIR *dp = NULL;

        struct stat stat;
        struct dirent *dirp = NULL;

        dp = opendir(cwd);

        if (dp != NULL)
        {
          while ((dirp = readdir(dp)) != NULL)
          {
            lstat(dirp->d_name, &stat);
            printf("%s\n", dirp->d_name);
          }
        }
       }		
	   else if(strcmp(name[0], "lcd") == 0)
	   {
		if(chdir(name[1]) == 0)
		{
		 printf("Directory Changed\n");
		}		
	   }		
	   else if(strcmp(name[0], "lpwd") == 0)
	   {
		system("pwd");
	   }		
	   else if(strcmp(name[0], "lmkdir") == 0)
	   {
		strcat(cwd, "/");
		strcat(cwd, name[1]);
		if(mkdir(cwd,0777) == 0)
		{
		 printf("Directory Created\n");
		}
		else
		{
		 printf("Directory Creation Failed\n");
		}
       }
	   else if(strcmp(name[0], "lrmdir") == 0)
	   {
		strcat(cwd, "/");
		strcat(cwd, name[1]);
		if(rmdir(cwd) == 0)
		{
		 printf("Directory Removed\n");
		}
		else
		{
		 printf("Directory Removal Failed!!!!!!\n");
		}
       } 
	   else if(strcmp(name[0], "lquit") == 0)
	   {
		exit(1);
	   }
*/
    // Read a line from sock and show it
   // n = read(server_sock, ans, MAX);
   // printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);
  }
}


