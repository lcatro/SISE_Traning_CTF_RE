
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <windows.h>

//#define FLAG "hrctf{re_250_xor}"

void is_debug(void) {
    unsigned char* peb=0;
    __asm {
        push eax;
        mov eax,fs:0x30;
        mov peb,eax;
        pop eax;
    }
    unsigned char debug_flag=*(unsigned long*)(peb+0x02);
    debug_flag&=0x1;
    if (debug_flag)
        exit(0);
}

#pragma data_seg(".tls")
#pragma comment(linker, "/INCLUDE:__tls_used")

void NTAPI check_debug(PVOID DllHandle, DWORD Reason, PVOID Reserved) {
    is_debug();
}

#pragma data_seg()

#pragma data_seg(".CRT$XLA")
PIMAGE_TLS_CALLBACK TLS_CALLBACK[] = {check_debug, 0};
#pragma data_seg()

unsigned char buffer[32]={0};

void check(void) {
    unsigned char flag[]={0x38,0x23,0x31,0x27,0x32,0x2E,0x24,0x32,0x07,0x6B,0x6F,0x6B,0x03,0x25,0x31,0x2D,0x1D};
    bool is_ok=true;
    for (int index=0;index<17;++index)
        if (flag[index]!=(buffer[index]^0x50+index))
            is_ok=false;
    if (is_ok)
        printf("OK!");
    else
        printf("ERROR!");
    exit(0);
}

void main(void) {
    try {
        printf("please input flag:\n>");
        scanf("%s",buffer);
        __asm __emit 0x00;
    } catch (...) {  //  用OD 修改异常入口点到check..
        printf("ERROR!");
    }
}
