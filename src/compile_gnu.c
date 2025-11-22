#include "header/convertor.h"
#include <stdio.h>
#include <stdlib.h>

void compile_gnu(const char* arguments)
{
    char buf[512];

    /*
      пока нет проверки на то что компилятор
      может быть не задан или может быть пустой
    */
    
    snprintf(buf, sizeof(buf), "\"%s\" %s", global.gnu_compiler, arguments);
    log(DebugLogger, "Execute: %s", buf);

    system(buf);
}