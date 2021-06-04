#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1

// declare functions for each command
int cmd_cd(char **args);
int cmd_help(char **args);
int cmd_exit(char **args);

// supported commands
char *commands_list[] = {"cd", "help", "exit"};

// array of function pointers 
int ( *builtin_functions[] ) ( char ** ) = { &cmd_cd, &cmd_help, &cmd_exit };

int function_num() {
   return sizeof(builtin_functions) / sizeof(char *);
}

// cd function
int cmd_cd(char **args) {
   // check if second argument exists
  if (args[1] == NULL) {
    fprintf(stderr, "You have passed only one argument \"cd\"\n");
  } else {
   //  call chdir() - change the current working directory
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}

// help function
int cmd_help(char **args) {
  int i;
  printf("Ahmed, Basil, Adi, Filip, Asim's dummy shell\n");
  printf("The following commands are built in (cd requires dir name to be passed as second argument!):\n");

  for (i = 0; i < function_num(); i++) {
    printf("  %s\n", commands_list[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}


// exit function
int cmd_exit(char **args) {
  return 0;
}




// launch the shell
int shell_start (char **args) {
   int status;
   pid_t wpid;

   pid_t pid = fork();

   // We are in the child process
   if (pid == 0) {
      // use execvp so we dont have to provide the full path of the program we want to run only the name
      // if this returns -1 or anything at all there's an error and we must exit it
      if (execvp(args[0], args) == -1) {
         perror("shell");
      }
      exit(EXIT_FAILURE);
   } else if (pid < 0) {
      // Fork had an error
      perror("shell");
   } else {
      //We are in the parent process / child executed successfully
      do {
         // use the arguments from waitpid() to ensure the parent process will wait until child exits and that it doesn't go if the child changes state
         wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
   }
   // signal to ask for input from user again
   return 1;
}


char *line_read (void) {
   char *line = NULL;
   ssize_t sizeOfBuff = 0;

   if (getline(&line, &sizeOfBuff, stdin) == -1) {
      // check if end of file -> EOF
      if (feof(stdin)) {
         exit(EXIT_SUCCESS);
      } else {
         perror("line reading");
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

   // Split input string by empty space, tabs, new line and other
   token = strtok(line, " \t\r\n\a");
   // iterate over the input while there are commands available
   while (token != NULL) {
      tokens[pos] = token;
      pos++;

      token = strtok(NULL, " \t\r\n\a");
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
   
   return 0;
}

