#ifndef __UTILS_H__
#define __UTILS_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // check is dir exist
#include <direct.h>   // mkdir

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

void red()
{
    printf("\033[1;31m");
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

void green()
{
    printf("\033[0;32m");
}

void reset()
{
    printf("\033[0m");
}

void progressingBar(double percentage)
{
    char const *progressingBarString = "============================================================";
    int const progressingBarWidth = 60;
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * progressingBarWidth);

    if (val == 100)
        purple();

    printf("\r%3d%% [%.*s%*s]", val, lpad, progressingBarString, progressingBarWidth - lpad, "");
    fflush(stdout);
    reset();
}

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

char *c_arrayNew_1d(int _size)
{
    return (char *)calloc(_size, sizeof(char));
}

unsigned char *uc_arraysAbsoluteDifference_1d(const unsigned char *arr1, const unsigned char *arr2, int _size)
{
    puts("******************** Subtracting 2 arrays");
    unsigned char *diff = uc_arrayNew_1d(_size);
    for (int i = 0; i < _size; i++)
    {
        diff[i] = ((arr1[i] - arr2[i]) < 0) ? (arr2[i] - arr1[i]) : (arr1[i] - arr2[i]);
        progressingBar(i / (double)(_size - 1));
    }
    green();
    printf("\n******************** Done Subtracting\n\n");
    reset();
    return diff;
}

double sRGB_to_linear(double x)
{
    if (x < 0.04045)
        return x / 12.92;
    return pow((x + 0.055) / 1.055, 2.4);
}

double linear_to_sRGB(double y)
{
    if (y <= 0.0031308)
        return 12.92 * y;
    return 1.055 * pow(y, 1 / 2.4) - 0.055;
}

unsigned char *uc_grayscaleGammaCompression_1d(unsigned char *color_image, int width, int height, int channel)
{
    puts("******************** Converting to grayscale using Gamma Compression");
    unsigned char *gray_image = uc_arrayNew_1d(width * height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double gray_linear = 0.2126 * sRGB_to_linear(color_image[i * width * channel + j * channel + 0] / 255.0) +
                                 0.7152 * sRGB_to_linear(color_image[i * width * channel + j * channel + 1] / 255.0) +
                                 0.0722 * sRGB_to_linear(color_image[i * width * channel + j * channel + 2] / 255.0);
            gray_image[i * width + j] = (unsigned char)round(linear_to_sRGB(gray_linear) * 255);
            progressingBar((i * width + j) / (double)(height * width - 1));
        }
    }
    green();
    printf("\n******************** Converting to grayscale successfully\n\n");
    reset();

    return gray_image;
}

double threshold(double number, double threshold, double min, double max)
{
    return number < threshold ? min : max;
}

void printTrailings(int len, int character)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c", character);
    }
}

int lengthOfInt(int value)
{
    // char str[ENOUGH];
    // sprintf(str, "%d", value);
    // return strlen(str);
    return (int)((ceil(log10(value)) + 1) * sizeof(char)) - 1;
}

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

unsigned char *binaryMaskByGreyscale(unsigned char *greyscale, int _size, int _threshold, int min, int max)
{
    puts("******************** Generating binary mask using greyscale image");
    unsigned char *mask = uc_arrayNew_1d(_size);
    for (int i = 0; i < _size; i++)
    {
        mask[i] = threshold(greyscale[i], _threshold, min, max);
        progressingBar(i / (double)(_size - 1));
    }
    green();
    printf("\n******************** Generating binary mask successfully\n\n");
    reset();
    return mask;
}

unsigned char *mergeImages(unsigned char *foreground, unsigned char *new_background, unsigned char *binaryMask, int width, int height, int channel)
{
    puts("******************** Merging new background to foreground");

    int index;
    unsigned char *new_image = uc_arrayNew_1d(width * height * channel);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channel; k++)
            {
                index = i * width * channel + j * channel + k;
                new_image[index] = (binaryMask[i * width + j] == 0) ? new_background[index] : foreground[index];
                progressingBar(index / (double)(height * width * channel - 1));
            }
        }
    }
    green();
    printf("\n******************** Merging successfully\n\n");
    reset();
    return new_image;
}

void makeDir(char const *path)
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

unsigned char *readImage(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    unsigned char *image = stbi_load(filename, x, y, comp, req_comp);
    if (image == NULL)
    {
        red();
        puts("Error in loading the image");
        reset();
        exit(1);
    }
    else
    {
        green();
        puts("******************** Reading image successfully");
        int length_path = 12 + strlen(filename) + 1;
        char *result_path = c_arrayNew_1d(length_path);
        sprintf(result_path, "Image Path: %s", filename);

        int length_width = 7 + lengthOfInt(*x) + 1;
        char *result_width = c_arrayNew_1d(length_width);
        sprintf(result_width, "Width: %d", *x);

        int length_height = 8 + lengthOfInt(*y) + 1;
        char *result_height = c_arrayNew_1d(length_height);
        sprintf(result_height, "Height: %d", *y);

        int length_channel = 9 + lengthOfInt(*comp) + 1;
        char *result_channel = c_arrayNew_1d(length_channel);
        sprintf(result_channel, "Channel: %d", *comp);

        cyan();
        printf("%c", 218);
        printTrailings(strlen(result_path) + 2, 196);
        printf("%c\n", 191);

        printf("%c %s %c\n", 179, result_path, 179);
        printf("%c %s", 179, result_width);
        printTrailings(strlen(result_path) - strlen(result_width) + 1, 32);
        printf("%c\n", 179);

        printf("%c %s", 179, result_height);
        printTrailings(strlen(result_path) - strlen(result_height) + 1, 32);
        printf("%c\n", 179);

        printf("%c %s", 179, result_channel);
        printTrailings(strlen(result_path) - strlen(result_channel) + 1, 32);
        printf("%c\n", 179);

        printf("%c", 192);
        printTrailings(strlen(result_path) + 2, 196);
        printf("%c\n\n", 217);

        reset();
        return image;
    }
}

#endif