#ifndef __OS_H__
#define __OS_H__

#include <stdio.h>
#include <direct.h>   // mkdir
#include <sys/stat.h> // check is dir exist

#include "./color.h"

int pathExists(char const *path)
{
    struct stat sb;
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        printf("******************** Path exists: %s\n", path);
        puts("");
        return 1;
    }
    else
    {
        printf("******************** Path not exists: %s\n", path);
        return 0;
    }
}

void makeDir(char const *path)
{
    if (!pathExists(path))
    {
        if (!mkdir(path))
        {
            green();
            printf("******************** Directory created: %s\n\n", path);
            reset();
        }
        else
        {
            red();
            printf("Unable to create directory: %s\n", path);
            reset();
            exit(1);
        }
    }
}

#endif