//
//  RWLine.h
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/18.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __CodeAnalizer__RWLine__
#define __CodeAnalizer__RWLine__

#include <stdio.h>
#include <string.h>

#define  TRUE 1
#define  FALSE 0

//半个有效行的个数
extern int halfEffectiveCodeLine;
//半个注释的个数
extern int halfCommnetLine;
//半个空行的个数
extern int halfBlankLine;
//半个函数的个数
extern int halfFunctionLine;

/*
 是否是有效行，是返回1，不是则返回0
 参数为文件中的某一行
 有效行：
 1.用“；”或者“｛”或者“｝”结尾
 2.行首不能有//和（左多行注释）
 3.栈里不能有未配对的(左多行注释)
 */
int isEffectiveCodeLine(const char * str);
/*
 是空行，返回1,不是，返回0
 */
int isBlankLine(const char * str);

/*
 注释行
 以//开头，或者/星星/中间的部分
 */
int isCommnetLine(const char * str);
/*
 预处理语句,是，返回1,不是，返回0
 以#开头，\n结束
 */
int isPreprocessStatements(const char * str);

//src是否以subStr结尾，是返回1，不是则返回0
int hasSuffix(const char * src, const char * subStr);
//src是否以subStr开头，是返回1，不是则返回0
int hasPrefix(const char * src, const char * subStr);
/*
 忽略字符串的开头的几个字符，把字符串中非空格的位置返回
 */
char * ignoreSpaceInString(const char * src);

//分析文件中的每一行，返回总行号
int lineAnalize(const char * p);
#endif /* defined(__CodeAnalizer__RWLine__) */
