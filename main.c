#include "main.h"
#include "./oauth/oauth.c"
#include "./include/json-parser.h"


char *spreadsheets_values_read(char *spreadsheet_id, char *range) {
    char *url, *header, *r;
    int code;
    begin:

    makestr(url, "https://sheets.googleapis.com/v4/spreadsheets/%s/values:batchGet"
    "?ranges=%s&majorDimension=ROWS&valueRenderOption=FORMATTED_VALUE", spreadsheet_id, range);

    makestr(header, "Authorization: Bearer %s", oauth_access());
    
    r = hget_method("GET", url, "", &code, header);

    free(url);
    free(header);

    printf("statuscode: %i\n\n", code);

    if (code == 401)
    {
        free(url);
        free(header);
        free(r);
        oauth_refresh();
        goto begin;
    }

    if (code != 200)
    {
        free(r);
        fprintf(stderr, "ERROR: could not read %s:%s\n", spreadsheet_id, range);
        return NULL;
    }

    return r;
}

void spreadsheets_values_write(char *spreadsheet_id, char *range, size_t count, ...) {
    char *url, *data, *header, *r, *arr;
    int code, i;
    size_t length;
    va_list args;

    begin:

    //get length of values
    va_start(args, count);

    for (i=0,length=0; i<count; i++)
        length += strlen(va_arg(args, char*));

    va_end(args);

    //prepare data array
    arr = calloc(1, length +
                (count * strlen("\\\"\\\",[]")) +
                + 1);

    va_start(args, count);

    for (i=0; i<count; i++)
    {
        if (i > 0) strcat(arr, ",");
        strcat(arr, "[");
        strcat(arr, "\\\"");
        strcat(arr, va_arg(args, char*));
        strcat(arr, "\\\"");
        strcat(arr, "]");
    }

    //prepare request
    makestr(url, "https://sheets.googleapis.com/v4/spreadsheets/%s/values:batchUpdate", spreadsheet_id);

    makestr(header, "Authorization: Bearer %s", oauth_access());

    makestr(data,
    "{"
        "\\\"valueInputOption\\\":\\\"USER_ENTERED\\\","
        "\\\"data\\\":"
            "["
                "{"
                    "\\\"range\\\":\\\"%s\\\","
                    "\\\"majorDimension\\\":\\\"COLUMNS\\\","
                    "\\\"values\\\":[%s]"
                "}"
            "]"
    "}",
    range, arr);

    r = get(url, data, &code, header, "Content-Type: application/json");

    if (code == 401)
    {
        free(url);
        free(data);
        free(header);
        free(r);
        free(arr);
        oauth_refresh();
        goto begin;
    }

    if (code != 200)
    {
        printf(r);
        fprintf(stderr, "ERROR: could not write at %s:%s\n", spreadsheet_id, range);
    }

    free(r);
    free(url);
    free(data);
    free(header);
    free(arr);
}
#define spreadsheets_values_write(spreadsheet_id, range, ...) \
    spreadsheets_values_write(spreadsheet_id, range, counter(#__VA_ARGS__), __VA_ARGS__)

void spreadsheets_values_clear(char *spreadsheet_id, char *range) {
    char *url, *data, *header, *r;
    int code;

    begin:

    makestr(url, "https://sheets.googleapis.com/v4/spreadsheets/%s/values:batchClear", spreadsheet_id);

    makestr(header, "Authorization: Bearer %s", oauth_access());

    makestr(data, "{\\\"ranges\\\":[\\\"%s\\\"]}", range);

    r = get(url, data, &code, header, "Content-Type: application/json");

    if (code != 200)
    {
        fprintf(stderr, "ERROR: could not clear at %s:%s\n", spreadsheet_id, range);
    }

    if (code == 401)
    {
        free(url);
        free(data);
        free(header);
        free(r);
        oauth_refresh();
        goto begin;
    }

    free(url);
    free(data);
    free(header);
    free(r);
}

void spreadsheets_values_append(char *spreadsheet_id, char *range, size_t count, ...) {
    char *url, *data, *header, *r, *arr;
    int code, i;
    size_t length;
    va_list args;

    begin:

    //get length of values
    va_start(args, count);

    for (i=0,length=0; i<count; i++)
        length += strlen(va_arg(args, char*));

    va_end(args);

    //prepare data array
    arr = calloc(1, length +
                (count * strlen("\\\"\\\",[]")) +
                + 1);

    va_start(args, count);

    for (i=0; i<count; i++)
    {
        if (i > 0) strcat(arr, ",");
        strcat(arr, "[");
        strcat(arr, "\\\"");
        strcat(arr, va_arg(args, char*));
        strcat(arr, "\\\"");
        strcat(arr, "]");
    }

    //prepare request
    makestr(url, "https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s:append"
        "?valueInputOption=USER_ENTERED&insertDataOption=OVERWRITE", spreadsheet_id, range);

    makestr(header, "Authorization: Bearer %s", oauth_access());

    makestr(data,
    "{"
        "\\\"range\\\":\\\"%s\\\","
        "\\\"majorDimension\\\":\\\"COLUMNS\\\","
        "\\\"values\\\":[%s]"
    "}",
    range, arr);

    r = get(url, data, &code, header, "Content-Type: application/json");

    if (code == 401)
    {
        free(url);
        free(data);
        free(header);
        free(r);
        free(arr);
        oauth_refresh();
        goto begin;
    }

    if (code != 200)
    {
        printf(r);
        fprintf(stderr, "ERROR: could not append at %s:%s\n", spreadsheet_id, range);
    }

    free(r);
    free(url);
    free(data);
    free(header);
    free(arr);
}
#define spreadsheets_values_append(spreadsheet_id, range, ...) \
    spreadsheets_values_append(spreadsheet_id, range, counter(#__VA_ARGS__), __VA_ARGS__)

void spreadsheets_sheets_copyTo(char *source_spreadsheet_id, char *source_sheet_id, char *dest_spreadsheet_id) {
    char *url, *data, *header, *r;
    int code;

    begin:

    makestr(url, "https://sheets.googleapis.com/v4/spreadsheets/%s/sheets/%s:copyTo",
        source_spreadsheet_id, source_sheet_id);

    makestr(header, "Authorization: Bearer %s", oauth_access());

    makestr(data, "{\\\"destinationSpreadsheetId\\\":\\\"%s\\\"}", dest_spreadsheet_id);

    r = get(url, data, &code, header, "Content-Type: application/json");

    if (code == 401)
    {
        free(url);
        free(data);
        free(header);
        free(r);
        oauth_refresh();
        goto begin;
    }

    if (code != 200)
    {
        fprintf(stderr, "ERROR: could not copy sheets from %s to %s\n",
            source_spreadsheet_id, dest_spreadsheet_id);
    }

    free(url);
    free(data);
    free(header);
    free(r);
}

char *spreadsheets_create(char *name, char *width, char *height) {
    char *data, *header, *r, *id;
    int code;
    json_t *json;
    size_t c;

    begin:

    makestr(header, "Authorization: Bearer %s", oauth_access());

  	makestr(data,
	"{"
		"\\\"properties\\\":"
		"{"
			"\\\"title\\\":\\\"%s\\\",\\\"timeZone\\\":\\\"America/Sao_Paulo\\\""
		"}"
		",\\\"sheets\\\":"
		"[{"
			"\\\"properties\\\":"
			"{"
				"\\\"sheetId\\\":\\\"0\\\""
				",\\\"gridProperties\\\":"
				"{"					
					"\\\"rowCount\\\":\\\"%s\\\""
					",\\\"columnCount\\\":\\\"%s\\\""
				"}"
			"}"			
		"}]"
	"}",
	name, height, width);

    r = get("https://sheets.googleapis.com/v4/spreadsheets", data, &code, header, "Content-Type: application/json");

    if (code == 401)
    {
        free(data);
        free(header);
        free(r);
        oauth_refresh();
        goto begin;
    }

    if (code != 200)
    {
        fprintf(stderr, "ERROR: could not create spreadsheet\n");
    }

    json = json_parse(r, &c);

    id = malloc(strlen(json[0].value) + 1);
    strcpy(id, json[0].value);

    chopstring(id, 2, strlen(id)-2, id);
    
    json_free(json, c);
    free(data);
    free(header);
    free(r);

    return id;
}

char *spreadsheets_get(char *spreadsheet_id) {
    char *url, *header, *r;
    int code;

    begin:

    makestr(url, "https://sheets.googleapis.com/v4/spreadsheets/%s", spreadsheet_id);

    makestr(header, "Authorization: Bearer %s", oauth_access());
    
    r = hget_method("GET", url, "", &code, header);

    if (code == 401)
    {
        oauth_refresh();
        goto begin;
    }

    if (code != 200)
    {
        fprintf(stderr, "ERROR: could not get %s\n", spreadsheet_id);
    }

    free(url);
    
    return r;
}

int main(void) {

	oauth_credentials_set("client-id-here", "client-secret-here"); //grab at Google Cloud Console
	
	oauth_refresh_set("refresh-token-here");

    char *r = spreadsheets_values_read("spreadsheet-id", "A1:B30");

    free(r);

    return 0;
}