#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "macro_handler.h"

#define MAX_MACROS 100
#define MAX_NAME 50
#define MAX_VALUE 200

typedef struct
{
    char name[MAX_NAME];
    char value[MAX_VALUE];
    int is_function;
    char parameter[MAX_NAME];
} Macro;

static Macro macros[MAX_MACROS];
static int macro_count = 0;

static void store_macro(char *line)
{
    char *ptr;
    char *space;
    char *open_bracket;
    char *close_bracket;

    ptr = line + 7;

    while (isspace((unsigned char)*ptr))
        ptr++;

    open_bracket = strchr(ptr, '(');
    space = strchr(ptr, ' ');

    if (open_bracket != NULL &&
        (space == NULL || open_bracket < space))
    {
        close_bracket = strchr(open_bracket, ')');

        if (close_bracket == NULL)
            return;

        macros[macro_count].is_function = 1;

        strncpy(macros[macro_count].name,
                ptr,
                open_bracket - ptr);

        macros[macro_count].name[open_bracket - ptr] = '\0';

        strncpy(macros[macro_count].parameter,
                open_bracket + 1,
                close_bracket - open_bracket - 1);

        macros[macro_count].parameter[
            close_bracket - open_bracket - 1] = '\0';

        ptr = close_bracket + 1;

        while (isspace((unsigned char)*ptr))
            ptr++;

        strcpy(macros[macro_count].value, ptr);
    }
    else
    {
        macros[macro_count].is_function = 0;

        if (space == NULL)
            return;

        strncpy(macros[macro_count].name,
                ptr,
                space - ptr);

        macros[macro_count].name[space - ptr] = '\0';

        ptr = space + 1;

        while (isspace((unsigned char)*ptr))
            ptr++;

        strcpy(macros[macro_count].value, ptr);
    }

    macro_count++;
}

static void replace_text(char *line)
{
    int i;

    for (i = 0; i < macro_count; i++)
    {
        if (macros[i].is_function)
        {
            char *pos = strstr(line, macros[i].name);

            while (pos != NULL)
            {
                char *open = pos + strlen(macros[i].name);

                if (*open != '(')
                    break;

                char *close = strchr(open, ')');

                if (close == NULL)
                    break;

                char argument[MAX_VALUE];
                char result[500];

                int arg_length = close - open - 1;

                strncpy(argument, open + 1, arg_length);
                argument[arg_length] = '\0';

                snprintf(result,
                         sizeof(result),
                         "%.*s%s%.*s%s%s",
                         (int)(pos - line),
                         line,
                         macros[i].value,
                         0,
                         "",
                         "",
                         "");

                char expanded[MAX_VALUE];

                strcpy(expanded, macros[i].value);

                char *param_pos =
                    strstr(expanded, macros[i].parameter);

                if (param_pos != NULL)
                {
                    char temp[MAX_VALUE];

                    snprintf(temp,
                             sizeof(temp),
                             "%.*s%s%s",
                             (int)(param_pos - expanded),
                             expanded,
                             argument,
                             param_pos + strlen(macros[i].parameter));

                    strcpy(expanded, temp);
                }

                char new_line[500];

                snprintf(new_line,
                         sizeof(new_line),
                         "%.*s%s%s",
                         (int)(pos - line),
                         line,
                         expanded,
                         close + 1);

                strcpy(line, new_line);

                pos = strstr(line, macros[i].name);
            }
        }
        else
        {
            char *pos = strstr(line, macros[i].name);

            while (pos != NULL)
            {
                char before;
                char after;

                before = (pos == line) ? ' ' : *(pos - 1);
                after = *(pos + strlen(macros[i].name));

                if (!isalnum((unsigned char)before) &&
                    before != '_' &&
                    !isalnum((unsigned char)after) &&
                    after != '_')
                {
                    char new_line[500];

                    snprintf(new_line,
                             sizeof(new_line),
                             "%.*s%s%s",
                             (int)(pos - line),
                             line,
                             macros[i].value,
                             pos + strlen(macros[i].name));

                    strcpy(line, new_line);
                }
                else
                {
                    pos += strlen(macros[i].name);
                }

                pos = strstr(pos, macros[i].name);
            }
        }
    }
}

void process_macros(const char *input_file, const char *output_file)
{
    FILE *fp_in;
    FILE *fp_out;
    char line[500];

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
        if (strncmp(line, "#define", 7) == 0)
        {
            store_macro(line);
        }
        else
        {
            replace_text(line);
            fputs(line, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);
}
