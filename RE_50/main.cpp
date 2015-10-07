

#include <stdio.h>
#include <string.h>

//#define FLAG_RE_50 "hrctf{crack_re50}"

void main(void) {
    char flag[19]={0,'r',0,0,'f','{','c','r','a','c','k',' ','r','e','5','0','}',0};

    printf("please input flag:\n>");

    flag[0]='h';
    flag[11]='_';
    flag[3]='t';
    flag[2]='c';

    char buffer[32]={0};
    scanf("%s",buffer);

    if (!strcmp(buffer,flag))
        printf("OK!");
    else
        printf("ERROR!");
}
