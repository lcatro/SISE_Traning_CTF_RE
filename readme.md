
#华软网络安全小组逆向工程训练营


###SNST Reverse Engineering Traning


各个训练程序解析


-----

###1.RE-50 Writeup

  把代码导入到IDA ,用Hex-ray 把Main 函数汇编转到伪C 代码,结果如下

 ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180046.png)

  程序代码意思是:先给本地的数组的每个位置赋值,然后接收我们输入的字符串,再通过字符串对比函数来对比输入的字符串是否和本地校验的字符串一样,于是在strcmp 处下断点,位置如下

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180056.png)

  启动Ollydbg ,随意输入字符串

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180101.png)


  Ollydbg 会卡在0x40109D 这个地方(下断点的快捷键是F2 )
 
 ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180107.png)

  查看寄存器窗口,可以看到flag

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180111.png)
 
---

###2.RE-100 Writeup


  同样是转换到伪C 代码分析

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180116.png)

  程序原理:判断输入的字符串长度是否为19 ,符合的话把输入的字符串里面的每个字符的值加50 和byte_408030 里面的数据进行对比,于是我们把进去byte_408030 ,设置它的Array (数组)长度

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180120.png)

  然后按几下d 键把这个串转换成char 

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180125.png)

  提取这些数据出来,拿到python 里面做逆运算(因为程序是把我们输入的数据加上50 再和这些数据做比较的,所以我们把这些数据减50 就可以获取到原来的字符串了)

    code=[0x9A,0xA4,0x95,0xA4,0x98,0xAD,0x84,0x77,0x63,0x62,0x62,0x91,0x75,0x93,0x9E,0x95,0xA7,0xAF]

    for index in code :
        print chr(index-50),

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180130.png)
 
---

###3.RE-250 Writeup


  在main 函数的入口处和之前所见的有点不同,我们直接关注jmp loc_401113 

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180137.png)

  跳到jmp loc_401113 中分析代码,可以看到这里有SEH 异常处理,异常回调的地址是0x401053

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180141.png)

  往下分析,程序会执行到0x40115A ,这是一个无效的指令,于是会印发SEH 异常处理,程序跳转到0x401053

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180146.png)

  于是找到0x401053 ,分析

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180151.png)

  可以看到这里的对比运算:输入的字符串和byte_40B938 里面的数据做异或运算之后再对比两个字符是否相同,异或运算的key 是80 加上当前对比的偏移位置,于是可以写出解密python


    code=[0x38,0x23,0x31,0x27,0x32,0x2E,0x4,0x32,0x7,0x6B,0x6F,0x6B,0x3,0x25,0x31,0x2D,0x1D]
    xor_number=80

    for index in code :
        print chr(index^xor_number),
        xor_number+=1

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180156.png)

---

###4.RE-500 Writeup

  第一步首先要绕过IDA 本身的bug ,因为IDA 默认是从第一区块开始解析数据的,但是在入口点中的跳转却是在第一区块之前的,所以IDA 无法从这个位置中获取数据,于是使用OllyDbg 跟踪

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180201.png)

  跟踪到0x401041 处,继续往下单步

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180208.png)

  可以看到这又是SEH 异常处理,直接跳到0x404100

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180213.png)

  来到0x404100 ,先给它创建函数

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180217.png)

  创建函数之后的变化
  
  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180221.png)

  然后用Hex-ray转换

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180225.png)

  进去到0x401014 

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180229.png)

  切换回汇编窗口查看代码

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180233.png)

  继续跳到0x40199E 去分析,这里创建一个线程,线程入口地址是0x401023 ,于是我们过去分析这个函数的功能

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180238.png)

  发现这里一直死循环调用两个函数

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180242.png)

  sub_40102D 的代码如下(为什么会显示sub_401400 呢?因为代码是从sub_40102D 直接jmp 到sub_401400 的):从PEB 中获取程序是否被调试

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180248.png)

  Sub_401028 的功能是直接调用IsDebuggerPresent() 获取是否被调试

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180252.png)

  转换到汇编,可以发现,每次检测到程序被调试的话就会除0 ,产生异常退出程序,于是这个线程是用来反调试的

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180259.png)

  回去主线代码继续分析,遇到代码混淆

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180304.png)

  先到0x4019C8 中用c 键把代码转换成二进制数据

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180309.png)

  再到0x4019C9 中用d 键把二进指数据转换成代码,转换之后的代码如下:

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180313.png)

  继续往下分析,这里计算函数的真实地址,具体作用还不知道

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180318.png)

  继续往下,发现程序又创建新线程

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180323.png)

  于是我们查看线程里面的代码,这里是要让我们输入flag

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180327.png)

  再往下,线程自己就关闭了

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180331.png)

  主线代码里面在创建线程成功之后就阻塞等待它关闭,如果创建失败的话就关闭程序

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180336.png)

  继续往下,发现又有新线程…

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180340.png)

  在线程执行的代码最后有一处混淆

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180344.png)

  因为Push EAX + Retn 等价于Jmp Eax ,eax 的值是ecx+40 (sub -40 等于add 40),ecx 的值是这个函数的入口地址,于是可以计算出接下来程序运行到这里的将会跳转到下面这个地址

0x401610+0x40=0x401650

  跳转到此

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180349.png)

  C 键转换代码

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180353.png)

  然后又是过混淆,相信大家已经明白啦

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180359.png)

  下面遇到switch 语句,先来分析下每个cases 的意义

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180403.png)

  Case 0 是检测程序是否被调试

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180407.png)

  Case 1 往缓冲区中写ERROR

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180411.png)

  Case 2 输出刚才的缓冲区

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180416.png)

  Case 3 把OK 写入缓冲区
  
  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180423.png)

  Case 4 是flag 对比,然后我们把精力主要集中在此

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180427.png)

  这里调用了两个未知的函数,先忽略他们,继续往下分析

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180433.png)

  发现下面有一段数据

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180438.png)

  然后把这段数据和另一个缓冲区做对比

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180442.png)

  往上寻找EBP-0x28 ,发现刚才忽略的函数有利用到这两个缓冲区

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180446.png)

  在分析sub_401019 中发现,这个应该是加密函数

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180450.png)

  函数的参数有两个,于是回去看看到底程序传哪两个参数进去

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180454.png)

  可以看到,一个是0x10 ,一个是缓冲区地址,所以我们可以大胆的确定,a1 是要加密的数据地址,a2 是数据长度

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180500.png)

  给加密函数中的变量写好命名,整体的加密流程也就一看明了

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180504.png)

  于是获取到这段加密过后的字符串

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ图片20160122180511.png)

  放到python 里面解码

    def decode(string) :
        for index in range(0,16,2) :
            string[index]=string[index]^16
            string[index+1]=string[index+1]^16
            bit_1_high=string[index]&0xF
            bit_2_low=(string[index]&0xF0)>>4
            bit_2_high=string[index+1]&0xF
            bit_1_low=(string[index+1]&0xF0)>>4
            source_bit_1=bit_1_high*16+bit_1_low
            source_bit_2=bit_2_high*16+bit_2_low
            print chr(source_bit_1),chr(source_bit_2),
        
    code=[0xE7,0x86,0xE7,0x45,0x06,0x26,0xE6,0xF5,0x06,0xC6,0x46,0xA6,0x05,0xE6,0xD6,0xD6]
    decode(code)

  ![](https://raw.githubusercontent.com/lcatro/SISE_Traning_CTF_RE/master/Writeup_Picture/QQ截图20160122183538.png)

  合起来就是hrctf:{you_can_make_all}

