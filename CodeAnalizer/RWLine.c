//
//  RWLine.c
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/18.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "RWLine.h"
#include "RWStack.h"

//半个有效行的个数
int halfEffectiveCodeLine = 0;
//半个注释的个数
int halfCommnetLine = 0;
//半个空行的个数
int halfBlankLine = 0;
//半个函数的个数
int halfFunctionLine = 0;

/*
 是否是有效行，是返回1，不是则返回0
 参数为文件中的某一行
 有效行：
 //1.用“；”或者“｛”或者“｝”或者“）”结尾
 //2.行首不能有//和（左多行注释）
 //3.栈里不能有未配对的(左多行注释)
 //4.预处理语句
 */
int isEffectiveCodeLine(const char * str)
{
    if (hasLeftAnnotations > 0) {
        //栈里有左多行注释,并且在栈顶
        //if (LeftAnnotations == top())
        {
            return FALSE;
        }
    }
    //行首不能有//和（/*）
    if (hasPrefix(str, "/*")
     || hasPrefix(str, "//")) {
        return FALSE;
    }
    //
    if (!hasSuffix(str, "\n")) {
        halfEffectiveCodeLine = 1;
    }
    else
    {
        halfEffectiveCodeLine = 0;
    }
    return TRUE;
}
//src是否以subStr结尾，是返回1，不是则返回0
int hasSuffix(const char * src, const char * subStr)
{
    //从字符串的最后几个字符开始比较
    if (0 == strncmp(src + strlen(src) - strlen(subStr), subStr, strlen(subStr)))
    {
        return TRUE;
    }
    return FALSE;
}
//src是否以subStr开头，是返回1，不是则返回0
int hasPrefix(const char * src, const char * subStr)
{
    //char * p = ignoreSpaceInString(src);
    //从字符串的开头非空格的几个字符开始比较
    if (0 == strncmp(src, subStr, strlen(subStr)))
    {
        return TRUE;
    }
    return FALSE;
}
/*
 忽略字符串的开头的几个字符，把字符串中非空格的位置返回
 */
char * ignoreSpaceInString(const char * src)
{
    while (*src) {
        if (' ' != *src && '\t' != *src) {
            break;
        }
        src++;
    }
    return (char *)src;
}
/*
 预处理语句,是，返回1,不是，返回0
 以#开头，\n结束
 */
int isPreprocessStatements(const char * str)
{
    if (hasLeftAnnotations > 0) {
        //栈里有左多行注释,并且在栈顶
        //if (LeftAnnotations == top())
        {
            return FALSE;
        }
    }
    if (hasPrefix(str, "#") && hasSuffix(str, "\n")) {
        return TRUE;
    }
    return FALSE;
}
/*
 是空行，返回1,不是，返回0
 */
int isBlankLine(const char * str)
{
    int ret = TRUE;
    if (hasLeftAnnotations > 0) {
        //栈里有左多行注释,并且在栈顶
        //if (LeftAnnotations == top())
        {
            ret = FALSE;
        }
    }
    //如果该行只有\n，则是空行
    if ('\n' == *str) {
        ret = TRUE;
    }
    //如果该行除去空格、\t、\n，再没有其它字符，那么它也是空行
    while (*str) {
        if (*str != ' '
         && *str != '\n'
         && *str != '\t') {
            ret =  FALSE;
            break;
        }
        str++;
    }
    //如果不是以\n结尾，则是半个空行
    if (!hasSuffix(str, "\n") && ret) {
        halfBlankLine = 1;
    }
    else
    {
        halfBlankLine = 0;
    }
    return ret;
}

/*
 注释行
 以//开头，或者/星星/中间的部分
 多行注释可以多次连续出现，只要出现右多行注释，则进行配对
 */
int isCommnetLine(const char * str)
{
    int ret = FALSE;
    //以//开头
    if (hasPrefix(str, "//")) {
        ret = TRUE;
    }
    //或者/星星/中间的部分
    
//    //如果是左多行注释和右多行注释
//    if (hasPrefix(str, "/*") && hasSuffix(str, "*/\n"))
//    {
//        ret = TRUE;
//    }
    //如果是左多行注释，没有右多行注释，则入栈
    if (hasPrefix(str, "/*")) {
        push('l');
        ret = TRUE;
    }
    
    //如果是右多行注释
    if (hasPrefix(str, "*/") || hasSuffix(str, "*/\n"))
    {
        //判断是不是上一个多行注释的另一半
        if (hasLeftAnnotations > 0) {
            //栈里有左多行注释,并且在栈顶
            if (LeftAnnotations == top()) {
                //出栈
                pop();
            }
        }
        ret = TRUE;
    }
    
    if (hasLeftAnnotations > 0) {
        //栈里有左多行注释,并且在栈顶
        //if (LeftAnnotations == top())
        {
            ret = TRUE;
        }
    }
    
    if (!hasSuffix(str, "\n") && ret == TRUE) {
        halfCommnetLine = 1;
    }
    else
    {
        halfCommnetLine = 0;
    }
    return ret;
}

//分析文件中的每一行，返回总行号
int lineAnalize(const char * p)
{
    int totalLine = 0;
    //空行
    if (!halfBlankLine && isBlankLine(p)) {
        uiBlankLineNum++;
    }
    //注释行
    else if (!halfCommnetLine && isCommnetLine(p))
    {
        uiAnnotationLineNum++;
    }
    //有效代码行
    else if (!halfEffectiveCodeLine)
    {
        uiEffectiveCodeLineNum++;
//        printf("%s",p);
    }
    
    isEffectiveCodeLine(p);
    if (!halfBlankLine && !halfCommnetLine && !halfEffectiveCodeLine) {
        totalLine++;
    }
    return totalLine;
}