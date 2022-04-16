[global shutdown]

shutdown:
    mov ax,5307h
    mov bx,0001h
    mov cx,0003h
    int 15h
    ret