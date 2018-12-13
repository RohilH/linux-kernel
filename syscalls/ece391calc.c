#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 1024
int compute(int lhs, int rhs, char oper) {
  if (oper == '+') {
    return lhs + rhs;
  }
  if (oper == '-') {
    return lhs - rhs;
  }
  if (oper == '*') {
    return lhs * rhs;
  }
  if (oper == '/') {
    return lhs / rhs;
  }
  return 0;
}

int calculate(char* s) {
    int n = 0;
    char opers[2];  // operators
    int args[2];    // arguments
    int p = 0;      // points to the current opers
    /* initialization */
    opers[0] = '+';
    args[0] = 0;
    char c;
    int i = 0;
    /* scan this string */
    while(c = s[i++], c != 0) {
        if (c == ' ') {
            continue;
        }
        /* we meet operator */
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            /* if previous operator has first privilege */
            if (opers[p] == '*' || opers[p] == '/') {
                n = compute(args[p], n, opers[p]);
                --p;
            }
            /* reduce above */
            if (c == '+' || c == '-') {
                args[p] = compute(args[p], n, opers[p]);
                opers[p] = c;
            }
            else {
                args[p + 1] = n;
                opers[p + 1] = c;
                ++p;
            }
            n = 0;
        }
        else {
            n = n * 10 + c - '0';
        }
    }
    if (opers[p] == '*' || opers[p] == '/') {
        n = compute(args[p], n, opers[p]);
    }
    return compute(args[0], n, opers[0]);
}
// int calculate(char* s) {
//     int i,len,top,j;
//     long stack[100],temp1,temp2,res;
//     const long op1 = (long )2147483 + 1,op2 = op1 + 1,op3 = op2 + 1, op4 = op3 + 1, op5 = op4 + 1;
//     res = top = 0;
//     len = ece391_strlen(s);
//     for(i = 0; i < len; i++) {
//         if(s[i] == '(') {
//             stack[top++] = op3;
//         }
//         else if(s[i] == '*') {
//             stack[top++] = op4;
//         }
//         else if(s[i] == '/') {
//             stack[top++] = op5;
//         }
//         else if(s[i] == '+') {
//             stack[top++] = op2;
//         }
//         else if(s[i] == '-') {
//             stack[top++] = op1;
//         }
//         else if(s[i] >= '0' && s[i] <= '9') {
//             temp1 = 0;
//             for(j = i; s[j] >= '0' && s[j] <= '9'; j++) {
//                 temp1 = temp1 * 10 + s[j] - '0';
//             }
//             stack[top++] = temp1;
//             i = j - 1;
//         }
//         else if(s[i] == ')') {
//             temp1 = 0;
//             temp2 = 0;
//             top--;
//             while(stack[top] != op3) {
//                 if(stack[top] < op1) {
//                     temp1 = stack[top];
//                 }
//                 else {
//                     if(stack[top] == op1) temp2 -= temp1;
//                     else if(stack[top] == op2) temp2 += temp1;
//                     else if(stack[top] == op4) temp2 *= temp1;
//                     else if(stack[top] == op5) temp2 = temp2/ temp1;
//                 }
//                 top--;
//             }
//             stack[top++] = temp2 + temp1;
//         }
//     }
//     int flag = 1;
//     for(i = 0; i < top; i++) {
//         if(stack[i] == op2) flag = 1;
//         else if(stack[i] == op1) flag = -1;
//         else res += flag * stack[i];
//     }
//     return res;
// }

int main ()
{
    int32_t cnt;
    uint8_t buf[BUFSIZE];

    ece391_fdputs (1, (uint8_t*)"Opening Calculator... \n");
    while (1) {
        ece391_fdputs (1, (uint8_t*)"Input expression>> ");

        if (-1 == (cnt = ece391_read (0, buf, BUFSIZE-1))) {
            ece391_fdputs (1, (uint8_t*)"Can't read name from keyboard.\n");
            return 3;
        }
        int i = 0, invalidFlag = 0;
        for (i = 0; i < cnt; i++) {
            if (buf[i] < '(' || buf[i] > '9' || buf[i] == '.' || buf[i] == ',') {
                if (buf[i] == '\0' || buf[i] == ' ') continue;
                ece391_fdputs (1, (uint8_t*)"Invalid Expression; Please try again. \n");
                invalidFlag = 1;
                break;
            }
        }
        if (invalidFlag == 1) continue;
        int ret = calculate(buf);
        int negFlag = 0;
        if (ret < 0) {
            ret = -ret;
            negFlag = 1;
        }
        ece391_itoa(ret, buf, 10);
        ece391_fdputs(1, (uint8_t*)"Answer: ");
        if (negFlag == 1) {
            ece391_fdputs(1, "-");
        }
        ece391_fdputs(1, buf);
        ece391_fdputs(1, (uint8_t*) "\n");
        continue;
    }
    return -1;
}
