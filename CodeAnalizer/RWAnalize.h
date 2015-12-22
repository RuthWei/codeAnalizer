//
//  RWAnalize.h
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/19.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __CodeAnalizer__RWAnalize__
#define __CodeAnalizer__RWAnalize__

#include <stdio.h>
#include <stdlib.h>
#include "RWMenu.h"
FILE_TYPE_E getFileType(const char * path);
void initGlobalVariables();
void analizeCSourceFile(FILE * fp);
void analizeCPlusPlusSourceFile(FILE * fp);
void analizeOCSourceFile(FILE * fp);
void analizeHeaderSourceFile(FILE * fp);
void analizeSwiftSourceFile(FILE * fp);
#endif /* defined(__CodeAnalizer__RWAnalize__) */
