#ifndef __PRINT_H__
#define __PRINT_H__

#include "./color.h"

void printTrailings(int len, int character)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c", character);
    }
}

void progressingBar(double percentage, double seconds)
{
    char const *progressingBarString = "============================================================";
    int const progressingBarWidth = 60;
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * progressingBarWidth);

    if (val == 100)
    {
        purple();
    }

    printf("\r%3d%% [%.*s%*s] %.3fs", val, lpad, progressingBarString, progressingBarWidth - lpad, "", seconds);
    fflush(stdout);
    reset();
}

void error(char *message)
{
    red();
    puts(message);
    reset();
    exit(1);
}

void success(char *message)
{
    green();
    puts(message);
    reset();
    puts("");
}

#endif