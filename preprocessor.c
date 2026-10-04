#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"
#include "comment_removal.h"
#include "file_inclusion.h"
#include "macro_handler.h"

void preprocess(const char *input_file, const char *output_file)
{
    char temp_file1[] = "temp_comments.c";
    char temp_file2[] = "temp_includes.c";

    remove_comments(input_file, temp_file1);

    process_includes(temp_file1, temp_file2);

    process_macros(temp_file2, output_file);

    remove(temp_file1);
    remove(temp_file2);

    printf("Preprocessing completed successfully.\n");
    printf("Output file: %s\n", output_file);
}
