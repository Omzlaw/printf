#include "main.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

int _printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = 0;
    char buffer[1024];
    int bufferIndex = 0;
    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == 'c') {
                int c = va_arg(args, int);
                buffer[bufferIndex++] = (char)c;
            } else if (*format == 's') {
                char *s = va_arg(args, char *);
                int len = strlen(s);
                memcpy(&buffer[bufferIndex], s, len);
                bufferIndex += len;
            } else if (*format == 'd' || *format == 'i') {
                int d = va_arg(args, int);
                bufferIndex += snprintf(&buffer[bufferIndex], sizeof(buffer) - bufferIndex, "%d", d);
            } else if (*format == 'b') {
                unsigned int b = va_arg(args, unsigned int);
                char binary[32];
                int i = 0;
                while (b > 0) {
                    binary[i++] = (b % 2) + '0';
                    b /= 2;
                }
                binary[i] = '\0';
                strrev(binary);
                int len = strlen(binary);
                memcpy(&buffer[bufferIndex], binary, len);
                bufferIndex += len;
            } else if (*format == 'u') {
                unsigned int u = va_arg(args, unsigned int);
                bufferIndex += snprintf(&buffer[bufferIndex], sizeof(buffer) - bufferIndex, "%u", u);
            } else if (*format == 'o') {
                unsigned int o = va_arg(args, unsigned int);
                bufferIndex += snprintf(&buffer[bufferIndex], sizeof(buffer) - bufferIndex, "%o", o);
            } else if (*format == 'x') {
                unsigned int x = va_arg(args, unsigned int);
                bufferIndex += snprintf(&buffer[bufferIndex], sizeof(buffer) - bufferIndex, "%x", x);
            } else if (*format == 'X') {
                unsigned int X = va_arg(args, unsigned int);
                bufferIndex += snprintf(&buffer[bufferIndex], sizeof(buffer) - bufferIndex, "%X", X);
            } else if (*format == '%') {
                buffer[bufferIndex++] = '%';
            }
        } else {
            buffer[bufferIndex++] = *format;
        }
        format++;
        if (bufferIndex >= sizeof(buffer) - 1) {
            write(STDOUT_FILENO, buffer, bufferIndex);
            count += bufferIndex;
            bufferIndex = 0;
        }
    }
    if (bufferIndex > 0) {
        write(STDOUT_FILENO, buffer, bufferIndex);
        count += bufferIndex;
    }
    va_end(args);
    return count;
}
