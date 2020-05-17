#include "parser.h"


Parser __parser__()
{
	Parser parser;
	parser.pos			= 0;
	parser.tokenizer	= __tokenizer__();
	return parser;
}

__forceinline TokEntity __peek__(Parser *parser, uint32_t offset)
{
	return parser->tokenizer.tokens[offset + parser->pos];
}

void expect_symbol(Parser *parser, const char *sym)
{
	TokEntity curr = curr(parser);
	printf("Curr: %s\n", curr.string);
	if (curr.string != sym && curr.type != Symbol) ice_error_expected_token(curr.line, curr.col, curr_line(parser, curr), 0, "Unexpected Token", sym);
	parser->pos++;
}


void expect_type(Parser *parser, TokType type)
{
	TokEntity curr = curr(parser);
	if (curr.type != type) ice_error(curr.line, 0, curr.col, curr_line(parser, curr), "Invalid Token Type!");
	parser->pos++;
}

int unary_precedence(Parser *parser)
{
	TokEntity curr = curr(parser);
	if (curr.string == symbol_plus || curr.string == symbol_minus || curr.string == symbol_not)
		return 8;
	return 0;
}

int operator_precedence(Parser* parser)
{
	TokEntity curr = curr(parser);
	if (curr.string == symbol_star || curr.string == symbol_div || curr.string == symbol_mod)
		return 7;
	else if (curr.string == symbol_plus || curr.string == symbol_minus)
		return 6;
	else if (curr.string == symbol_gtr || curr.string == symbol_gtrorequal || curr.string == symbol_less || curr.string == symbol_lessorequal)
		return 5;
	else if (curr.string == symbol_equalequal || curr.string == symbol_notequal)
		return 4;
	else if (curr.string == keyword_and)
		return 3;
	else if (curr.string == keyword_or)
		return 2;
	else if (curr.string == symbol_bitwiseand || curr.string == symbol_bitwisenot || curr.string == symbol_bitwisexor || curr.string == symbol_bitwiseor)
		return 1;
	return 0;
}

Ast** __parse__(Parser* parser)
{
	int c = 1;
	Ast** asts = NULL;
	parser->statement = false;
	while (curr(parser).type != EOFToken)
	{
		Ast* stmt = parse_stmt(parser);
		vec_push(asts, stmt);
	}
	return asts;
}

Ast *parse_stmt(Parser *parser)
{
	switch (curr(parser).type)
	{
		case EOFToken:
			return NULL;
		case Identifier:
		{
			TokEntity curr = curr(parser);
			TokEntity next = __peek__(parser, 1);
			if (next.string == symbol_colonequal)
			{
				// parse_assignment
				TokEntity i = next(parser);
				parse_assignment(parser, curr);
			}
			else if (curr.string == keyword_print)
			{
				if (next.string != symbol_lparen)
					expect_symbol(parser, "(");
				next(parser);
				next(parser);
				parse_print(parser);
			}
			else if (curr.string == keyword_rand)
			{
				return parse_rand(parser);
			}
			else if (curr.string == keyword_input)
			{
				if (next.string != symbol_lparen)
					expect_symbol(parser, "(");
				next(parser);
				next(parser);
				parse_input(parser);
			}
			else if (curr.string == keyword_if)
				parse_ifexpr(parser);
			else if (curr.string == keyword_while)
				parse_while(parser);
			break;
		}
		default:
			break;
	}
}

Ast *match_expr(Parser *parser)
{
	TokEntity curr = curr(parser);
	if (curr.type == EOFToken) return NULL;
	if (curr.type == Identifier && __peek__(parser, 1).string == symbol_lparen)
		return parse_stmt(parser);
	if (curr.type == Int64 || curr.type == StringLiteral || curr.type == Double || curr.type == Identifier)
	{
		Ast *lit = malloc(sizeof(Ast));
		lit->type = LiteralAST;
		lit->literalast = (LiteralAst) {
			.literal = curr
		};
		next(parser);
		return lit;
	}
	else if (curr.string == symbol_lparen)
	{
		parser->pos++;
		Ast *expr = parse_expr(parser, 0);
		expect_symbol(parser, symbol_rparen);
		return expr;
	}
	TokEntity c = curr(parser);
	ice_error_expected_token(c.line, c.col, curr_line(parser, c), 0, "Unexpected Token in Expression", "valid binary expression token");
	return NULL;
}

Ast *parse_expr(Parser *parser, int parent_precedence)
{
	Ast *left = malloc(sizeof(Ast));
	int uprecedence = unary_precedence(parser);
	if (uprecedence != 0 && uprecedence >= parent_precedence)
	{
		TokEntity op = next(parser);
		Ast* operand = parse_expr(parser, uprecedence);
		left->type = UnaryAST;
		left->unaryast = (UnaryAst) {
			.op		= op,
			.expr	= operand
		};
	} else left = match_expr(parser);
	while (true)
	{
		int precedence = operator_precedence(parser);
		if (precedence == 0 || precedence <= parent_precedence)
			break;
		TokEntity op = curr(parser);
		TokEntity current = next(parser);
		//if (current.string != symbol_lparen) ice_error_expected_token(current.line, current.col, curr_line(parser, current), 0, "Unexpected Token", "(");
		Ast *right = parse_expr(parser, precedence);
		Ast *l = malloc(sizeof(Ast));
		*l = *left;
		left->type = BinaryAST;
		left->binaryast = (BinaryAst) {
			.lhs = l,
			.op = op,
			.rhs = right
		};
	}
	return left;
}

Ast *parse_assignment(Parser *parser, TokEntity name)
{
	if (is_keyword(name.string))
	{
		// Error!
	}
	parser->statement = true;
	next(parser);
	Ast *expr = parse_expr(parser, 0);
	Ast* node = malloc(sizeof(Ast));
	node->type = AssignmentAST;
	node->assignmentast = (AssignmentAst) {
		.expr = expr,
		.name = name
	};
	expect_symbol(parser, symbol_semicolon);
	parser->statement = false;
	return node;
}

Ast *parse_print(Parser *parser)
{
	Ast *expr = parse_expr(parser, 0);
	Ast *node = malloc(sizeof(Ast));
	parser->statement = true;
	node->type = PrintAST;
	node->printast = (PrintAst) {
		.expr = expr
	};
	expect_symbol(parser, symbol_rparen);
	expect_symbol(parser, symbol_semicolon);
	parser->statement = false;
	return node;
}

Ast *parse_rand(Parser *parser)
{
	next(parser);
	expect_symbol(parser, symbol_lparen);
	Ast *min = parse_expr(parser, 0);
	expect_symbol(parser, symbol_comma);
	Ast *max = parse_expr(parser, 0);
	expect_symbol(parser, symbol_rparen);
	Ast *node = malloc(sizeof(Ast));
	node->type = RandAST;
	node->randast = (RandAst) {
		.min = min,
		.max = max
	};
	if (!parser->statement) expect_symbol(parser, symbol_semicolon);
	return node;
}

Ast *parse_input(Parser *parser)
{
	Ast* expr = parse_expr(parser, 0);
	Ast* node = malloc(sizeof(Ast));
	node->type = InputAST;
	node->inputast = (InputAst){
		.expr = expr
	};
	expect_symbol(parser, symbol_rparen);
	if(!parser->statement) expect_symbol(parser, symbol_semicolon);
	return node;
}

Ast *parse_block(Parser *parser, BOOL flag)
{
	Ast **stmts = NULL;
	int c = 0;
	if (!flag) expect_symbol(parser, symbol_lcurly);
	while (curr(parser).string != symbol_rcurly && curr(parser).type != EOFToken)
	{
		c++;
		Ast *stmt = parse_stmt(parser);
		vec_push(stmts, stmt);
		if (flag && c == 1) break;
	}
	if (!flag) expect_symbol(parser, symbol_rcurly);
	Ast *node = malloc(sizeof(Ast));
	node->type = BlockAST;
	node->blockast = (BlockAst) {
		.stmts = stmts
	};

	return node;
}

Ast* parse_ifexpr(Parser* parser)
{
	next(parser); // Get to the condition
	Ast* condition = parse_expr(parser, 0);
	Ast* body = NULL;
	Ast* else_ = NULL;
	Ast* elif = NULL;
	if (curr(parser).string != symbol_lcurly)
		body = parse_block(parser, true);
	else body = parse_block(parser, false);
	if (curr(parser).string == keyword_else)
	{
		Ast * else_body = NULL;
		TokEntity next	= next(parser);
		if (next.string != symbol_lcurly)
			else_body	= parse_block(parser, true);
		else else_body	= parse_block(parser, false);
		else_ = malloc(sizeof(Ast));
		else_->type = ElseAST;
		else_->elseast = (ElseAst) {
			.body = else_body
		};
	}
	else if (curr(parser).string == keyword_elif)
	{
		elif = parse_ifexpr(parser);
	}
	Ast *node = malloc(sizeof(Ast));
	node->type = IfAST;
	node->ifast = (IfAst) {
		.condition	= condition,
		.body		= body,
		.else_		= else_,
		.elif		= elif
	};
	return node;
}

Ast *parse_while(Parser *parser)
{
	
	next(parser); // Get To the Condition
	Ast *body = NULL;
	Ast *condition = parse_expr(parser, 0);
	parser->loop = true;
	if (curr(parser).string != symbol_lcurly)
		body = parse_block(parser, true);
	else body = parse_block(parser, false);
	Ast *node = malloc(sizeof(Ast));
	node->type = WhileAST;
	node->whileast = (WhileAst) {
		.condition	= condition,
		.body		= body
	};
	parser->loop = false;
	return node;
}
