# MDK下lua移植到stm32

## 移植lua

l  添加lua源码到MDK，把lua.c 和luac.c删除。

l  修改堆栈大小：堆最小为5.5kb，栈最小是1.5kb。lua的空间分配在堆空间。16kb的内存建议分配是堆11Kb，栈4Kb，留1Kb给全局变量。

在startup_stm32f10x_md.s文件中

;stack size 0x1000 == 4Kb

Stack_Size     EQU     0x00001000

;heap size 0x2c00 == 11Kb

Heap_Size       EQU     0x00002C00

l  在mian.c文件里引用lua.h头文件。
l  
l  
l  