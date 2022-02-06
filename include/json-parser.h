#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum json_value_type {
	jv_string,
	jv_number,
	jv_array,
	jv_object
};

typedef struct json {
	char *key;
	char *value;
	int type;
} json_t;

char **array_parse(char *array, size_t *elem_count) {
	int pair = 0;
	int i = 0;
	
	size_t size = 0;
	size_t count = 1;
	
	char *array_copy;
	char **parsed;
	
	int inside_string = 0;
	
	array_copy = array;
	
	while (*array_copy++) {
		if (*array_copy == '\"') while (*++array_copy != '\"' || *(array_copy-1) == '\\');
		else if (*array_copy == ',') count++;
	}

	parsed = (char**)malloc(sizeof(char*) * count);
	*elem_count = count;
	
	array_copy = array;
	
	while (*array_copy++) {
		size++;
		
		if (*array_copy == '\"') inside_string = 1;
		if (*array_copy == '\"' && *(array_copy -1) != '\\') inside_string = 0;
		
		if (inside_string == 0)
			if (*array_copy == ',') parsed[pair++] = (char*)malloc(sizeof(char) * size + 1), size = 0;
	}
	parsed[pair++] = (char*)malloc(sizeof(char) * size + 1), size = 0;
	pair = 0;
	
	while (*array++) {
		
		if (*array == '\"') inside_string = 1;
		if (*array == '\"' && *(array -1) != '\\') inside_string = 0;
		
		if (inside_string == 0)
			if (*array == ',') parsed[pair++][i++] = 0, i = 0, array++;
		
		parsed[pair][i++] = *array;
	}
	parsed[pair++][i-2] = 0;
	
	return parsed;
}

json_t *json_parse(char *json, size_t *pair_count) {
	size_t size = 1;
	char *json_copy;
	
	int i = 0;
	int pair = 0;
	int array_pair = 1;
	int object_pair = 1;
	
	//count pairs
	json_copy = json;
	while (*json_copy++) {
		if (*json_copy == '\"') while (*++json_copy != '\"' || *(json_copy-1) == '\\');
		
		if (*json_copy == '[') {
			while (1) {
				json_copy++;
				if (*json_copy == '[') array_pair++;
				if (*json_copy == ']') array_pair--;
				if (array_pair == 0) break;
			}
		}
		
		if (*json_copy == ',') size++;
	}
	
	//allocate
	json_t *parsed = (json_t*)calloc(size, sizeof(json_t));
	*pair_count = size;
	
	while (*json) {
		
		if (*json == ',') pair++;
		
		//key
		else if (*json == '\"') {
			
			json_copy = json;
			size = 0;
			i = 0;
			
			while (*++json_copy != '\"' || *(json_copy-1) == '\\') size++;
			
			parsed[pair].key = (char*)malloc(sizeof(char) * size + 1);
			
			while(*++json != '\"' || *(json-1) == '\\') parsed[pair].key[i++] = *json;
			parsed[pair].key[i++] = 0;
		}
		
		else if (*json == ':') {
			
			//string
			if (*(json+1) == '\"') {
				
				parsed[pair].type = jv_string;
				
				json_copy = json;
				size = 0;
				i = 0;
				
				json_copy++;
				while (*++json_copy != '\"' || *(json_copy-1) == '\\') size++;
				
				parsed[pair].value = (char*)malloc(sizeof(char) * size + 1);
				
				json++;
				while (*++json != '\"' || *(json-1) == '\\') parsed[pair].value[i++] = *json;
				parsed[pair].value[i++] = 0;
			}
			
			//array
			else if (*(json+1) == '[') {
				
				parsed[pair].type = jv_array;
				
				json_copy = json;
				size = 0;
				i = 0;
				
				json_copy++;
				while (*json_copy++) {
					if (*json == '[') array_pair++;
					if (*json == ']') array_pair--;
					if (array_pair == 0) break;
					size++;
				}
				array_pair = 1;
				
				parsed[pair].value = (char*)malloc(sizeof(char) * size + 2 + 1);
				
				json++;
				parsed[pair].value[i++] = '[';
				while (*json++) {
					if (*json == '[') array_pair++;
					if (*json == ']') array_pair--;
					if (array_pair == 0) break;
					parsed[pair].value[i++] = *json;
				}
				parsed[pair].value[i++] = ']';
				array_pair = 1;
				
				parsed[pair].value[i++] = 0;
			}
			
			//object
			else if (*(json+1) == '{') {
				
				parsed[pair].type = jv_object;
				
				json_copy = json;
				size = 0;
				i = 0;
				
				json_copy++;
				while (*json_copy++) {
					if (*json == '{') array_pair++;
					if (*json == '}') array_pair--;
					if (array_pair == 0) break;
					size++;
				}
				array_pair = 1;
				
				parsed[pair].value = (char*)malloc(sizeof(char) * size + 2 + 1);
				
				json++;
				parsed[pair].value[i++] = '{';
				while (*json++) {
					if (*json == '{') object_pair++;
					if (*json == '}') object_pair--;
					if (object_pair == 0) break;
					parsed[pair].value[i++] = *json;
				}
				parsed[pair].value[i++] = '}';
				object_pair = 1;
				
				parsed[pair].value[i++] = 0;
			}
			
			//number or bool
			else {
				
				parsed[pair].type = jv_number;
				
				json_copy = json;
				size = 0;
				i = 0;
				
				while (*++json_copy != ',') {
					if (*json_copy == '}') break;
					size++;
				}
				
				parsed[pair].value = (char*)malloc(sizeof(char) * size + 1);
				
				while (*++json != ',') {
					if (*json == '}') break;
					parsed[pair].value[i++] = *json;
				}
				
				parsed[pair].value[i++] = 0;
				json--;
			}
		}
		
		json++;
	}
	
	return parsed;
}

void json_free(json_t *json, int count) {
	int i;
	
	for (i=0; i<count; i++) {
		if (json[i].value) 	free(json[i].value);
		if (json[i].key) 	free(json[i].key);
	}
	
	free(json);
}

void array_free(char **array, int count) {
	int i;
	
	for (i=0; i<count; i++)
		if (array[i]) free(array[i]);
	
	free(array);
}

#endif
