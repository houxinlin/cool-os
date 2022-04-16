[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; 内核装载位置
xor ax,ax
mov ds,ax
mov es,ax

mov [BOOT_DRIVE], dl 
mov bp, 0x9000
mov sp, bp  ;使用0x9000这个位置作为栈，这个位置不能瞎设置，实模式只有不多的内存可以用
xchg bx, bx

call load_kernel ;从磁盘中加载内核
call switch_to_32 ;切换到32模式
jmp $

[bits 16]
load_kernel:

    mov bx, KERNEL_OFFSET ;将内核加载到这里
    mov dh,30
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret
disk_load:
    pusha
    push dx
    mov ah, 0x02 ; 02表示读取硬盘，下面是参数
    mov al, dh   
    mov cl, 0x02 
    mov ch, 0x00 
    mov dh, 0x00
    int 0x13      ; BIOS 中断
    pop dx
    popa
    ret
[bits 32]
ENTRY_32BIT:
    mov ebx, MSG_32BIT_MODE
    call print32        ;打印MSG_32BIT_MODE
    call KERNEL_OFFSET ;进入内核
    jmp $

%include "boot/print-32bit.asm"
%include "boot/gdt.asm"
%include "boot/switch-to-32bit.asm"

BOOT_DRIVE db 0 
MSG_32BIT_MODE db "Entry 32bit", 0

; 填充0
times 510 - ($-$$) db 0
dw 0xaa55
