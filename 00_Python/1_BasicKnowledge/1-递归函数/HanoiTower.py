# -*- coding: utf-8 -*-
# HanoiTower.py
# @author guokonghui
# @description 
# @created 2019-09-06T19:47:04.413Z+08:00
# @last-modified 2019-09-06T19:47:04.413Z+08:00
#

def Hanoi(n, sor, mid, des):
    if 1 == n:
        print(sor, '——>', des)
    else:
        Hanoi(n - 1, sor, des, mid)
        print(sor, '——>', des)
        Hanoi(n - 1, mid, sor, des)


def main():
    Hanoi(3, 'A', 'B', 'C')


if __name__ == '__main__':
    main()
