//
//  RWStack.c
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/18.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include<assert.h>
#include<stdio.h>
#include "RWStack.h"
/*
 **
 ** 静态数组实现堆栈程序 ，数组长度由#define确定
 */

#define STACK_SIZE 100 /* 堆栈最大容纳元素数量 */
//栈里有未配对的（
int hasLeftSmallBrackets = 0;
//栈里有未配对的［
int hasLeftMiddleBrackets = 0;
//栈里有未配对的｛
int hasLeftLargeBrackets = 0;
//栈里有未配对的/*
int hasLeftAnnotations = 0;
//栈里有未配对的*/
int hasRightAnnotations = 0;
//有效行的个数
unsigned int uiEffectiveCodeLineNum = 0;
//注释的个数
unsigned int uiAnnotationLineNum = 0;
//空行的个数
unsigned int uiBlankLineNum = 0;
//总函数的个数
unsigned int uiFunctionNum = 0;

/*
 ** 存储堆栈中的数组和一个指向堆栈顶部元素的指针
 */
static STACK_TYPE stack[STACK_SIZE];
static int top_element = -1;

/* push */
void push(STACK_TYPE value)
{
    assert(!is_full()); /* 压入堆栈之前先判断是否堆栈已满*/
    top_element += 1;
    stack[top_element] = value;
//    printf("<--%c\n", value);
    switch (value) {
        case 'l':
        case 'L':
            hasLeftAnnotations++;
            break;
        case 'r':
        case 'R':
            hasRightAnnotations++;
            break;
        case '(':
            hasLeftSmallBrackets++;
            break;
        case '[':
            hasLeftMiddleBrackets++;
            break;
        case '{':
            hasLeftLargeBrackets++;
            break;
        default:
            break;
    }
}

/* pop */
void pop(void)
{
//    printf("-->%c\n", stack[top_element]);
    assert(!is_empty()); /* 弹出堆栈之前先判断是否堆栈已空 */
    switch (stack[top_element]) {
        case 'l'://注释
        case 'L':
            hasLeftAnnotations--;
            break;
        case 'r'://注释
        case 'R':
            hasRightAnnotations--;
            break;
        case '(':
            hasLeftSmallBrackets--;
            break;
        case '[':
            hasLeftMiddleBrackets--;
            break;
        case '{':
            hasLeftLargeBrackets--;
            break;
        default:
            break;
    }
    top_element -= 1;
}

/* top */
STACK_TYPE top(void)
{
    assert(!is_empty());
    return stack[top_element];
}

/* is_empty */
int is_empty(void)
{
    return top_element == -1;
}

/* is_full */
int is_full(void)
{
    return top_element == STACK_SIZE - 1;
}

/*
 ** 定义一个print函数，用来打印堆栈里面的元素。
 */
void print(void)
{
    int i;
    i = top_element;
    printf("打印出静态数组堆栈里面的值: ");
    if(i == -1)
        printf("这是个空堆栈\n");
    while(i!= -1)
        printf("%d ",stack[i--]);
    printf("\n");
}