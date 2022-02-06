#ifndef FILE_BUFFER_H
#define FILE_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ftob(char *file_name, int *length, ...) {
    FILE *fp;
    size_t size;
    char *buffer;
    
    if (length) *length = 0;
    
    if ((fp = fopen(file_name, "r")) == NULL) {
    	fprintf(stderr, "Couldn't open '%s'.\n", file_name);
    	return 0;
	}

    if (fseek(fp, 0, SEEK_END) != 0) {
    	fclose(fp);
    	
    	fprintf(stderr, "Couldn't set '%s' to the end.\n", file_name);
    	return 0;
	}
    
    if ((size = ftell(fp)) == -1) {
    	fclose(fp);
    	
    	fprintf(stderr, "Couldn't get size of '%s'.\n", file_name);
    	return 0;
	}

    if (fseek(fp, 0, SEEK_SET) != 0) {
    	fclose(fp);
    	
    	fprintf(stderr, "Couldn't set '%s' to the start.\n", file_name);
    	return 0;
	}
    
    if (length) *length = size;
    
    buffer = (char*)malloc(size * sizeof(char) + 1);
    if (buffer == NULL) {
    	fclose(fp);
    	
    	fprintf(stderr, "Couldn't allocate buffer for '%s'.\n", file_name);
    	return 0;
	}

    memset(buffer, 0, size + 1);
    
    fread(buffer, sizeof(char), size, fp);

    fclose(fp);
    
    return buffer;
}
#define ftob(...) \
	ftob(__VA_ARGS__, 0)

size_t btof(char *file_name, char *buffer) {
	FILE *fp;
	size_t amount;
	
    if ((fp = fopen(file_name, "w")) == NULL) {
    	fprintf(stderr, "Couldn't open '%s'.\n", file_name);
    	return 0;
	}
	
	amount = fwrite(buffer, strlen(buffer), 1, fp);
	
	fclose(fp);
	
	return amount;
}


#endif
