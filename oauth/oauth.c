#include "oauth.h"
#include "../include/easy-string.h"
#include "../include/makestr.h"

/*
https://accounts.google.com/o/oauth2/auth?acess_type=offline&approval_prompt=auto&client_id=699363216020-2rqc120u9n2ftpe0721pjdp07bkfgm5s.apps.googleusercontent.com
&response_type=code&scope=https://www.googleapis.com/auth/spreadsheets&redirect_uri=http://localhost
*/

#define CLIENT_ID_SIZE 75
char CLIENT_ID[CLIENT_ID_SIZE] = {0};

#define CLIENT_SECRET_SIZE 30
char CLIENT_SECRET[CLIENT_SECRET_SIZE] = {0};

#define ACCESS_TOKEN_SIZE 200
char ACCESS_TOKEN[ACCESS_TOKEN_SIZE] = {0};

#define REFRESH_TOKEN_SIZE 120
char REFRESH_TOKEN[REFRESH_TOKEN_SIZE] = {0};

void oauth_code(char *code)  {
	char *r, *data;
	int statuscode;

	makestr(data,
	"grant_type=authorization_code"
	"&code=%s"
	"&client_id=%s"
	"&client_secret=%s"
	"&redirect_uri=http://localhost",
	code, CLIENT_ID, CLIENT_SECRET);
	
	r = curl_go("POST", "https://accounts.google.com/o/oauth2/token", data, &statuscode, "");
	
	printf("%s\n", r);
	
	free(r);
    free(data);
}

void oauth_credentials_set(char *client_id, char *client_secret) {
	strncpy(CLIENT_ID, client_id, CLIENT_ID_SIZE-1);
	strncpy(CLIENT_SECRET, client_secret, CLIENT_SECRET_SIZE-1);
}

void oauth_refresh_set(char *refresh_token) {
	strncpy(REFRESH_TOKEN, refresh_token, REFRESH_TOKEN_SIZE-1);
}

void oauth_refresh() {
	char *r, *data;
	int code;
    
	makestr(data,
	"grant_type=refresh_token"
	"&refresh_token=%s"
	"&client_id=%s"
	"&client_secret=%s"
	"&redirect_uri=http://localhost",
	REFRESH_TOKEN, CLIENT_ID, CLIENT_SECRET);
	
	r = curl_go("POST", "https://accounts.google.com/o/oauth2/token", data, &code, "");
	
    if (code != 200)
    {
        free(r);
        free(data);
        fprintf(stderr, "ERROR: could not refresh access token.\n");
        return;
    }
	
	chopstring(r, indexOf(r, "\"", 2)+1, indexOf(r, "\"", 3)-1, ACCESS_TOKEN);
	
	free(r);
    free(data);
}

char *oauth_access() {

    if(*ACCESS_TOKEN == 0) oauth_refresh();

    if(*ACCESS_TOKEN != 0) return ACCESS_TOKEN;

    return "";
}