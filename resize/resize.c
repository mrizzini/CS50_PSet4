/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    int enlargeBy = atoi(argv[1]); // size we want to enlarge the file by

    if (enlargeBy < 1 || enlargeBy > 100) // error checking for size
    {
        fprintf(stderr, "enlargeBy must be a positive integer less than or equal to 100, %i\n", enlargeBy);
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) // error checking for error in file
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w"); // file we want to write to
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }


    // determine padding for scanlines
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // OLD PADDING
    int oldWidth = bi.biWidth; // OLD WIDTH (INPUT)
    int oldHeight = bi.biHeight; //OLD HEIGHT (INPUT)

    bi.biWidth *= enlargeBy; // will now be NEW WIDTH we want the picture to be
    bi.biHeight *= enlargeBy; // will now be NEW HEIGHT we want the picture to be
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // NEW PADDING, uses NEW WIDTH VALUE

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(bi.biHeight); // NEW VALUES
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // NEW VALUES

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // int offset = oldWidth * sizeof(RGBTRIPLE);

    // iterate over infile's scanlines (for each row) 1ST LOOP
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++) // loops thru OLD HEIGHT to scale to new size
    {

        for (int repeat = 0; repeat < enlargeBy ; repeat++)
        {

            for (int j = 0; j < oldWidth; j++) // uses OLD WIDTH to scale to new size
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //output pixels per input pixel in a line 4TH LOOP
                // write RGB triple to outfile

                for (int pix = 0; pix < enlargeBy; pix++) // writes to new file. uses new value to enlarge by
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    // fputc(0x00, outptr);
                }
            }

            // writes padding
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }

            if (repeat < enlargeBy - 1) // does this n - 1 times. fseek moves file cursor back to start of row. will not do this the last time thru the loop
            {
                fseek(inptr, -(oldWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }

        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);

    }

    // close infile at end of loop
    fclose(inptr);

    // close outfile at end of loop
    fclose(outptr);

    // success
    return 0;
}
