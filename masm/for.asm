INF1 DB "PLEASE INPUT A NUMBER(1-361):$"
IBUF DB 7,0,6 DUP(?)
OBUF DB 6 DUP(?)

CODE SEGMENT 
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA 
    MOV DS, AX
    ;从键盘输入的十进制数，并将其转换成二进制数的指令序列
    MOV DX, INF1
    MOV AH, 09H
    INT 21H
    
    MOV BX, 0
;    MOV DX, OFFSET IBUF //输入一个十进制数字
 ;   MOV AH, 0AH
  ;  INT 21H
;
 ;   MOV CL, IBUF+1
   ; MOV CH, 0
    
    ;MOV SI, OFFSET IBUF+2
   ; MOV AX, 0
    
   ; MOV CX, AX
    ;MOV AX, 0
    ;MOV BX, 1
LOOP2:  ADD AH, 01H
        INT 21H
        SUB AL, 30H
        JL EXIT
        CMP AL, 9D
        JG EXIT
        LOOP LOOP2
        ;将ax中存放的二进制转换成十进制数并显示的指令序列

