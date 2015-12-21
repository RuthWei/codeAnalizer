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

/*
 是否是函数，是返回1，不是则返回0
 一个标识符，空格，标识符，（），｛｝
 void test(){}
 struct student test(int a){}
 */
int isFunction(const char * str)
{
    const char * pLeftBracket = NULL;
    const char * pRightBracket = NULL;
    const char * pSpace = NULL;
    const char * start = str;
    int has2SmallBracket = 0;
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
/*
    //查找空格的位置
    pSpace = strchr(str, ' ');
    //在空格前面有一个字符串是函数返回值类型
    if (!pSpace || pSpace <= str + 1) {
        ret = FALSE;
    }
    else{
        halfFunctionLine |= SPACE;
        //在进入该函数之前已经删除了字符串开头的空格
        //所以目前空格前面一定是有字符串的，暂时把这个字符串当作返回值
        halfFunctionLine |= RETURNTYPE;
    }
    //查看当前字符串中有没有左小括号
    pLeftBracket = strchr(str, '(');
    if (!pLeftBracket) {
        ret = FALSE;
    }
    else
    {
        //有左小括号，则入栈
        push('(');
        halfFunctionLine |= LEFTSBRACKET;
    }
    //有左小括号
    has2SmallBracket++;
    //查找空格的位置,括号和空格之间至少有一个字符
 
    //一般在函数定义时，函数的返回值类型不加括号。
    //所以，空格在括号的左侧。
    if (pSpace >= pLeftBracket + 1) {
        ret = FALSE;//空格在左括号的右侧
    }
    halfFunctionLine |= FUNCNAME;
    //本行是以)\n结尾，或者){\n结尾
//    if (!strstr(pLeftBracket + 1, ")\n")
//    &&  !strstr(pLeftBracket + 1, "){\n"))
    pRightBracket = strchr(str + 1, ')');
    if (!pRightBracket
     && has2SmallBracket) {//只有左括号，没有右括号
        ret = FALSE;
    }
    else if (pRightBracket && pRightBracket > pLeftBracket){
        //有右括号
        has2SmallBracket++;
        //如果栈顶是‘(’,正好配对，出栈
        if (top() == '(') {
            pop();
        }else{
            //如果栈顶不是‘(’,不能配对，入栈
            push(')');
        }
        halfFunctionLine |= RIGHTSBRACKET;
    }

    //查看当前字符串中有没有左大括号
    pLeftBracket = strchr(str, '{');
    pRightBracket = strchr(str, '}');
    if (pLeftBracket) {
        push('{');
        halfFunctionLine |= LEFTLBRACKET;
    }
    
    pRightBracket = strchr(str, '}');
    if (pRightBracket) {
        if (top() == '{') {
            pop();
        }
        else{
            push('}');
        }
        halfFunctionLine |= RIGHTLBRACKET;
    }
*/
    //目前的格式是：标识符，空格，标识符（）
    if (is_empty()
     && strstr(str, "}\n")
     && halfFunctionLine == FUNCTION) {
        halfFunctionLine = 0;
        ret = TRUE;
    }
    
    return ret;
}