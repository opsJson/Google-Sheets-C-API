# Google-Sheets-C-API
Google Sheets Rest API implementation in C

## First use

1. Set your Google Cloud Console credentials with oauth_credentials_set()
1. Exchange oauth code for a refresh token with oauth_code()
> URL template to get oauth code:
> https://accounts.google.com/o/oauth2/auth?acess_type=offline&approval_prompt=auto&client_id=client-id&response_type=code&scope=https://www.googleapis.com/auth/spreadsheets&redirect_uri=http://localhost

## Later uses

1. Set your Google Cloud Console credentials with oauth_credentials_set()
1. Set a valid refresh token with oauth_refresh_set()
1. Enjoy
