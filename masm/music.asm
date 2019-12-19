data segment 
    table dw 262 
        dw 294
        dw 330
        dw 349
        dw 392
        dw 440
        dw 494
data ends 

stack segment para 'stack'
    db 64 dup('stack')
stack ends

code segment 
    assume ss:stack, ds:data, cs:code 
    start :
        mov ds, data
        mov ax, ds 
        mov ah, 01
        int 21h
        cmp al, '0'

        je exit

        mov bx, offset table
        
        cmp al, '1'
        jb start
        cmp al, '7'
        ja start 
        
        and ax, 0fh
        shl ax, 1
        sub ax, 2
        mov si, ax
        mov di, [bx][si]
        mov bx, 10
        call sound 
        jmp start 
    exit:
        mov ah, 4ch
        int 21h
code ends
    end start 

waitf proc
    push ax
waitf1: 
int al, 61h
and al, 10h
cmp al, ah
je waitf1
mov ah, al
loop waitf1
pop ax
ret 
waitf endp 

sound proc
    push ax
    push bx 
    push cx
    push dx
    push di
    mov al, 0b6h
    out 43h, al
    mov dx, 12h
    mov ax, 348ch
    div di
    out 42h, al
    mov al, ah
    out 42h, al
    in al. 61h
    mov ah, al
    or al, 3
    out 61h
wait1:
    mov  cx, 3314
    call waitf
delay :
dec bx
jnz wait1
mov al, ah
out 61h, al
 pop di
 pop dx
 pop cs
 pop bx
 pop ax
 ret
sound endp ;


