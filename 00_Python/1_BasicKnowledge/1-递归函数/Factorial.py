# -*- coding: utf-8 -*-
# Factorial.py
# @author guokonghui
# @description
# @created 2019-09-03T22:25:34.267Z-04:00
# @last-modified 2019-09-03T22:39:58.518Z-04:00
#


def factorial(n):
    result = 1
    if n < 0:
        print("Negative numbers cannot carry out factorial operations.")
        result = 0
    elif (1 == n) | (0 == n):
        result = 1
    else:
        result = n * factorial(n - 1)
    return result


def main():
    r = factorial(5)
    print(r)


if __name__ == '__main__':
    main()
