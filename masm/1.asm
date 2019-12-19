;实现多字节相加
;3b74ac60f8h
;30d59e36c1h
data segment 
data1 db 0f8h, 60h, 0ach, 74h, 3bh
data2 db 0c1h, 36h, 9eh, 0d5h, 20h
data end

code segment 
    assume cs:code, ds:data
    start :
        mov ax, data 
        mov ds, ax
        mov cx, 5
        mov si,0
        mov ah, 4ch
        clc
    looper:
        mov al, data2[si]
        adc data1[si], al
        inc si
        dec cx
        jnz looper
        
        mov ah, 01h
        mov dl, data1
        int 21h

        mov ah, 4ch
        int 21h
code ends
end start 
