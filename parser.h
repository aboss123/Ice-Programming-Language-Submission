#pragma once
#include "tok.h"
#include "ast.h"

typedef struct {
	Tokenizer	tokenizer;
	BOOL		statement;
	BOOL		function;
	BOOL		loop;
	int pos;
} Parser;

#define curr(parser) __peek__(parser, 0)
#define curr_line(parser, tok) parser->tokenizer.data.lines[tok.line - 1]
#define next(parser) (parser->pos++, __peek__(parser, 0))

void __freeparser__(Parser *parser); // Also frees tokenizer
int operator_precedence(Parser *parser);
Parser __parser__();
Ast **__parse__(Parser *parser);
Ast *parse_assignment(Parser *parser, TokEntity name);
Ast *parse_stmt(Parser *parser);
Ast *parse_print(Parser *parser);
Ast* parse_rand(Parser *parser);
Ast *parse_expr(Parser *parser, int parent_precedence);
Ast *parse_input(Parser *parser);
Ast *parse_ifexpr(Parser *parser);
Ast *parse_while(Parser *parser);