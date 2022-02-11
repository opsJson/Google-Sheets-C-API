#ifndef EASY_STRING_H
#define EASY_STRING_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

char* __countstr__(char *format, size_t count, ...) {
	int i;
	char *str;
	va_list args;
	size_t length;
	va_start(args, count);
	
	for (i=0,length=0; i<count; i++)
		length += strlen(va_arg(args, char*));
		
	str = (char*)malloc((sizeof(char) * length)
					  + (sizeof(char) * strlen(format))
					  + 1);
	
	return str;
}

char* makestr(char *ptr, char *format, ...) {return 0;}
#define makestr(str, format, ...) \
	str = __countstr__(format, counter(#__VA_ARGS__), __VA_ARGS__); \
	sprintf(str, format, __VA_ARGS__);

//////////////////////////////////////////////

#define str_assert(condition) \
	{ \
		if (!(condition)) { \
			fprintf(stderr, "Expression: '%s' failed at %s:%i\n", \
				#condition, file, line); \
			return 0; \
		} \
	}
	
#define str_assert_void(condition) \
	{ \
		if (!(condition)) { \
			fprintf(stderr, "Expression: '%s' failed at %s:%i\n", \
				#condition, file, line); \
			return; \
		} \
	}	

int indexOf(char *source, char *key, int pos, char *file, int line) {
	int i, j;
	size_t length;
	
	str_assert(source != NULL);
	str_assert(key != NULL);
	str_assert(pos > -1);
	
	length = strlen(source);
	
	for (i=0,j=0; i<length; i++) {
		(source[i] == key[j]) ? (j++) : (j=0);
		
		if (j == strlen(key)) {
			j = 0;
			if (pos == 0) return i - strlen(key) + 1;
			pos--;
		}
	}
	
	return -1;
}
#define indexOf(source, key, pos) indexOf(source, key, pos, __FILE__, __LINE__)

char *chopstring(char *source, int start, int end, char *toChop, char *file, int line) {
	
	str_assert(source != NULL);
	str_assert(start > -1);
	str_assert(end > -1);
	str_assert(start <= end);
	
	strncpy(toChop, source + start, end - start + 1);
	toChop[end - start + 1] = 0;
	
	return toChop;
}
#define chopstring(source, start, end, toChop) chopstring(source, start, end, toChop, __FILE__, __LINE__)

char* substring(char* source, int start, int end, char *file, int line) {
	char *dest, *back;
	
	str_assert(source != NULL);
	str_assert(start > -1);
	str_assert(end > -1);
	str_assert(start <= end);
	
	dest = malloc(sizeof(char) * strlen(source) +1);
	str_assert(dest != NULL);
	
	strcpy(dest, source);
	back = dest;

	memcpy(dest, dest + start, strlen(dest + start) + 1);
	*(dest + end - start + 1) = 0;

	return back;
}
#define substring(source, start, end) substring(source, start, end, __FILE__, __LINE__)

char* _substring(char *source, int start, int end, char *file, int line) {
	str_assert(source != NULL);
	str_assert(start > -1);
	str_assert(end > -1);
	str_assert(start <= end);
	
	source = source + start;
	source[end - start + 1] = 0;
	
	return source;
}
#define _substring(source, start, end) _substring(source, start, end, __FILE__, __LINE__)

char *replace(char *source, char *older, char *newer, char *file, int line) {
	int older_index;
	
	str_assert(source != NULL);
	str_assert(older != NULL);
	str_assert(newer != NULL);
	
	older_index = indexOf(source, older, 0);
	str_assert(older_index != -1);
	
	memmove(source + older_index + strlen(newer), source + older_index + strlen(older),
		strlen(source + older_index) - strlen(older));
	
	memcpy(source + older_index, newer, strlen(newer));
	
	return source;
}
#define replace(source, older, newer) replace(source, older, newer, __FILE__, __LINE__)

char *getNumbers(char *source, char *file, int line) {
	int i, j;
	size_t length;
	
	str_assert(source != NULL);
	
	length = strlen(source);
	
	for (i=0,j=0; i<length; i++) {
		if (source[i] > 47 && source[i] < 58) source[j++] = source[i];
	}
	source[j] = 0;
	
	return source;
}
#define getNumbers(source) getNumbers(source, __FILE__, __LINE__)

char *toUpper(char *source, char *file, int line) {
	int i;
	size_t length;
	
	str_assert(source != NULL);
	
	length = strlen(source);
	
	for (i=0; i<length; i++) {
		if (source[i] > 96 && source[i] < 123) source[i] = toupper(source[i]);
	}
	source[i] = 0;
	
	return source;
}
#define toUpper(source) toUpper(source, __FILE__, __LINE__)

char *toLower(char *source, char *file, int line) {
	while (*source++) {
		if (*source > 64 && *source < 91) {
		    *source = tolower(*source);						
		}		
	}
	
	return source;
}
#define toLower(source) toLower(source, __FILE__, __LINE__)

char *noSpace(char *source, char *file, int line) {
	int i, j, whitespace;
	size_t length;
	
	str_assert(source != NULL);
	
	length = strlen(source);
	
	for (i=0,j=0,whitespace=0; i<length; i++) {
		
		(source[i] == ' ') ? (whitespace = 1) : (whitespace = 0);
		if (!whitespace) source[j++] = source[i];
	}
	source[j] = 0;
	
	return source;
}
#define noSpace(source) noSpace(source, __FILE__, __LINE__)

void trim(char *source, char *file, int line) {
	int i, j, whitespace;
	size_t length;
	
	str_assert_void(source != NULL);
	
	length = strlen(source);
	
	for (i=0,j=0,whitespace=0; i<length; i++) {
		
		if (!whitespace) source[j++] = source[i];
		(isspace(source[i]) && isspace(source[i+1])) ? (whitespace = 1) : (whitespace = 0);
	}
	source[j] = 0;
}
#define trim(source) trim(source, __FILE__, __LINE__)

#endif
