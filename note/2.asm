;从键盘输入一串字符，个数小于50
data segment 
    buf db 50
    db ?
    db 50 dup(?)
data ends
stack segment stack
stack ends
code segment
    assume ds:data, cs:code, ss:stack
    start :
        mov ax, dta
        mov ds, ax
        mov bx, 0
        lea dx, buf 
        mov ah, 10
        int 21h
        mov ah, 4ch
        int 21h
code ends
    end start
