#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isJpeg(unsigned char byteCheck[]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("./recover image\n");
        return 1;
    }
    else
    {
        int noOfFiles = 0;
        bool prevJpeg = false;
        FILE *file = fopen(argv[1], "r");
        FILE *img = NULL;

        unsigned char checkBytes[512];
        char imageName[8];

        if (!file)
        {
            return 1;
        }

        while (fread(checkBytes, 512, 1, file)) // Store first 512 bytes into checkBytes
        {
            if (isJpeg(checkBytes))
            {
                if (!prevJpeg)
                {
                    prevJpeg = true;
                }
                else
                {
                    fclose(img);
                }

                sprintf(imageName, "%03i.jpg", noOfFiles); // Set imageName for img file later.
                img = fopen(imageName, "w"); // Create a file and store to img pointer variable
                fwrite(checkBytes, 512, 1, img);

                noOfFiles++;
            }
            else if (prevJpeg) // If still same file, keep writing to file.
            {
                fwrite(checkBytes, 512, 1, img);
            }
        }

        fclose(file);
        fclose(img);
        return 0;
    }
}

bool isJpeg(unsigned char byteCheck[])
{
    if (byteCheck[0] == 0xff && byteCheck[1] == 0xd8 && byteCheck[2] == 0xff && (byteCheck[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}
