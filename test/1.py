#!/usr/bin/env python
# coding=utf-8
# 最大公约数
# 最大公约数
def gcd(a, b):
    while b != 0:
        tem = a % b
        a = b
        b = tem
    return a

def ischar(c) : 
    return ((c>='a' and c <= 'z') or (c>='A' and c<='Z'))
# 加密 
def encrypt(m, c, a, b):
    for i in range(len(m)):
        # 加密成相应的大写字母
        if ischar(m[i]) :
            c.append(chr(((ord(m[i]) - 97) * a + b) % 36 + 65))
        else :
            #122是z的ascll码，123开始往后递增是0-9的数字，将数字和a-z连着，-97使a-z 0-9转换成为0-35的范围，
	    #后面的加65是将加密后的字符转换成大写字母
            k = chr(((123+(ord(m[i])-49)-97)*a+b) % 36 + 65)        
            c.append(k) 
    d = ''.join(c)
    print(d)

# 求逆元
def niyuan(a, b):
    ny = 1
    while (a * ny) % b != 1:
        ny += 1
    return ny

# 解密
def decrypt(c, k, b):
    mw = []
    for i in range(len(c)):
        tem = ord(c[i]) - 65 - b
        if tem < 0 :
            tem += 36
        print(tem) 
        if(tem >= 25) :
            mw.append(chr((k * tem) % 36+97+49-123)) 
        else :
            mw.append(chr((k * tem) % 36 + 97))
    print("k=" + str(k) + ", b=" + str(b) + "时，解密后的明文为：")
    res = ''.join(mw)
    print(res)

#实现
if __name__ == "__main__":
    # 明文
    m = 'lala000999'
    # 密文
    c = []
    #学号后三位
    x = 106
    #学号后四位
    y = 3106 
    a = int(x)%36 
    b = int(y)%36
    while gcd(a, b) != 1:
        #要是不互素的话，a=a+7
        a = a+7 
    print("明文内容为：")
    print(m)
    print("加密后的密文为：")
    encrypt(m, c, a, b)
    print("知道密钥破解：")
    k = niyuan(a, 36)
    decrypt(c, k, b)
