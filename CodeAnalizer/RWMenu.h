//
//  RWMenu.h
//  CodeAnalizer
//
//  Created by qianfeng on 15/12/19.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __CodeAnalizer__RWMenu__
#define __CodeAnalizer__RWMenu__

#include <stdio.h>

typedef enum fileType
{
    HEADER_FILE,
    C_LANGUAGE_SOURCE,
    C_PLUS_PLUS_SOURCE,
    OC_SOURCE,
    SWIFT_FILE,
    OTHRES,
}FILE_TYPE_E;
void endMenu();
void startMenu();
#endif /* defined(__CodeAnalizer__RWMenu__) */
