#ifndef GSHEETS_OAUTH_H
#define GSHEETS_OAUTH_H

void oauth_code(char *code);

void oauth_credentials_set(char *cliente_id, char *cliente_secret);

void oauth_refresh_set(char *refresh_token);

void oauth_refresh();

char *oauth_access();

#endif //GSHEETS_OAUTH_H