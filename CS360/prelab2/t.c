#include <stdio.h>
#include <stdlib.h>

int *FP;



main(int argc, char *argv[ ], char *env[ ])

{

  int a,b,c;

  printf("enter main\n");



  

  // PRINT ADDRESS OF a, b, c

  printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);




  a=1; b=2; c=3;

  A(a,b);

  printf("exit main\n");

}



int A(int x, int y)

{

  int d,e,f;

  printf("enter A\n");


  // PRINT ADDRESS OF d, e, f
  printf("&d=%8x &e=%8x &f=%8x\n", &d, &e, &f);

  d=4; e=5; f=6;

  B(d,e);

  printf("exit A\n");

}



int B(int x, int y)

{

  int g,h,i;

  printf("enter B\n");

  // PRINT ADDRESS OF g,h,i

  printf("&g=%8x &h=%8x &i=%8x\n", &g, &h, &i);

  g=7; h=8; i=9;

  C(g,h);

  printf("exit B\n");

}



int C(int x, int y)

{

  int u, v, w, i, *p;

  int *fp0, k;



  printf("enter C\n");

  // PRINT ADDRESS OF u,v,w,i,p;

  printf("&u=%8x &v=%8x &w=%8x &i=%8x p=%8x\n", &u, &v, &w, &i, p);

  u=10; v=11; w=12; i=13, p=14;



  p = (int *)&p;

  printf("p = %8x\n", p);



  asm("movl %ebp, FP"); // set FP=CPU's %ebp register
  printf("<stack frame link list>\n");

  p = FP;

  while (p != NULL) //List isn't empty

  {

     printf("%08x\n", p);

	p = *p;	
  }

  printf("fp = %8x\n", p);

  printf("</stack frame link list>\n");

  p = &FP;
  while(p != 0)
  {
    p = *p;
    if(p != 0)
    {
	fp0 = p;
    }	
  }
  for(i = 0; i < 128; i++)
  {
    printf("%d, fp0 = 0x%08x	Value: 0x%08x\n", i, fp0, *fp0);
    fp0 += 8;
  }
}
