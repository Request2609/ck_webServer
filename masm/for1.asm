;从键盘输入十进制数，输出该十进制数的十进制，二进制（B)，十六进制(H)，八进制(Q)
;用循环左移指令做的输出函数
DATA SEGMENT ;此处输入数据段代码  
INFO1   DB   0DH,0AH,'INPUT A NUMBER:$'
INFO2   DB   0DH,0AH,'THE DECIMAL OF THIS NUMBER IS:$'
INFO3   DB   0DH,0AH,'THE HEXIMAL OF THIS NUMBER IS:$'
INFO4   DB   0DH,0AH,'THE QECIMAL OF THIS NUMBER IS:$'
INFO5   DB   0DH,0AH,'THE BINIMAL OF THIS NUMBER IS:$'
DATA ENDS
ZHUANHUA SEGMENT
  ASSUME CS:ZHUANHUA,DS:DATA
  

MAIN PROC FAR
       MOV AX,DATA
       MOV DS,AX
REPET:
       CALL DECIBIN    ;十进制转化到二进制，输入函数   
       CALL BINIDEC    ;二进制转化到十进制输出
       CALL CRLF       ;输出换行
       CALL BINIHEX    ;二进制转化到十六进制输出
       CALL CRLF  
       CALL BINIQEC    ;二进制转化到八进制输出
       CALL CRLF
       CALL BINIBIN    ;二进制转到二进制并输出
       CALL CRLF  
       CALL STOP     
       JMP REPET 
MAIN ENDP

DECIBIN PROC NEAR      ;十进制转化到二进制函数
        LEA  DX,INFO1
        MOV  AH,09H           
        INT  21H     
        MOV BX,0
NEWCHAR:MOV AH,01H
        INT 21H
        SUB AL,30H
        JL EXIT
        CMP AL,9D
        JG EXIT
        CBW 
        XCHG AX,BX
        MOV CX,10D
        MUL CX
        XCHG AX,BX
        ADD BX,AX
        JMP NEWCHAR
EXIT: 
        RET
       
DECIBIN ENDP

BINIDEC PROC NEAR      ;用除10取余法输出十进制
                      
         LEA  DX,INFO2
         MOV  AH,09H           
         INT  21H     
         MOV CX,3
         MOV AX,BX
         PUSH BX
         MOV BX,10
    LP1: XOR DX,DX    
         DIV BX
         ADD DL,30H
         PUSH DX
         LOOP LP1
         MOV CX,3
    LP2: POP DX
         MOV AH,02H
         INT 21H
         LOOP LP2
         POP BX
         RET
BINIDEC ENDP

BINIHEX PROC NEAR   ;输出十六进制
        LEA  DX,INFO3
        MOV  AH,09H           
        INT  21H     
        MOV CH,4
ROTATE: MOV CL,4
        ROL BX,CL
        MOV AL,BL
        AND AL,0FH
        ADD AL,30H
        CMP AL,3AH
        JL  PRINTIT
        ADD AL,7H
PRINTIT:MOV DL,AL
        MOV AH,02H
        INT 21H
        DEC CH
        JNZ  ROTATE
        SUB AX,AX   ;输出H
        ADD AL,48H
        MOV DL,AL
        MOV AH,02H
        INT 21H
        RET
BINIHEX ENDP

BINIQEC PROC NEAR  ;输出八进制
        LEA  DX,INFO4
        MOV  AH,09H           
        INT  21H    
        PUSH BX 
        SHL BX,1    
        MOV CH,5
ROTATE: MOV CL,3
        ROL BX,CL
        MOV AL,BL
        AND AL,07H
        ADD AL,30H
        MOV DL,AL
        MOV AH,02H
        INT 21H
        DEC CH
        JNZ  ROTATE
        POP BX
        SUB AX,AX  ;输出Q
        ADD AL,51H
        MOV DL,AL
        MOV AH,02H
        INT 21H
        RET
BINIQEC ENDP        
        
BINIBIN PROC NEAR       ;输出二进制
        LEA  DX,INFO5
        MOV  AH,09H           
        INT  21H    
        PUSH BX   
        MOV CH,16
ROTATE: MOV CL,1
        ROL BX,CL
        MOV AL,BL
        AND AL,01H
        ADD AL,30H
        MOV DL,AL
        MOV AH,02H
        INT 21H
        DEC CH
        JNZ  ROTATE
        POP BX
        SUB AX,AX     ;输出B
        ADD AL,42H
        MOV DL,AL
        MOV AH,02H
        INT 21H
        RET  
BINIBIN ENDP
  
CRLF   PROC NEAR    ;输出换行符
       MOV DL,0DH
       MOV AH,2
       INT 21H
       MOV DL,0AH
       MOV AH,2
       INT 21H
       RET
CRLF  ENDP
STOP PROC NEAR         ;用于结束MAIN过程
      MOV AX,4C00H
      INT 21H
      RET
STOP ENDP
ZHUANHUA    ENDS
  END MAIN    

