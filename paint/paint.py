#!/usr/bin/env python
# coding=utf-8

# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt

name_list = ['1','8','90','1000']
name_list1 = ['1','8','90','1000']
num_list = [779200,2376466,2411873,2370371]
num_list1 = [744204,2816412,2880995,2827004]
x =list(range(len(num_list)))
total_width, n = 0.5, 2
width = total_width / n
plt.ylabel('pages/min')
plt.xlabel('client_number') 
plt.bar(x, num_list, width=width,  label='nginx',tick_label=name_list, fc='gray')
for i in range(len(x)):
    x[i] = x[i] + width
plt.bar(x, num_list1, width=width, label='ck_webServer',tick_label=name_list1,fc = 'pink')
for i in range(len(x)):
    x[i] = x[i] + width
plt.legend()
plt.show()
