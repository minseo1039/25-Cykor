#include <stdio.h>
#include "color.h"

void print_color(const char *color, const char *text) {
    printf("%s%s%s", color, text, RESET);
}