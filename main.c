#include <stdio.h>
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./mypreprocessor inputfile.c\n");
        return 1;
    }
    printf("Input file: %s\n", argv[1]);
    return 0;
}
