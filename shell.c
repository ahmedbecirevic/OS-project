#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TRUE 1

char *line_read (void) {
   char *line = NULL;
   ssize_t sizeOfBuff = 0;

   if (getline(&line, &sizeOfBuff, stdin) == -1) {
      // check if end of file -> EOF
      if (feof(stdin)) {
         exit(EXIT_SUCCESS);
      } else {
         perror("lineReading");
         exit(EXIT_FAILURE);
      }
   }
   return line;
}

char **line_split (char *line) {
   int pos = 0;
   char **tokens = malloc(64 * sizeof(char*));
   char *token;

   if (!tokens) {
      fprintf(stderr, "error when allocating\n");
      exit(EXIT_FAILURE);
   }

   token = strtok(line, LSH_TOK_DELIM);
   while (token != NULL) {
      tokens[pos] = token;
      pos++;

      token = strtok(NULL, LSH_TOK_DELIM);
   }
   tokens[pos] = NULL;
   return tokens;
}

void commands_input_loop (void) {
   char **arguments;
   char *line;
   int state;

// use do while loop beacuse it executes once before it checks its value 
   do (state) {
      printf("prompt$ ");
      // read user input command
      line = line_read();
      arguments = line_split(line);
      state = line_execute(arguments);

      free(line);
      free(arguments);
   } while (state);
}


int main (int argc, char **argv) {
   int pid, i = 0;
   char tab[256], *s;

   commands_input_loop();

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

