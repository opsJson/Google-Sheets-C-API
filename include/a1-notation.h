#ifndef A1_NOTATION_H
#define A1_NOTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char itoc(int i) {
	if (i < 0) return 0;
	
	return ((i-1) % 26) + 65;
}

int ctoi(char c) {
	if (c < 65) return 0;
	if (c > 90) return 0;
	
	return c - 64;
}

char* itoa1(int i) {
	char *a1;
	int count = 0;
	int back = i;
	
	while (i) {
		count++;
		i = (i-1) / 26;
	}
	
	a1 = malloc(sizeof(char) * count + 1);
	
	a1[count] = 0;
	
	while (back) {
		a1[--count] = itoc(back);
		back = (back-1) / 26;
	}
	
	return a1;	
}

int a1toi(char *a1) {
	int i;
	int base;
	int integer = 0;
	size_t length;
	
	length = strlen(a1);
	
	for (i=length-1,base=0; i>=0; i--, base++) {
		
		integer += ctoi(a1[i]) * (int)pow(26, base);
	}
	
	return integer;
}

#endif
