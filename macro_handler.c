#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "macro_handler.h"

#define MAX_MACROS 100
#define MAX_NAME 50
#define MAX_VALUE 200
#define MAX_LINE 500

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

static void replace_parameter(char *text,
                              const char *parameter,
                              const char *argument)
{
    char result[MAX_VALUE];
    char *pos;
    int length = 0;

    result[0] = '\0';

    while ((pos = strstr(text, parameter)) != NULL)
    {
        int prefix_length = pos - text;

        if (length + prefix_length + strlen(argument) >= MAX_VALUE - 1)
            return;

        strncat(result, text, prefix_length);
        strcat(result, argument);

        text = pos + strlen(parameter);
        length = strlen(result);
    }

    strcat(result, text);
    strcpy(text, result);
}

static void replace_function_macro(char *line, Macro *macro)
{
    char *pos;

    while ((pos = strstr(line, macro->name)) != NULL)
    {
        char *open;
        char *close;
        char argument[MAX_VALUE];
        char expanded[MAX_VALUE];
        char new_line[MAX_LINE];

        open = pos + strlen(macro->name);

        if (*open != '(')
            break;

        close = strchr(open, ')');

        if (close == NULL)
            break;

        strncpy(argument, open + 1, close - open - 1);
        argument[close - open - 1] = '\0';

        strcpy(expanded, macro->value);

        replace_parameter(expanded,
                          macro->parameter,
                          argument);

        snprintf(new_line,
                 sizeof(new_line),
                 "%.*s%s%s",
                 (int)(pos - line),
                 line,
                 expanded,
                 close + 1);

        strcpy(line, new_line);
    }
}

static void replace_simple_macro(char *line, Macro *macro)
{
    char *pos;

    while ((pos = strstr(line, macro->name)) != NULL)
    {
        char before;
        char after;
        char new_line[MAX_LINE];

        before = (pos == line) ? ' ' : *(pos - 1);
        after = *(pos + strlen(macro->name));

        if (!isalnum((unsigned char)before) &&
            before != '_' &&
            !isalnum((unsigned char)after) &&
            after != '_')
        {
            snprintf(new_line,
                     sizeof(new_line),
                     "%.*s%s%s",
                     (int)(pos - line),
                     line,
                     macro->value,
                     pos + strlen(macro->name));

            strcpy(line, new_line);
        }
        else
        {
            pos += strlen(macro->name);
        }
    }
}

static void replace_macros(char *line)
{
    int i;

    for (i = 0; i < macro_count; i++)
    {
        if (macros[i].is_function)
        {
            replace_function_macro(line, &macros[i]);
        }
        else
        {
            replace_simple_macro(line, &macros[i]);
        }
    }
}

void process_macros(const char *input_file, const char *output_file)
{
    FILE *fp_in;
    FILE *fp_out;
    char line[MAX_LINE];

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
            replace_macros(line);
            fputs(line, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);
}
