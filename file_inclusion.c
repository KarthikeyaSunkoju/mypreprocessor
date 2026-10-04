#include <stdio.h>
#include <string.h>
#include "file_inclusion.h"

void process_includes(const char *input_file, const char *output_file)
{
    FILE *fp_in;
    FILE *fp_out;
    FILE *fp_include;
    char line[500];
    char filename[100];

    fp_in = fopen(input_file, "r");

    if (fp_in == NULL)
    {
        printf("Error: Cannot open input file.\n");
        return;
    }

    fp_out = fopen(output_file, "w");

    if (fp_out == NULL)
    {
        printf("Error: Cannot create output file.\n");
        fclose(fp_in);
        return;
    }

    while (fgets(line, sizeof(line), fp_in) != NULL)
    {
        if (sscanf(line, "#include <%99[^>]>", filename) == 1)
        {
            fp_include = fopen(filename, "r");

            if (fp_include == NULL)
            {
                printf("Error: Cannot open included file %s\n", filename);
                fclose(fp_in);
                fclose(fp_out);
                return;
            }

            while (fgets(line, sizeof(line), fp_include) != NULL)
            {
                fputs(line, fp_out);
            }

            fclose(fp_include);
        }
        else
        {
            fputs(line, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);
}
