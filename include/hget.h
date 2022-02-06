#ifndef HGET_H
#define HGET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "file-buffer.h"
#include "easy-string.h"
#include "makestr.h"

char *get(char *url, char *data, int *statuscode, size_t count, ...) {
    va_list args;
    size_t length = 0, i;
    char *comando, *response, *statusline;

    //get headers length
    va_start(args, count);

    for (i=0; i<count; i++)
        length += strlen(va_arg(args, char*)) + strlen("-H \"\" ");

    //get url data and other length
    length += strlen(url);
    length += strlen(data);
    length += strlen("curl \"\"");
    length += strlen("--data \"\"");
    length += strlen("-i --output response.txt ");

    //allocate
    comando = (char*)malloc(sizeof(char) * length + 1);

    //set url and data
    sprintf(comando, "curl \"%s\" --data \"%s\" -i --output response.txt ", url, data);

    //set headers
    va_end(args);
    va_start(args, count);

    for (i=0; i<count; i++) {
        strcat(comando, "-H \"");
        strcat(comando, va_arg(args, char*));
        strcat(comando, "\" ");
    }

    //perform
    system(comando);

    //get response
    response = ftob("response.txt");
    remove("response.txt");

    //get statuscode
    statusline = substring(response, indexOf(response, " ", 0)+1, indexOf(response, " ", 1)-1);
    *statuscode = atoi(statusline);

    //parse
    chopstring(response, indexOf(response, "\n\n", 0)+2, strlen(response), response);

    return response;
}
#define get(url, data, statuscode, ...) get(url, data, statuscode, counter(#__VA_ARGS__), __VA_ARGS__)

char *hget_method(char *method, char *url, char *data, int *statuscode, size_t count, ...) {
    va_list args;
    size_t length = 0, i;
    char *comando, *response, *statusline;

    //get headers length
    va_start(args, count);

    for (i=0; i<count; i++)
        length += strlen(va_arg(args, char*)) + strlen("-H \"\" ");

    //get url data and other length
    length += strlen(method);
    length += strlen("-X \"\"");
    length += strlen(url);
    length += strlen(data);
    length += strlen("curl \"\"");
    length += strlen("--data \"\"");
    length += strlen("-i --output response.txt ");

    //allocate
    comando = (char*)malloc(sizeof(char) * length + 1);

    //set url and data
    sprintf(comando, "curl \"%s\" --data \"%s\" -X \"%s\" -i --output response.txt ", url, data, method);

    //set headers
    va_end(args);
    va_start(args, count);

    for (i=0; i<count; i++) {
        strcat(comando, "-H \"");
        strcat(comando, va_arg(args, char*));
        strcat(comando, "\" ");
    }

    //perform
    system(comando);

    //get response
    response = ftob("response.txt");
    remove("response.txt");

    //get statuscode
    statusline = substring(response, indexOf(response, " ", 0)+1, indexOf(response, " ", 1)-1);
    *statuscode = atoi(statusline);

    //parse
    chopstring(response, indexOf(response, "\n\n", 0)+2, strlen(response), response);

    return response;
}
#define hget_method(method, url, data, statuscode, ...) hget_method(method, url, data, statuscode, counter(#__VA_ARGS__), __VA_ARGS__)

#endif