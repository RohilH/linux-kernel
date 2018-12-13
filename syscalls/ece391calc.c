#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 1024

void sumSubtract(long * argsStack, long * operStack, int * top1, int * top2) {
    long val = 0;
    while(*top2 != -1 && operStack[*top2] != '(') {
        if (operStack[*top2] == '+') {
            val += argsStack[*top1];
        }
        else {
            val -= argsStack[*top1];
        }
        (*top1)--;
        (*top2)--;
    }
    argsStack[(*top1)] += val;
    if ((*top2) != -1)
        (*top2)--;
}
void multDiv(long * argsStack, long * operStack, int * top1, int * top2) {
    long val = 0;
    if ((*top2) == -1)
        return;
    if (operStack[*top2] == '*') {
        val = argsStack[(*top1)];
        (*top1)--;
        argsStack[(*top1)] *= val;
        (*top2)--;
        return;
    }
    if (operStack[*top2] == '/') {
        val = argsStack[(*top1)];
        (*top1)--;
        argsStack[(*top1)] /= val;
        (*top2)--;
        return;
    }
}
int calculate(char* s) {
    int i, len, top1, top2, j;
    long argsStack[100], operStack[100], temp1;
    top1 = -1;
    top2 = -1;

    len = ece391_strlen(s);
    for(i = 0; i < len; i++) {
        if (s[i] == ' ') {
            continue;
        }
        if(s[i] >= '0' && s[i] <= '9') {
            temp1 = 0;
            for(j = i; s[j] >= '0' && s[j] <= '9'; j++) {
                temp1 = temp1 * 10 + s[j] - '0';
            }
            top1++;
            argsStack[top1] = temp1;
            i = j - 1;
            multDiv(argsStack, operStack, &top1, &top2);
        }
        else if(s[i] == ')') {
            sumSubtract(argsStack, operStack, &top1, &top2);
            multDiv(argsStack, operStack, &top1, &top2);
        }
        else {
            top2++;
            operStack[top2] = s[i];
        }
    }
    sumSubtract(argsStack, operStack, &top1, &top2);
    if (top1 == -1)
        return 0;

    return argsStack[top1];
}

int main ()
{
    int32_t cnt;
    uint8_t buf[BUFSIZE];

    ece391_fdputs (1, (uint8_t*)"Opening DagOS Calculator... \n");
    while (1) {
        ece391_fdputs (1, (uint8_t*)"Input expression>> ");

        if (-1 == (cnt = ece391_read (0, buf, BUFSIZE-1))) {
            ece391_fdputs (1, (uint8_t*)"Can't read name from keyboard.\n");
            return 3;
        }
        if (buf[0] == 'q') {
            ece391_fdputs (1, (uint8_t*)"Thank you for using DagOS calculator.\n");
            return 0;
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
