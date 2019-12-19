
DATA SEGMEN
    BLANK DB ' aaaaaaa$'
DATA ENDS 

CODE SEGMENT 
ASSUME CS:DATA, DS:DATA
START :
    MOV AX, DATA
    MOV DS, AX
    mov ah, 09h
    mov dx, offset BLANK
    int 21h

    call speakon
    mov ah, 01h
    int 21h
    cmp al, '0'
    call speakoff 

    je exit
    jmp start 
exit:
    MOV AH, 4CH
    INT 21H
CODE ENDS
    ENDS START 

speakon proc ; 扬声器开子程序                                                    
 push ax
 in al,61h ; 读取61H端口的原控制信息
 or al,03h ; D1D0＝11，其他位不变
 out 61h,al ; 直接控制发声
 pop ax
 ret
speakon endp
 ;
speakoff proc ; 扬声器关子程序
 push ax
 in al,61h
 and al,0fch ; D1D0＝00，其他位不变 
 out 61h,al ; 直接控制闭音
 pop ax
 ret
speakoff endp

;发音频率设置子程序，入口参数：AX＝1193180÷发音频率
speaker proc
 push ax ; 暂存入口参数以免被破坏
 mov al,0b6h ; 定时器2为方式3，先低后高写入16位计数值
 out 43h,al
 pop ax ; 恢复入口参数
 out 42h,al ; 写入低8位计数值
 mov al,ah
 out 42h,al ; 写入高8位计数值
 ret
speaker endp
