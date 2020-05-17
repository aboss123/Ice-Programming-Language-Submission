#pragma once
#include "common.h"


typedef enum {
	IONullPointer,
	ReadNullPointer,
	InvalidVariableType,
	UnexpectedToken,
	IncompatibleTypes,
	OperatorDoesNotApply,
	HeapCorruption,
	DoubleFree,
	UnexpectedEOF,
} ErrorType;

void ice_error(int line, int start_col, int end_col, const char *line_text, const char *error, ...);
void ice_error_expected_token(int line, int end_col, const char *line_text, int start_col, const char *error, const char *expected_token, ...);
void warning(const char *warning, ...);
void fatal(const char *error, ...);
