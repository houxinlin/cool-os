[bits 16]
switch_to_32:
    cli ; 关闭中断
    lgdt [gdt_descriptor] ; 加载gdt描述符
    mov eax, cr0
    or eax, 0x1 ; 设置cr0标志位，进入32保护模式
    mov cr0, eax
    jmp CODE_SEG:init_32 

[bits 32]
init_32:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; 设置堆栈
    mov esp, ebp

    call ENTRY_32BIT 
