#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    printf("file is %s\n", argv[1]);
    // FILE *file = fopen(argv[1], "r");
}