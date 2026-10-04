#include <stdio.h>
#include "comment_removal.h"

void remove_comments(const char *input_file, const char *output_file)
{
    FILE *fp_in;
    FILE *fp_out;
    int ch;
    int next_ch;

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

    while ((ch = fgetc(fp_in)) != EOF)
    {
        if (ch == '/')
        {
            next_ch = fgetc(fp_in);

            if (next_ch == '/')
            {
                while ((ch = fgetc(fp_in)) != EOF && ch != '\n')
                    ;

                if (ch == '\n')
                    fputc('\n', fp_out);
            }
            else if (next_ch == '*')
            {
                while ((ch = fgetc(fp_in)) != EOF)
                {
                    if (ch == '*')
                    {
                        next_ch = fgetc(fp_in);

                        if (next_ch == '/')
                            break;
                    }
                }
            }
            else
            {
                fputc('/', fp_out);

                if (next_ch != EOF)
                    fputc(next_ch, fp_out);
            }
        }
        else
        {
            fputc(ch, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);
}
