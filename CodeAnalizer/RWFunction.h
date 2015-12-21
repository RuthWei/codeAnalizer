//
//  RWFunction.h
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/18.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __CodeAnalizer__RWFunction__
#define __CodeAnalizer__RWFunction__

#include <stdio.h>
#include <string.h>
#define  TRUE 1
#define  FALSE 0

//函数返回值
#define RETURNTYPE    0x0001
//函数返回值与函数名之间的空格
#define SPACE         0x0002
//函数名
#define FUNCNAME      0x0004
//左小括号
#define LEFTSBRACKET  0x0008
//右小括号
#define RIGHTSBRACKET 0x0010
//左大括号
#define LEFTLBRACKET  0x0020
//右大括号
#define RIGHTLBRACKET 0x0040
//函数的四要素组合全部齐了
#define FUNCTION      0x007F
/*
 是否是函数，是返回1，不是则返回0
 */
int isFunction(const char * str);
#endif /* defined(__CodeAnalizer__RWFunction__) */
