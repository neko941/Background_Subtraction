#include <stdio.h>

#include "./include/os.h"
#include "./include/utils.h"
#include "./include/print.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./include/stb_image_write.h"

unsigned char *backgroundSubtraction(char *background_path, char *foreground_path, char *new_background_path, double _theshold)
{
    int width_foreground, height_foreground, channel_foreground;
    int width_background, height_background, channel_background;
    int width_new_background, height_new_background, channel_new_background;
    // read images
    unsigned char *foreground = readImage(foreground_path, &width_foreground, &height_foreground, &channel_foreground, 0);
    unsigned char *background = readImage(background_path, &width_background, &height_background, &channel_background, 0);
    unsigned char *new_background = readImage(new_background_path, &width_new_background, &height_new_background, &channel_new_background, 0);

    // validation
    if (
        !(width_foreground == width_background && width_foreground == width_new_background) ||
        !(height_foreground == height_background && height_foreground == height_new_background) ||
        !(channel_foreground == channel_background && channel_foreground == channel_new_background))
    {
        error("Size of all input images must match");
    }

    makeDir("results");

    // fix varibles' names
    int height = height_foreground;
    int width = width_foreground;
    int channel = channel_foreground;
    int index;
    int diff;
    int t = clock();
    puts("******************** Background Subtraction");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            diff = 0;
            for (int k = 0; k < channel; k++)
            {
                index = i * width * channel + j * channel + k;
                diff += abs(foreground[index] - background[index]);
                progressingBar(index / (double)(height * width * channel - 1), ((double)clock() - t) / CLOCKS_PER_SEC);
            }
            if (diff < (_theshold * 3))
            {
                for (int k = 0; k < channel; k++)
                {
                    index = i * width * channel + j * channel + k;
                    foreground[index] = new_background[index];
                }
            }
        }
    }
    puts("");
    success("******************** Done successfully");
    stbi_write_png("results/result.png", width, height, channel, foreground, width * channel);
}

int main()
{
    char background[100];
    char foreground[100];
    char new_background[100];
    int _theshold = 45;

    while (1)
    {
        blue();
        printf("%c", 218);
        printTrailings(30, 196);
        printf("%c\n", 191);

        printf("%c              MENU\n", 179);
        printf("%c 0. Quit\n", 179);
        printf("%c 1. Test case 1 (300x218x3)\n", 179);
        printf("%c 2. Test case 2 (640x480x3)\n", 179);
        printf("%c 3. Test case 3 (964x699x3)\n", 179);
        printf("%c 4. Test case 4 (Fail)\n", 179);
        printf("%c 5. Custom input\n", 179);

        printf("%c", 192);
        printTrailings(30, 196);
        printf("%c\n", 217);

        printf(">> ");
        reset();
        int selection;
        scanf("%d", &selection);
        fflush(stdin);

        switch (selection)
        {
        case 0:
            printf("******************** Quiting app\n\n");
            exit(1);
            break;
        case 1:
            system("cls");
            printf("******************** Using test case 1\n\n");
            backgroundSubtraction("images/0_background.jpg",
                                  "images/0_foreground.jpg",
                                  "images/0_new_background.jpg",
                                  _theshold);
            break;
        case 2:
            system("cls");
            printf("******************** Using test case 2\n\n");
            backgroundSubtraction("images/1_background.png",
                                  "images/1_foreground.png",
                                  "images/1_new_background.png",
                                  _theshold);
            break;
        case 3:
            system("cls");
            printf("******************** Using test case 3\n\n");
            backgroundSubtraction("images/2_background.png",
                                  "images/2_foreground.png",
                                  "images/2_new_background.png",
                                  _theshold);
            break;
        case 4:
            system("cls");
            printf("******************** Using test case 4\n\n");
            backgroundSubtraction("images/1_background.png",
                                  "images/1_foreground.png",
                                  "images/98239648_p0.png",
                                  _theshold);
            break;
        case 5:
            printf("Background image     >> ");
            scanf("%s", &background);
            fflush(stdin);

            printf("Foreground image     >> ");
            scanf("%s", &foreground);
            fflush(stdin);

            printf("New background image >> ");
            scanf("%s", &new_background);
            fflush(stdin);

            system("cls");

            backgroundSubtraction(background,
                                  foreground,
                                  new_background,
                                  _theshold);
            break;

        default:
            printf("Invalid selection\n");
            break;
        }
    }
}