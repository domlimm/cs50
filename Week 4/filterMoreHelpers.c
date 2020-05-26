#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float sum = 0;
    int average = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            average = (int) round(sum / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
        sum = 0;
        average = 0;
    }

    return;
}

void swapReflect(RGBTRIPLE *first, RGBTRIPLE *second)
{
    RGBTRIPLE temp1;
    RGBTRIPLE temp2;

    temp1 = *first;
    temp2 = *second;
    *first = temp2;
    *second = temp1;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int swapIndex = -1;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swapIndex = width - 1 - j;

            // If odd, no swap required.
            if (j == swapIndex)
            {
                continue;
            }

            // first and second pointers will point to address in memory of given address.
            swapReflect(&image[i][j], &image[i][swapIndex]);
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int totalPixels = 0;
    float totalB = 0, totalG = 0, totalR = 0;
    RGBTRIPLE temp[height][width];

    // Make a temporary copy to reference values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int a = -1; a < 2; a++) // Rows
            {
                for (int b = -1; b < 2; b++) // Columns
                {
                    bool passedRow = i + a >= 0, passedCol = j + b >= 0, withinHeight = i + a <= height - 1, withinWidth = j + b <= width - 1;

                    // -1 , 0 , 1
                    if (passedRow && passedCol && withinHeight && withinWidth)
                    {
                        totalB += temp[i + a][j + b].rgbtBlue;
                        totalG += temp[i + a][j + b].rgbtGreen;
                        totalR += temp[i + a][j + b].rgbtRed;
                        totalPixels++;
                    }
                }
            }

            // Change original pixel here
            image[i][j].rgbtBlue = (int) round(totalB / totalPixels);
            image[i][j].rgbtGreen = (int) round(totalG / totalPixels);
            image[i][j].rgbtRed = (int) round(totalR / totalPixels);
            totalPixels = 0, totalB = 0, totalG = 0, totalR = 0;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int totalPixel = 0, maxTop = 0, maxBottom = 0, maxLeft = 0, maxRight = 0;

    int gxVals[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gyVals[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    int gxB = 0, gxG = 0, gxR = 0, gyB = 0, gyG = 0, gyR = 0;
    RGBTRIPLE temp[height][width];

    // Make a temporary copy to reference values
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            temp[i][j] = image[i][j];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // bool isCorner = (i == 0 && j == 0) || (i == height - 1 && j == width - 1) || (i == 0 && j == width - 1) || (i == height - 1 && j == 0);

            for (int a = -1; a < 2; a++) // Rows
            {
                for (int b = -1; b < 2; b++) // Columns
                {
                    // If not passedRow, pixel is in first row.
                    // If not passedColumn, pixel is in first column.
                    // If not withinHeight, pixel is in last row.
                    // If not withinWidth, pixel is in last column.

                    // Gx      Gy
                    // -1 0 1  -1 -2 -1
                    // -2 0 2   0  0  0
                    // -1 0 1   1  2  1
                    // Formula : (Gx of R/G/B ^ 2 + Gy of R/G/B ^ 2)sqrt = Pixel R/G/B

                    bool passedRow = i + a >= 0, passedCol = j + b >= 0, withinHeight = i + a < height, withinWidth = j + b < width;

                    // If pixel is located within height and width and not out of bounds
                    if (passedRow && passedCol && withinHeight && withinWidth)
                    {
                        int gxVal = gxVals[a + 1][b + 1];
                        gxB += temp[i + a][j + b].rgbtBlue * gxVal;
                        gxG += temp[i + a][j + b].rgbtGreen * gxVal;
                        gxR += temp[i + a][j + b].rgbtRed * gxVal;

                        int gyVal = gyVals[a + 1][b + 1];
                        gyB += temp[i + a][j + b].rgbtBlue * gyVal;
                        gyG += temp[i + a][j + b].rgbtGreen * gyVal;
                        gyR += temp[i + a][j + b].rgbtRed * gyVal;
                    }
                }
            }

            // Change original pixel here
            int b = round(sqrt((gxB * gxB) + (gyB * gyB))) > 255 ? 255 : round(sqrt((gxB * gxB) + (gyB * gyB)));
            int g = round(sqrt((gxG * gxG) + (gyG * gyG))) > 255 ? 255 : round(sqrt((gxG * gxG) + (gyG * gyG)));
            int r = round(sqrt((gxR * gxR) + (gyR * gyR))) > 255 ? 255 : round(sqrt((gxR * gxR) + (gyR * gyR)));

            image[i][j].rgbtBlue = b;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtRed = r;

            gxB = 0, gxG = 0, gxR = 0, gyB = 0, gyG = 0, gyR = 0;
        }
    }

    return;
}

