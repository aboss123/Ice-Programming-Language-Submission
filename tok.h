#pragma once
#include "error.h"

/** Keyword Pointer Comparison */
extern const char * keyword_if;
extern const char * keyword_else ;
extern const char * keyword_elif ;
extern const char * keyword_while ;
extern const char * keyword_for ;
extern const char * keyword_var ;
extern const char * keyword_double ;
extern const char * keyword_int ;
extern const char * keyword_bool ;
extern const char * keyword_break ;
extern const char * keyword_continue ;
extern const char * keyword_return ;
extern const char * keyword_and ;
extern const char * keyword_or ;
extern const char * keyword_print ;
extern const char * keyword_input ;
extern const char * keyword_rand ;
extern const char * keyword_true ;
extern const char * keyword_false ;

/** Symbol Pointer Comparison */
extern const char * symbol_plus ;
extern const char * symbol_minus ;
extern const char * symbol_div ;
extern const char * symbol_star ;
extern const char * symbol_mod ;
extern const char * symbol_lparen ;
extern const char * symbol_rparen ;
extern const char * symbol_colon ;
extern const char * symbol_semicolon ;
extern const char * symbol_hash ;
extern const char * symbol_not ;
extern const char * symbol_bitwiseand ;
extern const char * symbol_bitwisexor ;
extern const char * symbol_bitwiseor ;
extern const char * symbol_bitwisenot ;
extern const char * symbol_gtr ;
extern const char * symbol_less ;
extern const char * symbol_equal ;
extern const char * symbol_plusplus ;
extern const char * symbol_minusminus ;
extern const char * symbol_lshift ;
extern const char * symbol_rshift ;
extern const char * symbol_gtrorequal ;
extern const char * symbol_lessorequal ;
extern const char * symbol_equalequal ;
extern const char * symbol_plusequal ;
extern const char * symbol_minusequal ;
extern const char * symbol_notequal ;
extern const char * symbol_divequal ;
extern const char * symbol_timesequal ;
extern const char * symbol_bitwiseandequal ;
extern const char * symbol_bitwiseorequal ;
extern const char * symbol_bitwisexorequal ;
extern const char * symbol_lshiftequal ;
extern const char * symbol_rshiftequal ;
extern const char * symbol_colonequal ;
extern const char * symbol_lcurly ;
extern const char * symbol_rcurly ;
extern const char* symbol_comma ;

/** TypeDefs */
typedef _Bool BOOL;

typedef enum {
	Identifier,
	StringLiteral,
	Int32,  // <--- LLVM unsigned / signed handled
	Int64, //  <--- LLVM unsigned / signed handled
	Double,
	Ellipsis,
	Symbol,
	TabToken, //  Special Tokens that represent blocks that are assigned to a statement
	EOFToken
} TokType;

typedef struct {
	int		line;
	int		col;
	TokType type;
	union {
		const char	*string;
		uintmax_t	int_v;
		double		fp;
		BOOL		boolean;
	};
} TokEntity;

typedef struct {
	int			line_c;
	const char	**lines;
} FileData;

typedef struct {
	FileData	data;
	TokEntity	*tokens;
} Tokenizer;


/** Macros */
#define whitespace(x)	x == ' '
#define equ(x, y)		x == y	
#define eof(x)			x == '\0'
#define null(x)			x == 0
#define quote(x)		x == '"'
#define zero(x)			x == 0
#define digit(x)		x >= '0'&& x <= '9'

void __readtok__(Tokenizer* tokenizer, const char *stream);
void __initreserved__();
Tokenizer __tokenizer__();
BOOL is_keyword(const char *a);
BOOL is_valid_symbol(const char *a);
