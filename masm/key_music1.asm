DATA SEGMENT
msg DW 0DH,0AH,'[ 1 2 3 4 5 6 7 8] 9 :exit'
;DB 0DH,0AH,' [ q w e r t y u ]'
; DB 0DH,0AH,'_________________'
; DB 0DH,0AH,'9: EXIT'
DB 0DH,0AH,'_________________','$'
;********音调******后面用到-1判断音乐播是否放完
sound_0 DW -1
sound_11 DW 441,-1
sound_12 DW 495,-1
sound_13 DW 556,-1
sound_14 DW 589,-1
sound_15 DW 661,-1
sound_16 DW 742,-1
sound_17 DW 833,-1
sound_1 DW 882,-1
sound_2 DW 990,-1
sound_3 DW 1112,-1
sound_4 DW 1178,-1
sound_5 DW 1322,-1
sound_6 DW 1484,-1
sound_7 DW 1655,-1
time DW 25
DATA ENDS
;************************
STACK SEGMENT
Db 200 DUP ('STACK')
STACK ENDS
;***********************
CODE SEGMENT
ASSUME DS:DATA,SS:STACK,CS:CODE
START:
MOV AX,DATA
MOV DS,AX
MOV aH,0
MOV AL,00
INT 10H
;*****定义一个宏*****
SHOW MACRO b
LEA DX,b
MOV AH,9
INT 21H

ENDM

;********一个声音宏********
onesound macro soundis,jumpis,letteris
CMP AL,letteris
JNZ jumpis
LEA SI,soundis
LEA BP,DS:time
CALL MUSIC
JMP input
ENDM

    show msg
    INPUT: MOV AH,01H
    INT 21H
    CMP AL,'9'
    jnz go
    mov ah,4ch
    int 21h

    go: onesound sound_11,a0,'q'
    a0: onesound sound_12,b0,'w'
    b0: onesound sound_13,c0,'e'
    c0: onesound sound_14,d0,'r'
d0: onesound sound_15,e0,48h
e0: onesound sound_16,f0,50h
f0: onesound sound_17,g0,48h
g0: onesound sound_1,h0,48h
h0: onesound sound_2,i0,50h
i0: onesound sound_3,j0,48h
j0: onesound sound_4,k0, 50h
k0: onesound sound_5,l0,48h
l0: onesound sound_6,m0, 48h
m0: onesound sound_7,n0,50h
n0: onesound sound_0,o0,al
o0: jmp input

;********************发声
GENSOUND PROC NEAR
;-------------
PUSH AX
PUSH BX
PUSH CX
PUSH DX
PUSH DI
;--------------
MOV AL,0B6H
OUT 43H,AL
MOV DX,12H
MOV AX,348ch
DIV DI
OUT 42H,AL
MOV AL,AH
OUT 42H,AL
IN AL,61H
MOV AH,AL
OR AL,3
OUT 61H,AL
WAIT1: MOV CX,3314
    call waitf
DELAY1: DEC BX
JNZ WAIT1
MOV AL,AH
OUT 61H,AL
;----------------
POP DI
POP DX
POP CX
POP BX
POP AX
;-------------------
RET
GENSOUND ENDP
;**************************
waitf proc near
    push ax
    waitf1:
        in al,61h
        and al,10h
        cmp al,ah
        je waitf1
        mov ah,al
    loop waitf1
    pop ax
    ret
    waitf endp

;************************
MUSIC PROC NEAR
PUSH DS
SUB AX,AX
PUSH AX
FREG: MOV DI,[SI]
CMP DI,-1;音乐是否放完？
JE END_MUS
MOV BX,DS:[BP]
CALL GENSOUND
ADD SI,2
ADD BP,2
JMP FREG
END_MUS:
RET
MUSIC ENDP
CODE ENDS
;**********************
END START
 
