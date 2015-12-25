//
//  main.c
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/18.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include "RWMenu.h"
#include "RWAnalize.h"
#include "RWLine.h"

/*
 判断一个文件的行数
 总行数＝空行行数＋注释行数＋有效行数
 如果前面半句是有效行，后面有注释，则算做是有效行
 */

//#define DIRPATH "/Users/qianfeng/Documents/day02/赵晓东develop_project/gps"

void outputFolderResult(int c, int cpp, int oc, int header, int swift, int others)
{
    printf("total %d files", c + cpp + oc + swift + others + header);
    if (c > 0) {
        printf(", %d c",c);
    }
    if (cpp > 0) {
        printf(", %d c++",cpp);
    }
    if (oc > 0) {
        printf(", %d OC",oc);
    }
    if (header > 0) {
        printf(", %d c&&oc&&c++ header",header);
    }
    if (swift > 0) {
        printf(", %d swift",swift);
    }
    if (others > 0) {
        printf(", %d other files", others);
    }
    printf("\n");
}
int main(int argc, const char * argv[]) {
    // insert code here...

    char buf[200] = {};
    DIR * dir = NULL;
    struct dirent * entry;
    char * fileType = NULL;
    //各类文件的个数
    int cSourceFile = 0;
    int ocSourceFile = 0;
    int swiftFile = 0;
    int others = 0;
    int headerFile = 0;
    int cPlusPlusFile = 0;

    char DIRPATH[100] = {};
    printf("input file path \n");
    scanf("%99s", DIRPATH);

    //打开目录，
    dir = opendir(DIRPATH);
    if (!dir) {
        perror("open dir error");
        return 1;
    }
    startMenu();
    //读取目录，获得里面的文件名
    while(1){
        entry = readdir(dir);
        if (!entry) {
            break;
        }
        //在entry的dname里存放的是文件名
        strcpy(buf, DIRPATH);
        if (!hasSuffix(buf, "/")) {
            strcat(buf, "/");
        }
        //把文件路径和文件名放在buf这个数组中
        strcat(buf, entry->d_name);

        //打开文件进行分析
        FILE * fp = fopen(buf, "r");
        //新打开文件，初始化全局变量
        initGlobalVariables();
        
        switch (getFileType(buf)) {
            case C_LANGUAGE_SOURCE:
                fileType = "  c  ";
                printf("%-30s  %6s",entry->d_name,fileType);
                analizeCSourceFile(fp);
                cSourceFile++;
                break;
                //return 0;
            case C_PLUS_PLUS_SOURCE:
                fileType = " c++ ";
                printf("%-30s  %6s",entry->d_name,fileType);
                analizeCPlusPlusSourceFile(fp);
                cPlusPlusFile++;
                break;
            case SWIFT_FILE:
                fileType = "swift";
                printf("%-30s  %6s",entry->d_name,fileType);
                analizeSwiftSourceFile(fp);
                swiftFile++;
                break;
            case OC_SOURCE:
                fileType = " OC ";
                printf("%-30s  %6s",entry->d_name,fileType);
                analizeOCSourceFile(fp);
                ocSourceFile++;
                break;
            case HEADER_FILE:
                
                fileType = "header";
                printf("%-30s  %6s",entry->d_name,fileType);
                analizeHeaderSourceFile(fp);
                headerFile++;
            case OTHRES:
                others++;
                break;
            default:
                break;
        }
        fclose(fp);
    }
    endMenu();
    outputFolderResult(cSourceFile, cPlusPlusFile, ocSourceFile, headerFile, swiftFile, others);
    
    closedir(dir);
    endMenu();
    return 0;
}
