STACK SEGMENT STACK 
    DB 256 DUP(?)
    TOP LABEL WORD
STACK ENDS

DATA SEGMENT 
    VAR1 DB (?)
    VAR2 DB (?)
DATA ENDS

CODE SEGMENT 
    ASSUME SS :STACK, DS:DATA
START:
    ;立即数寻址
    MOV AX, DATA ;DS ES SS　没有初始化
    MOV DS, AX
    MOV ES, AX
    ;后面三条操作系统已经初始化了
    MOV SS, STACK
    MOV SS, AX
    ;将段偏移量设置成top
    MOV SP, OFFSET TOP
    ;
    INT AH, 4CH
    INT 21H
CODE ENDS
    END START 
