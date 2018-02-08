#include <stdio.h>
#include <stdlib.h>
typedef unsigned int u32;

int BASE = 10;
int OCT = 8;
int HEX = 16;

char *table = "0123456789ABCDEF";

/*Prototypes*/
int rpu(u32 x);
int printu(u32 x);
void prints(char *s);
void printc(char c);
void printi(int n);
int rpo(u32 x);
int printOct(u32 x);
int rph(u32 x);
int printHex(u32 x);
int myprintf(char *fmt, ...);

main() //Testing
{
    myprintf("this is a string\n"); //PRINT: this is a string

    myprintf("this is %c hi %d test %x\n", 'A', 1234, 100); //PRINT: this is A hi 1234 test 0x64

    myprintf("---------- testing YOUR myprintf() ---------\n");

       myprintf("this is a test\n");

          myprintf("testing a=%d b=%x c=%c s=%s\n", 123, 123, 'a', "testing");

             myprintf("string=%s, a=%d  b=%u  c=%o  d=%x\n", "testing string", -1024, 1024, 1024, 1024);

                myprintf("mymain() return to main() in assembly\n");
}

int mymain(int argc, char *argv[], char *env[])
{
    int i = 0;
    printf("%d\n", argc);
    myprintf("int argc = %d\n", argc);
    myprintf("argv[]:\n");
    for(i = 0; i < argc; i++)
    {
        myprintf("argv[%d] = %s\n", i, argv[i]);
    }
    putchar('\n');
}

int rpu(u32 x)
{
  char c;
  if (x){
     c = table[x % BASE];
     rpu(x / BASE);
     putchar(c);
  }
} 


int printu(u32 x)
{
  if (x==0)
     putchar('0');
  else
     rpu(x);
  putchar(' ');
}

void prints(char *s)
{
  while(*s)
  {
    putchar(*s);
    s++;
  }
  putchar('\n');
}

void printc(char c)
{
    putchar(c);
}

void printi(int n)
{
    if( n < 0)
    {
        putchar('-');
        n = -n;
    }
    printu((u32) n);
}

int rpo(u32 x)
{
    char c;
    if(x)
    {
        c = table[x % OCT];
        rpo(x / OCT);
        putchar(c);
    }

}

int printOct(u32 x)
{
    if(x == 0)
        putchar('0');
    else
        putchar('0');
        rpo(x);
        putchar(' ');
}

int rph(u32 x)
{
    char c;
    if(x)
    {
        c = table[x % HEX];
        rph(x / HEX);
        putchar(c);
    }
}

int printHex(u32 x)
{
    if(x == 0)
        putchar('0');
    else
        putchar('0');
        putchar('x');
        rph(x);
        putchar(' ');
}
int myprintf(char *fmt, ...)
{
    char *cp = fmt;
    int *ip = &fmt + 1;
    while(*cp)
    {
    if(*cp == '%')
    {
     cp++;
     if(*cp == 'c')
     {
        printc(*ip);
        ip++;
     }
     if(*cp == 's')
     {
        prints(*ip);
        ip++;
     }
     if(*cp == 'u')
     {  
        printu(*ip);
        ip++;
     }
             
     if(*cp == 'd')
     {
        printi(*ip);
        ip++;
     }
     if(*cp == 'o')
     {
        printOct(*ip);
        ip++;
     }
     if(*cp == 'x')
     {
        printHex(*ip);
        ip++;
     }
     if(*cp == '\n')
     {
        printc('\n');
        printc('\r');
     }
    cp++;
    }
    else
    {
        putchar(*cp);
        cp++;
    }
    }
}
