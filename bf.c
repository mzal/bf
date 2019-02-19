#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct {
     const unsigned char* ip;
     struct {
          unsigned char* base;
          unsigned char* p;
          size_t size;
     } mem;
} vm;

_Bool init_vm(size_t mem_size) {
     vm.ip = (unsigned char*)NULL;
     vm.mem.base = calloc(mem_size, sizeof(unsigned char));
     vm.mem.p = vm.mem.base + (mem_size / 2);
     vm.mem.size = mem_size;
     return (_Bool)vm.mem.base;
}

struct {
     const unsigned char** base;
     const unsigned char** top;
     size_t size;
} lpstack;

_Bool init_lpstack(size_t stack_size) {
     lpstack.base = malloc(sizeof(unsigned char*) * stack_size);
     lpstack.top = lpstack.base;
     lpstack.size = stack_size;
     return (_Bool)lpstack.base;
}

_Bool resize_lpstack() {
     const unsigned char ** new_base = malloc(sizeof(unsigned char*) * lpstack.size * 2);
     if (new_base == NULL) {
          return (_Bool) NULL;
     }
     const unsigned char ** new_top = new_base;
     const unsigned char ** ptr = lpstack.base;
     while (ptr < lpstack.top) {
          *new_top++ = *ptr++;
     }
     free(lpstack.base);
     lpstack.base = new_base;
     lpstack.top = new_top;
     lpstack.size *= 2;
     return (_Bool) 1;
}

void lpstack_push(const unsigned char* ptr) {
     *lpstack.top = ptr;
     lpstack.top++;
     if (lpstack.top > lpstack.base + lpstack.size - 1) {
          resize_lpstack();
     }
}

const unsigned char* lpstack_pop() {
     if (lpstack.top == lpstack.base) {
          return NULL;
     }
     lpstack.top--;
     return *lpstack.top;
}

enum opcode {
     FRWD = '>',  // increment IP
     BACK = '<',  // decrement IP
     INCR = '+',  // increment cell
     DECR = '-',  // decrement cell
     READ = '.',  // output data from cell
     WRIT = ',',  // write data to cell
     READ_NUM = '*',
     LPST = '[',  // if cell == 0 jump after matching ]
     LPEN = ']',  // if cell != 0 jump back to matching [
     STOP = 0,    // end execution
};

const unsigned char* find_lpen() {
     const unsigned char* ptr = vm.ip;
     unsigned int balance = 1;
     for (;;) {
          switch (*ptr++) {
          case LPST:
               balance++;
               break;

          case LPEN:
               if (--balance == 0) {
                    return ptr;
               }
               break;

          case STOP:
               return NULL;
          }
     }
     return NULL;
}

enum status {
     SUCCESS = 0,
     INV_OPCODE = 1,
     INV_LOOP = 2
};

int eval(const unsigned char *expr){
     vm.ip = expr;
     for (;;) {
          unsigned char opcode = *vm.ip++;
          switch (opcode) {
          case FRWD:
               if (vm.mem.p == vm.mem.base + vm.mem.size - 1) {
                    vm.mem.p = vm.mem.base;
               }
               else {
                    vm.mem.p++;
               }
               break;

          case BACK:
               if (vm.mem.p == 0) {
                    vm.mem.p = vm.mem.base + vm.mem.size - 1;
               }
               else {
                    vm.mem.p--;
               }
               break;

          case INCR:
               (*vm.mem.p)++;
               break;

          case DECR:
               (*vm.mem.p)--;
               break;

          case READ:
               fprintf(stdout, "%c", *vm.mem.p);
               break;

          case READ_NUM:
               fprintf(stdout, "%hu\n", (unsigned int)(*vm.mem.p));
               break;

          case WRIT:
               ;
               short unsigned int input;
               fscanf(stdin, "%hu", &input);
               (*vm.mem.p) = (unsigned char) input;
               getc(stdin);
               break;

          case LPST:
               if (*vm.mem.p == 0) {
                    const unsigned char* match_lpen = find_lpen();
                    if (match_lpen == (unsigned char *) NULL) {
                         return INV_LOOP;
                    }
                    vm.ip = match_lpen;
               }
               else {
                     lpstack_push(vm.ip - 1);
               }
               break;

          case LPEN:
               if (*vm.mem.p != 0) {
                    vm.ip = lpstack_pop();
               }
               else {
                    lpstack_pop();
               }
               break;

          case STOP:
               return SUCCESS;

          default:
               ;
               /* return INV_OPCODE; */
          }
     }
     return SUCCESS;
}
