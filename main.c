#include "bf.c"
#include <stdio.h>
#include <string.h>

int main() {
     unsigned char input[100] = {0};
     if (!init_vm(100))
          return 1;
     if (!init_lpstack(100))
          return 1;

     char prompt[] = "> ";
     
     while (strcmp((char *)input, "q\n")) {
          fputs(prompt, stdout);
          fgets((char *)input, 100, stdin);
          if (eval(input) == 0) {
               strcpy(prompt, "> ");
          }
          else {
               strcpy(prompt, "X ");
          }
     }

     return 0;
}
