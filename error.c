#include "error.h"

void ice_error(int line, int start_col, int end_col, const char* line_text, const char *error, ...)
{
	va_list args;
	printf("error: ");
	printf("(Line: %i, Col: %i) ", line, end_col);
	va_start(args, error);
	vprintf(error, args);
	va_end(args);
	while (line_text == ' ') line_text++; // Skip whitespace
	printf("\t\t\n\t\t");
	for (int i = (start_col > 10 && start_col < 35 ? start_col - 10 : 0); i < end_col + 1; i++)
		printf("%c", *(line_text + i));
	printf("\n\t\t");
	for (int i = (start_col > 10 && start_col < 35 ? start_col - 10 : 0); i < end_col; i++) printf("~");
	printf("^\n");
	exit(1);
}

void ice_error_expected_token(int line, int end_col, const char *line_text, int start_col, const char* error, const char* expected_token, ...)
{
	va_list args;
	printf("error: ");
	printf("(Line: %i, Col: %i) ", line, end_col);
	va_start(args, error);
	vprintf(error, args);
	va_end(args);
	while (line_text == ' ') line_text++; // Skip whitespace
	printf("\t\t\n\t\t");
	for (int i = (start_col > 10 && start_col < 35 ? start_col - 10 : 0); i < end_col + 1; i++)
		printf("%c", *(line_text + i));
	printf("\n\t\t");
	for (int i = (start_col > 10 && start_col < 35 ? start_col - 10 : 0); i < end_col + 1; i++)
		printf("~");
	printf("^\n");
	printf("\n\t\t");
	for (int i = (start_col > 10 && start_col < 35 ? start_col - 10 : 0); i < end_col + 1; i++)
		printf(" ");
	printf("%s\n", expected_token);
	printf("Expected value '%s' instead of '%c'.\n", expected_token, *(line_text + end_col));
	exit(1);
}
void warning(const char *warning, ...)
{
	va_list args;
	va_start(args, warning);
	printf("warning: ");
	vprintf(warning, args);
	printf("\n");
	va_end(args);
}
void fatal(const char* error, ...)
{
	va_list args;
	va_start(args, error);
	printf("fatal: ");
	vprintf(error, args);
	va_end(args);
	exit(1);
}

