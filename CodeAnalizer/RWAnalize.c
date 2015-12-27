//
//  RWAnalize.c
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/19.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "RWAnalize.h"
#include "RWStack.h"
#include "RWLine.h"
//#include "RWFunction.h"
#include "RWMenu.h"

//分析文件类型，c, c++, oc, 头文件
FILE_TYPE_E getFileType(const char * path)
{
    if (hasSuffix(path, ".h")) {
        return HEADER_FILE;
    }
    else if (hasSuffix(path, ".c")){
        return C_LANGUAGE_SOURCE;
    }
    else if (hasSuffix(path, ".cpp")){
        return C_PLUS_PLUS_SOURCE;
    }
    else if (hasSuffix(path, ".m")){
        return OC_SOURCE;
    }
    else if (hasSuffix(path, ".swift"))
    {
        return SWIFT_FILE;
    }
    return OTHRES;
}

void initGlobalVariables()
{
    //半句话
    halfBlankLine = FALSE;//空行
    halfCommnetLine = FALSE;//注释
    halfEffectiveCodeLine = FALSE;//有效行
    //半个函数的个数
    halfFunctionLine = FALSE;
    //行号
    uiBlankLineNum = 0;//空行
    uiAnnotationLineNum = 0;//注释
    uiEffectiveCodeLineNum = 0;//有效行
    uiFunctionNum = 0;
    
    //栈清空
    while(!is_empty())
    {
        pop();
    }
    //栈里有未配对的（
    hasLeftSmallBrackets = 0;
    //栈里有未配对的［
    hasLeftMiddleBrackets = 0;
    //栈里有未配对的｛
    hasLeftLargeBrackets = 0;
    //栈里有未配对的/*
    hasLeftAnnotations = 0;
    //栈里有未配对的*/
    hasRightAnnotations = 0;
}

void outputResult()
{
    printf("%8d%10d%10d\n",uiBlankLineNum,uiEffectiveCodeLineNum,uiAnnotationLineNum);
}
void analizeCSourceFile(FILE * fp)
{
    char buf[200] = {};
    int totalLine = 0;
    //文件指针不存在
    if (!fp) {
        printf("open file error\n");
        exit(1);
    }
    //文件没有读完，接着分析
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 199, fp);//buf太小一行没有全部读完
        
        //避开行首的空格和\t
        char * p = ignoreSpaceInString(buf);
        totalLine += lineAnalize(p);
        //uiFunctionNum += isCLanguageFunction(p);
    }
    
    outputResult();
}
void analizeCPlusPlusSourceFile(FILE * fp)
{
    char buf[200] = {};
    int totalLine = 0;
    //文件指针不存在
    if (!fp) {
        printf("open file error\n");
        exit(1);
    }
    //文件没有读完，接着分析
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 199, fp);//buf太小一行没有全部读完
        
        //避开行首的空格和\t
        char * p = ignoreSpaceInString(buf);
        totalLine += lineAnalize(p);
        
    }
    
    outputResult();
}
void analizeOCSourceFile(FILE * fp)
{
    char buf[200] = {};
    int totalLine = 0;
    //文件指针不存在
    if (!fp) {
        printf("open file error\n");
        exit(1);
    }
    //文件没有读完，接着分析
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 199, fp);//buf太小一行没有全部读完
        
        //避开行首的空格和\t
        char * p = ignoreSpaceInString(buf);
        totalLine += lineAnalize(p);
        
    }
    
    outputResult();
}

void analizeSwiftSourceFile(FILE * fp)
{
    char buf[200] = {};
    int totalLine = 0;
    //文件指针不存在
    if (!fp) {
        printf("open file error\n");
        exit(1);
    }
    //文件没有读完，接着分析
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 199, fp);//buf太小一行没有全部读完
        
        //避开行首的空格和\t
        char * p = ignoreSpaceInString(buf);
        totalLine += lineAnalize(p);
        
    }
    
    outputResult();
}

void analizeHeaderSourceFile(FILE * fp)
{
    char buf[200] = {};
    int totalLine = 0;
    //文件指针不存在
    if (!fp) {
        printf("open file error\n");
        exit(1);
    }
    //文件没有读完，接着分析
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 199, fp);//buf太小一行没有全部读完
        
        //避开行首的空格和\t
        char * p = ignoreSpaceInString(buf);
        totalLine += lineAnalize(p);
    }

    printf("%8d%10d%10d\n",uiBlankLineNum,uiEffectiveCodeLineNum,uiAnnotationLineNum);
}