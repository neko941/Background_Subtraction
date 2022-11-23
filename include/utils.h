#ifndef __UTILS_H__
#define __UTILS_H__

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#include "./color.h"
#include "./print.h"

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

char *c_arrayNew_1d(int _size)
{
    return (char *)calloc(_size, sizeof(char));
}

// ==============================================================================================================

unsigned char *uc_arraysAbsoluteDifference_1d(const unsigned char *arr1, const unsigned char *arr2, int _size)
{
    int t = clock();
    puts("******************** Subtracting 2 arrays");
    unsigned char *diff = uc_arrayNew_1d(_size);
    for (int i = 0; i < _size; i++)
    {
        diff[i] = ((arr1[i] - arr2[i]) < 0) ? (arr2[i] - arr1[i]) : (arr1[i] - arr2[i]);
        progressingBar(i / (double)(_size - 1), ((double)clock() - t) / CLOCKS_PER_SEC);
    }
    puts("");
    success("******************** Done Subtracting");

    return diff;
}

// ==============================================================================================================

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
    int t = clock();
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
            progressingBar((i * width + j) / (double)(height * width - 1), ((double)clock() - t) / CLOCKS_PER_SEC);
        }
    }
    puts("");
    success("******************** Converting to grayscale successfully");

    return gray_image;
}

// ==============================================================================================================

double threshold(double number, double threshold, double min, double max)
{
    return number < threshold ? min : max;
}

int lengthOfInt(int value)
{
    // char str[ENOUGH];
    // sprintf(str, "%d", value);
    // return strlen(str);
    return (int)((ceil(log10(value)) + 1) * sizeof(char)) - 1;
}

// ==============================================================================================================

unsigned char *binaryMaskByGreyscale(unsigned char *greyscale, int _size, int _threshold, int min, int max)
{
    int t = clock();
    puts("******************** Generating binary mask using greyscale image");
    unsigned char *mask = uc_arrayNew_1d(_size);
    for (int i = 0; i < _size; i++)
    {
        mask[i] = threshold(greyscale[i], _threshold, min, max);
        progressingBar(i / (double)(_size - 1), ((double)clock() - t) / CLOCKS_PER_SEC);
    }
    puts("");
    success("******************** Generating binary mask successfully");

    return mask;
}

// ==============================================================================================================

unsigned char *mergeImages(unsigned char *foreground, unsigned char *new_background, unsigned char *binaryMask, int width, int height, int channel)
{
    puts("******************** Merging new background to foreground");
    int t = clock();
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
                progressingBar(index / (double)(height * width * channel - 1), ((double)clock() - t) / CLOCKS_PER_SEC);
            }
        }
    }
    puts("");
    success("******************** Merging successfully");
    return new_image;
}

// ==============================================================================================================

unsigned char *readImage(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    unsigned char *image = stbi_load(filename, x, y, comp, req_comp);
    if (image == NULL)
    {
        error("Error in loading the image");
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