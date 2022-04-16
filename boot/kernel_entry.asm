global _start;
[bits 32]

_start:
    [extern main]
    call main  ;;进入内核   c代码
    jmp $