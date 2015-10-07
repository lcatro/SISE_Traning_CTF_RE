
#pragma warning (disable:4244)

#include <memory.h>
#include <stdio.h>

#include <windows.h>

char output_string[10]={0};

//#define FLAG "hrctf:{you_can_make_all}"

unsigned int network_encode(char* encode_string,unsigned int encode_string_length_) {
    unsigned int encode_buffer_length=!(encode_string_length_%2)?encode_string_length_:encode_string_length_+1;
    char* encode_buffer=(char*)malloc(encode_buffer_length);
    char* encode_point=encode_buffer;
    memset(encode_buffer,0,encode_buffer_length);
    memcpy(encode_buffer,encode_string,encode_string_length_);

    for (unsigned int index=0;index<encode_buffer_length;index+=2,encode_point+=2) {
        unsigned char key_num1=(*encode_point&0xF),key_num2=(*encode_point&0xF0)>>4,key_num3=(*(encode_point+1)&0xF),key_num4=(*(encode_point+1)&0xF0)>>4;
        *encode_point=key_num2+(key_num3<<4);
        *(encode_point+1)=key_num4+(key_num1<<4);
        *encode_point^=encode_buffer_length;
        *(encode_point+1)^=encode_buffer_length;
    }
    memcpy(encode_string,encode_buffer,encode_buffer_length);
    free(encode_buffer);
    return encode_buffer_length;
}

void network_decode(char* decode_string,unsigned int decode_string_length_) {
    unsigned int decode_buffer_length=!(decode_string_length_%2)?decode_string_length_:decode_string_length_+1;
    char* decode_buffer=(char*)malloc(decode_buffer_length);
    char* decode_point=decode_buffer;
    memset(decode_point,0,decode_buffer_length);
    memcpy(decode_buffer,decode_string,decode_string_length_);

    for (unsigned int index=0;index<decode_buffer_length;index+=2,decode_point+=2) {
        *decode_point^=decode_buffer_length;
        *(decode_point+1)^=decode_buffer_length;
        unsigned char key_num1=(*(decode_point+1)&0xF0)>>4,key_num2=(*decode_point&0xF),key_num3=(*(decode_point)&0xF0)>>4,key_num4=(*(decode_point+1)&0xF);
        *decode_point=key_num1+(key_num2<<4);
        *(decode_point+1)=key_num3+(key_num4<<4);
    }
    memcpy(decode_string,decode_buffer,decode_buffer_length);
    free(decode_buffer);
}

bool is_debug_peb(void) {
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
        return true;
    return false;
}

bool is_debug_api(void) {
    return (IsDebuggerPresent()==TRUE)?true:false;
}

void anti_debug_thread(void) {
    while (true) {
        if (is_debug_peb()) {
            __asm {
                mov eax,0;
                mov ebx,[eax];
            }
        }
        if (is_debug_api()) {
            __asm {
                mov eax,0;
                mov ebx,[eax];
            }
        }
    }
}

void get_input_string(unsigned char* input_string) {
    unsigned long function_addreess=*(unsigned long*)((unsigned long)get_input_string+1)+(unsigned long)get_input_string+5;
    __asm {
        mov eax,function_addreess;
        add eax,0x37;
        jmp $+26;
        __emit 'P';
        __emit 'l';
        __emit 'a';
        __emit 'e';
        __emit 's';
        __emit 'e';
        __emit ' ';
        __emit 'i';
        __emit 'n';
        __emit 'p';
        __emit 'u';
        __emit 't';
        __emit ' ';
        __emit 'f';
        __emit 'l';
        __emit 'a';
        __emit 'g';
        __emit ':';
        __emit '\n';
        __emit '>';
        __emit '\0';
        push eax;
        call printf;
        add esp,4;
    }

    scanf("%s",input_string);
    CloseHandle(GetCurrentThread());
}

void check_flag_string(unsigned char* input_string) {
    __asm {
        sub esp,0x1000;
    }
    unsigned long function_address=*(unsigned long*)((unsigned long)check_flag_string+1)+(unsigned long)check_flag_string+5;
    unsigned long switch_code=1;
    __asm {
        mov eax,function_address;
        sub eax,-0x41;
        push eax;
        ret;
    }
    if (IsDebuggerPresent()) {
        __asm {
            mov eax,0;
            mov ebx,[eax];
        }
    }
    __asm {
        test eax,eax;
        jz  $+0xD;
        jnz $+7;
        __emit 0xE9;
    }
    __asm {
        jz  $+0x11;
        jnz $+0xB;
        __emit 0xE9;
        __emit 0x00;
        __emit 0x10;
        __emit 0x40;
        __emit 0x00;
    }
SWITCH:
    switch (switch_code) {
        case 1:
            if (is_debug_api()) {
                __asm {
                    mov eax,0;
                    mov ebx,[eax];
                }
            }
            switch_code=5;
            break;
        case 2:
            *output_string='E';
            *(output_string+1)='R';
            *(output_string+2)='R';
            *(output_string+3)='O';
            *(output_string+4)='R';
            *(output_string+5)='\0';
            switch_code=3;
            break;
        case 3:
            printf(output_string);
            ExitProcess(0);
            break;
        case 4:
            *output_string='O';
            *(output_string+1)='K';
            *(output_string+2)='\n';
            *(output_string+3)='\0';
            switch_code=3;
            break;
        case 5:
            char flag_1_valid[8]={'h','r','c','t','f',':','{','\0'};
            char flag_1_input[8]={0};
            memcpy(flag_1_input,input_string,7);
            flag_1_input[7]='\0';
            if (!strcmp(flag_1_valid,flag_1_input) || input_string[24]=='}') {
                char encode[16]={0};
                char check[16]={0};
                memcpy(encode,&input_string[7],16);
                network_encode(encode,16);
                check[14]=0xD6;
                check[15]=0xD6;
                check[5]=0x26;
                check[8]=0x06;
                check[9]=0xC6;
                check[6]=0xE6;
                check[12]=0x05;
                check[13]=0xE6;
                check[11]=0xA6;
                check[1]=0x86;
                check[0]=0xE7;
                check[2]=0xE7;
                check[4]=0x06;
                check[3]=0x45;
                check[7]=0xF5;
                check[10]=0x46;
                if (!memcmp(check,encode,16)) {
                    switch_code=4;
                    break;
                }
            }
            switch_code=2;
    }
    __asm {
        jmp $+6;
        __emit 0xE8;
        jmp SWITCH;
    }
    CloseHandle(GetCurrentThread());
    __asm {
        sub esp,-0x1000;
    }
}

void init_anti_debug(void) {
    __asm {
        jmp $+6;
        __emit 0xE8;
    }
    CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&anti_debug_thread,NULL,NULL,NULL);
    __asm {
        jz  $+0xD;
        jnz $+7;
        __emit 0xE9;
    }

    unsigned char* input_string=(unsigned char*)malloc(0x100);

    __asm {
        jz  $+0x11;
        jnz $+0xB;
        __emit 0xE9;
        __emit 0x00;
        __emit 0x10;
        __emit 0x40;
        __emit 0x00;
    }
    __asm {
        push eax;
        mov eax,network_decode;
        mov eax,[eax+1];
        add eax,network_decode;
        jmp $+7;
        call eax;
        pop eax;
    }
    HANDLE get_input_thread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&get_input_string,input_string,NULL,NULL);
    if (INVALID_HANDLE_VALUE==get_input_thread)
        ExitProcess(0);
    WaitForSingleObject(get_input_thread,INFINITE);
    
    HANDLE check_thread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&check_flag_string,input_string,NULL,NULL);
    if (INVALID_HANDLE_VALUE==check_thread)
        ExitProcess(0);
    WaitForSingleObject(check_thread,INFINITE);
}

void main(void) {
    init_anti_debug();
    ExitProcess(0);
}

