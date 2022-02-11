#ifndef CURL_GO
#define CURL_GO

extern char *curl_go(char*, char*, char*, long int *, ...);

size_t counter(char *str) {
	size_t commas = 0;
	int string = 0;
	
	while (*str) {
		//escape commas inside string;
		if (*str == '"' && *(str - 1) != '\\') string = !string;
		
		//count commas
		if (string == 0)
			if (*str == ',') commas++;					
		str++;
	}
		
	return commas+1;
}
#define curl_go(method, url, data, code, ...) \
	curl_go(method, url, data, (long int*)code, counter(#__VA_ARGS__), __VA_ARGS__)

#endif //CURL_GO
