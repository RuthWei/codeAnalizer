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
 判断一个Ｃ语言源文件的行数
 总行数＝空行行数＋注释行数＋有效行数
 如果前面半句是有效行，后面有注释，则算做是有效行
 */
#define PATH "/Users/qianfeng/Documents/day02/赵晓东develop_project/gps/function.c"
#define DIRPATH "/Users/qianfeng/Documents/day02/gprs_project_zhouchaofeng"//"/Users/qianfeng/Documents/day02/郧亚锋good_ftp/client"//"/Users/qianfeng/Documents/day02/赵晓东develop_project/gps"

void delay()
{
    for (int i = 0; i < 500000; i++) {
        i++;
        i--;
    }
}
int main(int argc, const char * argv[]) {
    // insert code here...

    char buf[200] = {};
    DIR * dir = NULL;
    struct dirent * entry;
    
    //打开目录，
    dir = opendir(DIRPATH);
    if (!dir) {
        perror("open dir error");
        return 1;
    }
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
        strcat(buf, entry->d_name);
        //printf("%s\n",buf);
        delay();
        //打开文件进行分析
        FILE * fp = fopen(buf, "r");
        //新打开文件，初始化全局变量
        printf("%s\t\tinit\n",entry->d_name);
        initGlobalVariables();
        switch (getFileType(buf)) {
            case C_LANGUAGE_SOURCE:
                printf("%s\t\t\n",entry->d_name);
                analizeCSourceFile(fp);
                break;
                //return 0;
            case C_PLUS_PLUS_SOURCE:
                break;
            case SWIFT_FILE:
                break;
            case OC_SOURCE:
                break;
            case HERDER_FILE:
            case OTHRES:
                break;
            default:
                break;
        }
  
        fclose(fp);
    }
    closedir(dir);
    return 0;
}
