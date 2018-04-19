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
    int n = atoi(argv[1]);

    if (n < 1 || n > 100)
    {
        fprintf(stderr, "N must be a positive integer less than or equal to 100, %i\n", n);
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
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
    int oldHeight = bi.biHeight;

    bi.biWidth *= n; // will now be NEW WIDTH
    bi.biHeight *= n; // will now be NEW HEIGHT
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // NEW PADDING, uses NEW WIDTH VALUE

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(bi.biHeight); // NEW VALUES
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // NEW VALUES

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // int offset = oldWidth * sizeof(RGBTRIPLE);
    // iterate over infile's scanlines (for each row) 1ST LOOP
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++) // uses NEW HEIGHTS
    {

        for (int repeat = 0; repeat < n ; repeat++)
        {


            for (int j = 0; j < oldWidth; j++) // uses
            {

                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    //output pixels per input pixel in a line 4TH LOOP
                    // write RGB triple to outfile

                    for (int pix = 0; pix < n; pix++)
                    {

                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    // fputc(0x00, outptr);
                    }
            }



            // then add it back (to demonstrate how)
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }

            if (repeat < n - 1)
            {
            // fseek(inptr, -(oldWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            // fseek(inptr, -offset, SEEK_CUR);
            fseek(inptr, -(oldWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }



        }

               // skip over padding, if any
            fseek(inptr, oldPadding, SEEK_CUR);

        // fseek(inptr, padding, SEEK_CUR);

    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
