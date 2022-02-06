#ifndef GSHEETS_H
#define GSHEETS_H

#include <stdio.h>

char *spreadsheets_values_read(char *spreadsheet_id, char *range);
void spreadsheets_values_write(char *spreadsheet_id, char *range, size_t count, ...);
void spreadsheets_values_clear(char *spreadsheet_id, char *range);
void spreadsheets_values_append(char *spreadsheet_id, char *range, size_t count, ...);

void spreadsheets_sheets_create(char *spreadsheet_id, char *name, char *width, char *height);
void spreadsheets_sheets_delete(char *spreadsheet_id, char *sheets_id);
void spreadsheets_sheets_edit_name(char *spreadsheet_id, char *sheets_id, char *new_name);
void spreadsheets_sheets_copyTo(char *source_spreadsheet_id, char *source_sheet_id, char *dest_spreadsheet_id);

char *spreadsheets_create(char *name, char *width, char *height);
char *spreadsheets_get(char *spreadsheet_id);

#endif //GSHEETS_H