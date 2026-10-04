#include <stdio.h>
#include <string.h>
#include "preprocessor.h"

int main(int argc, char *argv[])
{
    char output_file[100];
    char input_file[100];
    char *dot;

    if (argc != 2)
    {
        printf("Usage: ./mypreprocessor inputfile.c\n");
        return 1;
    }

    strcpy(input_file, argv[1]);

    dot = strrchr(input_file, '.');

    if (dot != NULL)
    {
        *dot = '\0';
    }

    snprintf(output_file, sizeof(output_file), "%s.i", input_file);

    preprocess(argv[1], output_file);

    return 0;
}
