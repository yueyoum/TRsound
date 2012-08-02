## 背景 ##
> 初衷是因为要看大量的英文文档，难免需要翻译。

> 先用Python做了一个google translate的程序。

> 但在使用中发现，如果不知道单词的发音，很难记住新单词.

> 所以就有了这个协助发音的程序

## 说明 ##
> C 程序使用 libmpg123 解码mp3， libao播放。

> 进入项目目录中 make , 就会生成两个文件：

> * wsound :  ./wsound "MUSIC FILE" 可以播放mp3

> * libwsound.so :  动态链接库, 其中的 <code>void decode(const char \*)</code> 提供给Python调用。

> 运行 googletrans.py 后，输入 字符 回车后翻译，输入 s 发音，输入 exit 退出

## 问题 ##
> 貌似libao动态链接库的问题，需要大家自己再编译一遍 libao

> 其实整个项目很简单，大家如果有需要自己编先编译 libao, libmpg123

> 然后根据自己的需要修改 Makefile 和 googletrans.py
