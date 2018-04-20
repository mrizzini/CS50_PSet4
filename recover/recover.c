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

    char *infile = argv[1]; // takes second argument and stores it as infile
    FILE *file = fopen(infile, "r"); // opens up infile

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file.\n"); // error checking
        return 2;
    }

    unsigned char buffer[512]; // declares enough room for each 512 byte of memory
    char filename[49]; // declares enough room for file names
    int fileNum = 0; // counter variable to keep track of file number for jpgs i.e. 001, 002, 003
    FILE *img = NULL; // this will be used as a check for if we are already in a jpeg

    while (fread(buffer, 512, 1, file) == 1) // REPEATS TILL END OF FILE. this reads 512 bytes into the buffer
    {

        // ARE WE AT START OF A NEW JPEG?
        // YES
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL) // IF WE HAVE ALREADY FOUND A JPEG
            {
                fclose(img); // CLOSE PREVIOUS FILE
                fileNum++; // increment so we can name the next file correctly
            }
            // the first time the if (buffer[0]) statement hits, this NULL if statement wont hit. the NULL statement should
            // hit every time after that

            // THEN OPEN NEW FILE. previous file will be closed by this point
            sprintf(filename, "%03i.jpg", fileNum); // this line names the new file using our fileNum counter
            img = fopen(filename, "w"); // opens up new file
            fwrite(buffer, 512, 1, img); // writes into img file from buffer
        }

        // ARE WE AT START OF A NEW JPEG?
        // NO
        else
        {
            // IF WE HAVE NOT REACHED START OF A NEW JPEG
            // HAVE WE ALREADY FOUND A JPEG OR NOT?
            if (img != NULL) // IF WE HAVE ALREADY FOUND A JPEG.
            {
                fwrite(buffer, 512, 1, img); // THOSE LAST 512 BYTES BELONG TO OPEN FILE. we continue writing to open file
            }
        }

    }
    // AT END OF LOOP, CLOSE ANY OPEN FILE
    fclose(img);

}

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