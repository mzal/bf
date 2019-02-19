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

int main() {
     const size_t mem_size = MEM_SIZE;
     const size_t stack_size = STACK_SIZE;

     unsigned char input[INPUT_BUFF] = {0};
     if(!init(mem_size, stack_size)) {
          return 1;
     }

     char prompt[] = "> ";

     while (strcmp((char *)input, "q\n")) {
          fputs(prompt, stdout);
          fgets((char *)input, INPUT_BUFF, stdin);
          if (strcmp((char*)input, "r\n")) {
               reset(mem_size, stack_size);
          }
          if (eval(input) == 0) {
               strcpy(prompt, "> ");
          }
          else {
               strcpy(prompt, "X ");
          }
     }

     return 0;
}
