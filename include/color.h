#ifndef __GENERAL_COLOR_H__
#define __GENERAL_COLOR_H__

#include <stdio.h>
#include <stdlib.h>

void black()
{
    printf("\033[0;30m");
}

void red()
{
    printf("\033[1;31m");
}

void green()
{
    printf("\033[0;32m");
}

void yellow()
{
    printf("\033[1;33m");
}

void blue()
{
    printf("\033[0;34m");
}

void purple()
{
    printf("\033[0;35m");
}

void cyan()
{
    printf("\033[0;36m");
}

void white()
{
    printf("\033[0;37m");
}

void reset()
{
    printf("\033[0m");
}

#endif