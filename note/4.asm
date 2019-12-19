;实现输出
data segment 
    buf db 'how do you do?$'
data ends
stack segment stack
stack ends
code segment
    assume ds:data, cs:code, ss:stack
    start :
        mov ax, data
        mov ds, ax
        
        mov dx, offset buf
        mov ah, 9
        int 21h

        mov ah, 4ch
        int 21h
code ends
    end start
