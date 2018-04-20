#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // char *infile = argv[1];
    // printf("file is %s\n", argv[1]);

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file.\n");
        return 2;
    }

    unsigned char buffer[1000];
    // int counter = 0;
    int jpeg = 0;
    char filename[50];
    int test = 1;
    // bool foundJPEG;
    FILE *img = NULL;


    while(fread(buffer, 512, 1, file) == 1) // REPEATS TILL END OF FILE?
    {
        fread(buffer, 512, 1, file); // READ FIRST 512 BYTES

        // ARE WE AT START OF A NEW JPEG?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            if (img != NULL) // IF WE HAVE ALREADY FOUND A JPEG
            {
                fclose(img); // CLOSE PREVIOUS FILE
            }

            // THEN OPEN NEW FILE
            printf("found it\n");
            sprintf(filename, "%03i.jpg", test);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
            // foundJPEG = true;
            jpeg++;
            test++;
            // filename++;
        }


        // IF WE HAVE NOT REACHED START OF A NEW JPEG
        // HAVE WE ALREADY FOUND A JPEG OR NOT

        if (img != NULL) // IF WE HAVE ALREADY FOUND A JPEG
        {
            fwrite(buffer, 512, 1, img); // THOSE LAST 512 BYTES BELONG TO OPEN FILE
        }


            // counter++;

    }
    fclose(img);
    printf("jpeg count is %i\n", jpeg);


    // if (buffer[0] == "0xff" && buffer[1] == "0xd8" && buffer[2] == "0xff" && (buffer[3] & "0xf0") == "0xe0")
    // {

    // }


    // sprintf(filename, "%03i.jpg", 2);

    // FILE *img = fopen(filename, "w");


}



//     char buffer[50];

// int course = 50;

// sprintf(buffer,"CS%d rocks!", course);

// printf("%s\n", buffer);

// open card file
// repear until end of card
//      read 512 bytes into a buffer
//      start of a new JPEG?
//          yes..
//          no...
//      already found a JPEG?
//          no...
//          yes..
//  close any remaining files