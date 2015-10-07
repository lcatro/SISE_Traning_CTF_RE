
#include <stdio.h>
#include <string.h>

//#define FLAG "hrcrf{RE100_Calcu}"

unsigned char flag[18]={0x9A,0xA4,0x95,0xA4,0x98,0xAD,0x84,0x77,0x63,0x62,0x62,0x91,0x75,0x93,0x9E,0x95,0xA7,0xAF};

void main(void) {
    printf("please input flag:\n>");
    unsigned char buffer[32]={0};
    scanf("%s",buffer);

    if (strlen((const char*)buffer)==18) {
        bool is_ok=true;
        for (int index=0;index<18;++index)
            if (buffer[index]+50!=flag[index])
                is_ok=false;
        if (is_ok) {
            printf("OK!");
            return;
        }
    }
    printf("ERROR!");
/*
    for (int index=0;index<18;++index)
        flag[index]=*(char*)(FLAG+index)+50;
    printf("%s",flag);*/
}
