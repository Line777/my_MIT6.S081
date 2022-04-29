# lab2
>>本实验要完成两个系统调用trace和sysinfo。最重要的是体会应用程序调用系统调用到获得数据的全过程。总的来说为应用程序调用系统调用，

系统调用通过ecall（一种中断）到达内核指定位置，然后通过syscall将对应系统调用的位置（编号）存入寄存器a0，然后调用对应的内核函数，将数据返回。

系统调用跳转过程：

user/user.h:		用户态程序调用系统调用函数 trace()

user/usys.S:		 系统调用函数 trace() 使用 CPU 提供的 ecall 指令，调用到内核态

kernel/syscall.c	到达内核态统一系统调用处理函数 syscall()，所有系统调用都会跳到这里来处理。

kernel/syscall.c	syscall() 根据跳板传进来的系统调用编号，查询 syscalls[] 表，找到对应的内核函数并调用。

kernel/sysproc.c	到达 sys_trace() 函数，执行具体内核操作

sysinfo过程同上。


