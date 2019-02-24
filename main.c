#include "bf.c"
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

_Bool init(size_t mem_size, size_t stack_size) {
     if (!init_vm(mem_size))
          return FALSE;
     if (!init_lpstack(stack_size))
          return FALSE;
     return TRUE;
}

_Bool reset(size_t mem_size, size_t stack_size) {
     del_vm();
     del_lpstack();
     return init(mem_size, stack_size);
}

#define MEM_SIZE 280
#define STACK_SIZE 280
#define INPUT_BUFF 280

void usage(char **argv) {
     printf("Usage:\n%s [filename]", argv[0]);
     return;
}

void repl() {
     unsigned char input[INPUT_BUFF] = {0};

     char prompt[] = "> ";

     while (strcmp((char *)input, "q\n")) {
          fputs(prompt, stdout);
          fgets((char *)input, INPUT_BUFF, stdin);
          if (!strcmp((char*)input, "r\n")) {
               reset(MEM_SIZE, STACK_SIZE);
          }
          if (eval(input) == 0) {
               strcpy(prompt, "> ");
          }
          else {
               strcpy(prompt, "X ");
          }
     }
     return;
}

int main(int argc, char **argv) {

     if(!init(MEM_SIZE, STACK_SIZE)) {
          return 1;
     }

     if (argc > 1) {
          FILE * input_f = fopen(argv[1], "r");
          if (input_f == NULL) {
               usage(argv);
               return 1;
          }
          else {
               fseek(input_f, 0, SEEK_END);
               size_t length = ftell(input_f);
               fseek(input_f, 0, SEEK_SET);
               char * buf = malloc(length);
               if (buf) {
                    fread(buf, 1, length, input_f);
               }
               fclose(input_f);

               int rv = eval((unsigned char *) buf);

               fprintf(stdout, "\n%d", rv);
          }
     }
     else {
          repl();
     }

     return 0;
}
