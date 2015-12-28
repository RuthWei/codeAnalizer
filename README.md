# codeAnalizer
code Analizer can statistic comment line numbers, blank line numbers, effective code line number and function numbers in about C, C++, OC, swift source code file.
This software is code analyzer. It only can analyze the program can be compiled. We need input directory of program, and it will show as file name , file type, total effective code lines, blank lines and comment lines. Blank line include no words except space, ‘\t’, ‘\r’, ‘\n’; Comment line begins with “//”or between “/*”and “*/”. The rest one is effective code. If there are effective code in previous and comment code in the end of one line. Such code is classified into effective ones. 
只分析能编译过的程序
空行：除了空格、tab、\n再没有其它字符
注释：以//、/*开头的一定是注释；在/**/包含范围内的一定是注释。
有效行：除了空行和注释行，剩下的，全部是有效行。如果一行中，前一部分是代码，后一部分是注释，这样，算有效行
