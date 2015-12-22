//
//  RWFunction.c
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/18.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "RWFunction.h"
#include "RWLine.h"
#include "RWStack.h"

int isCPlusPlusFunction(const char * str)
{
    return 0;
}
int isOCFunction(const char * str)
{
    return 0;
}
int isSwiftFunction(const char * str)
{
    return 0;
}
/*
 是否是函数，是返回1，不是则返回0
 一个标识符，空格，标识符，（），｛｝
 void test(){}
 struct student test(int a){}
 */
int isCLanguageFunction(const char * str)
{
    const char * pSpace = NULL;
    const char * start = str;
    int ret = FALSE;
    //注释行
    if (halfCommnetLine || isCommnetLine(str))
    {
        return FALSE;
    }
    
    while (*str){
        switch (*str) {
            case ' ':
                if (!pSpace){
                    pSpace = str;//查找空格的位置
                    halfFunctionLine |= SPACE;
                    //在进入该函数之前已经删除了字符串开头的空格
                    //所以目前空格前面一定是有字符串的，暂时把这个字符串当作返回值
                    halfFunctionLine |= RETURNTYPE;
                }
                break;
            case '(':
                //有左小括号，则入栈
                push('(');
                //左小括号前面是函数名
                halfFunctionLine |= FUNCNAME;
                halfFunctionLine |= LEFTSBRACKET;
                break;
            case ')':
                //如果栈顶是‘(’,正好配对，出栈
                if (top() == '(') {
                    pop();
                }else{
                    //如果栈顶不是‘(’,不能配对，入栈
                    push(')');
                }
                halfFunctionLine |= RIGHTSBRACKET;
                break;
            case '{':
                push('{');
                halfFunctionLine |= LEFTLBRACKET;
                break;
            case '}':
                if (top() == '{') {
                    pop();
                }
                else{
                    push('}');
                }
                halfFunctionLine |= RIGHTLBRACKET;
                break;
            default:
                break;
        }
        str++;
    }

    //目前的格式是：标识符，空格，标识符（）
    if (is_empty()
     && strstr(start, "}\n")
     && halfFunctionLine == FUNCTION) {
        halfFunctionLine = 0;
        ret = TRUE;
    }
    
    return ret;
}