#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<unistd.h>

#define TRUE 1

int main () {
   int pid, i = 0;
   char tab[256], *s;

   while(TRUE) {
      printf("prompt %d", i);

      fflush(stdout);

      s = gets(tab);

      if( s == NULL ) {
         fprintf(stderr, "Bye bye\n");
         exit(0);
      }

      pid = fork();

      printf("I'm running");

      switch (pid)
      {
      case 0:
         printf("In the child\n");
         execlp(tab, tab, NULL);
         perror("exec");
         exit(0);
         break;
      case -1:
         perror("fork");
         break;
      default:
         printf("in the parent .. wait\n");
         wait(0);
         i++;
      }
   }
   return 0;
}

