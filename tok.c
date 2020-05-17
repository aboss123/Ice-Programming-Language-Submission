#include "tok.h"

/** O(1) to_lower */
extern char to_lower[256] = {
	['A'] = 'a', ['B'] = 'b', ['C'] = 'c',
	['D'] = 'd', ['E'] = 'e', ['F'] = 'f',
	['G'] = 'g', ['H'] = 'h', ['I'] = 'i',
	['J'] = 'j', ['K'] = 'k', ['L'] = 'l',
	['M'] = 'm', ['N'] = 'n', ['O'] = 'o',
	['P'] = 'p', ['Q'] = 'q', ['R'] = 'r',
	['S'] = 's', ['T'] = 't', ['U'] = 'u',
	['V'] = 'v', ['W'] = 'w', ['X'] = 'x',
	['Y'] = 'y', ['Z'] = 'z',

	['a'] = 'a',['b'] = 'b',['c'] = 'c',
	['d'] = 'd',['e'] = 'e',['f'] = 'f',
	['g'] = 'g',['h'] = 'h',['i'] = 'i',
	['j'] = 'j',['k'] = 'k',['l'] = 'l',
	['m'] = 'm',['n'] = 'n',['o'] = 'o',
	['p'] = 'p',['q'] = 'q',['r'] = 'r',
	['s'] = 's',['t'] = 't',['u'] = 'u',
	['v'] = 'v',['w'] = 'w',['x'] = 'x',
	['y'] = 'y',['z'] = 'z',
};

/** All parseable escapes */
extern const char * char_escape[256] = {
		['n'] = "\n",
		['r'] = "\r",
		['t'] = "\t",
		['v'] = "\v",
		['b'] = "\b",
		['a'] = "\a",
		['0'] = "\0",
};


extern char char_escape2[256] = {
		['n'] = '\n',
		['r'] = '\r',
		['t'] = '\t',
		['v'] = '\v',
		['b'] = '\b',
		['a'] = '\a',
		['0'] = 0,
};


extern BOOL identifier[256] = {
	['A'] = true,['B'] = true,['C'] = true,
	['D'] = true,['E'] = true,['F'] = true,
	['G'] = true,['H'] = true,['I'] = true,
	['J'] = true,['K'] = true,['L'] = true,
	['M'] = true,['N'] = true,['O'] = true,
	['P'] = true,['Q'] = true,['R'] = true,
	['S'] = true,['T'] = true,['U'] = true,
	['V'] = true,['W'] = true,['X'] = true,
	['Y'] = true,['Z'] = true,

	['a'] = true,['b'] = true,['c'] = true,
	['d'] = true,['e'] = true,['f'] = true,
	['g'] = true,['h'] = true,['i'] = true,
	['j'] = true,['k'] = true,['l'] = true,
	['m'] = true,['n'] = true,['o'] = true,
	['p'] = true,['q'] = true,['r'] = true,
	['s'] = true,['t'] = true,['u'] = true,
	['v'] = true,['w'] = true,['x'] = true,
	['y'] = true,['z'] = true,['_'] = true
};

/** All other registered symbols */
extern BOOL is_symbol[256] = {
	['+'] = true,['-'] = true,['*'] = true,
	['/'] = true,['|'] = true,['&'] = true,
	['^'] = true,['~'] = true,['('] = true,
	[')'] = true,[':'] = true,[';'] = true,
	['='] = true,['!'] = true,['.'] = true,
	['>'] = true,['<'] = true,['#'] = true,
	[','] = true,['%'] = true,[';'] = true,
	['?'] = true,['{'] = true,['}'] = true
};


/** Hexidecimal Chars */
extern uint8_t hex_chars[256] = {
		['0'] = 0,
		['1'] = 1,
		['2'] = 2,
		['3'] = 3,
		['4'] = 4,
		['5'] = 5,
		['6'] = 6,
		['7'] = 7,
		['8'] = 8,
		['9'] = 9,
		['a'] = 10,['A'] = 10,
		['b'] = 11,['B'] = 11,
		['c'] = 12,['C'] = 12,
		['d'] = 13,['D'] = 13,
		['e'] = 14,['E'] = 14,
		['f'] = 15,['F'] = 15,
};


extern const char * keyword_if = "";
extern const char * keyword_else = "";
extern const char * keyword_elif = "";
extern const char * keyword_while = "";
extern const char * keyword_for = "";
extern const char * keyword_var = "";
extern const char * keyword_double = "";
extern const char * keyword_int = "";
extern const char * keyword_bool = "";
extern const char * keyword_break = "";
extern const char * keyword_continue = "";
extern const char * keyword_return = "";
extern const char * keyword_and = "";
extern const char * keyword_or = "";
extern const char * keyword_print = "";
extern const char * keyword_input = "";
extern const char * keyword_rand = "";
extern const char * keyword_true = "";
extern const char * keyword_false = "";

/** Symbol Pointer Comparison */
extern const char * symbol_plus = "";
extern const char * symbol_minus = "";
extern const char * symbol_div = "";
extern const char * symbol_star = "";
extern const char * symbol_mod = "";
extern const char * symbol_lparen = "";
extern const char * symbol_rparen = "";
extern const char * symbol_colon = "";
extern const char * symbol_semicolon = "";
extern const char * symbol_hash = "";
extern const char * symbol_not = "";
extern const char * symbol_bitwiseand = "";
extern const char * symbol_bitwisexor = "";
extern const char * symbol_bitwiseor = "";
extern const char * symbol_bitwisenot = "";
extern const char * symbol_gtr = "";
extern const char * symbol_less = "";
extern const char * symbol_equal = "";
extern const char * symbol_plusplus = "";
extern const char * symbol_minusminus = "";
extern const char * symbol_lshift = "";
extern const char * symbol_rshift = "";
extern const char * symbol_gtrorequal = "";
extern const char * symbol_lessorequal = "";
extern const char * symbol_equalequal = "";
extern const char * symbol_plusequal = "";
extern const char * symbol_minusequal = "";
extern const char * symbol_notequal = "";
extern const char * symbol_divequal = "";
extern const char * symbol_timesequal = "";
extern const char * symbol_bitwiseandequal = "";
extern const char * symbol_bitwiseorequal = "";
extern const char * symbol_bitwisexorequal = "";
extern const char * symbol_lshiftequal = "";
extern const char * symbol_rshiftequal = "";
extern const char * symbol_colonequal = "";
extern const char * symbol_lcurly = "";
extern const char * symbol_rcurly = "";
extern const char * symbol_comma = "";

#define KEYWORD(x) keyword_##x = __salloc__(#x), ((size_t*)(void*)(keyword_##x) - 1)[0] = 1
#define SYMBOL(x, y) symbol_##x = __salloc__(y), ((size_t*)(void*)(symbol_##x) - 1)[0] = 2
void __initreserved__()
{
	KEYWORD(if);
	KEYWORD(else);
	KEYWORD(elif);
	KEYWORD(while);
	KEYWORD(for);
	KEYWORD(int);
	KEYWORD(double);
	KEYWORD(bool);
	KEYWORD(var);
	KEYWORD(break);
	KEYWORD(continue);
	KEYWORD(and);
	KEYWORD(or);
	KEYWORD(print);
	KEYWORD(rand);
	KEYWORD(input);
	KEYWORD(true);
	KEYWORD(false);

	SYMBOL(plus, "+");
	SYMBOL(minus, "-");
	SYMBOL(div, "/");
	SYMBOL(star, "*");
	SYMBOL(mod, "%");
	SYMBOL(lparen, "(");
	SYMBOL(rparen, ")");
	SYMBOL(colon, ":");
	SYMBOL(semicolon, ";");
	SYMBOL(hash, "#");
	SYMBOL(not, "!");
	SYMBOL(bitwiseand, "&");
	SYMBOL(bitwiseor, "|");
	SYMBOL(bitwisexor, "^");
	SYMBOL(bitwisenot, "~");
	SYMBOL(gtr, ">");
	SYMBOL(less, "<");
	SYMBOL(equal, "=");
	SYMBOL(plusplus, "++");
	SYMBOL(minusminus, "--");
	SYMBOL(lshift, "<<");
	SYMBOL(rshift, ">>");
	SYMBOL(gtrorequal, ">=");
	SYMBOL(lessorequal, "<=");
	SYMBOL(equalequal, "==");
	SYMBOL(plusequal, "+=");
	SYMBOL(minusequal, "-=");
	SYMBOL(notequal, "!=");
	SYMBOL(timesequal, "*=");
	SYMBOL(divequal, "/=");
	SYMBOL(bitwiseandequal, "&=");
	SYMBOL(bitwiseorequal, "|=");
	SYMBOL(bitwisexorequal, "^=");
	SYMBOL(lshiftequal, "<<=");
	SYMBOL(rshiftequal, ">>=");
	SYMBOL(colonequal, ":=");
	SYMBOL(lcurly, "{");
	SYMBOL(rcurly, "}");
	SYMBOL(comma, ",");
}

BOOL is_keyword(const char *s)
{
	return ((size_t*)(void*)(s)-1)[0] == 1;
}

BOOL is_valid_symbol(const char *s)
{
	return ((size_t*)(void*)(s)-1)[0] == 2;
}

Tokenizer __tokenizer__()
{
	Tokenizer tokenizer;
	tokenizer.tokens		= NULL;
	tokenizer.data.lines	= NULL;
	tokenizer.data.line_c	= 1;
	return tokenizer;
}

#define error(x) ice_error(tokenizer->data.line_c, start_col, (int)(stream - line), line, x)
#define next() (stream++, *stream)
void __readtok__(Tokenizer *tokenizer, const char *stream) 
{
	// Begin The Lexing Process Using Recursive Goto Statements
	// This will allow for one method overhead function call
	BOOL		zero = false;
	TokEntity	token;
	const char *line = stream;
	goto begin;
	begin:
		if (null(*stream))
		{
			token = (TokEntity) {
				.line	= tokenizer->data.line_c,
				.col	= (int)(stream - line),
				.type	= EOFToken,
				.string		= "\0"
			};
			vec_push(tokenizer->tokens, token);

			// Handle Last Line
			const char* end = stream;
			const char* l = __sallocrange__(line, stream);

			tokenizer->data.line_c++;

			vec_push(tokenizer->data.lines, l);
			return;
		}
		else if (whitespace(*stream) || *stream == '\t')	goto whitespace;
		else if (identifier[*stream])						goto identifier;
		else if (*stream == '\n')							goto lines;
		else if (*stream == '"')							goto string;
		else if (equ(*stream, '.'))							goto fp;
		else if (equ(*stream, '0'))							{zero = true; goto fp;}
		else if (digit(*stream))							goto numbers;
		else if (*stream == '\\')							goto escape;
		else goto symbols;
	lines:
	{
		stream++;
		const char *end = stream;
		const char *l = __sallocrange__(line, stream);

		tokenizer->data.line_c++;
		line = stream;
		vec_push(tokenizer->data.lines, l);
		goto begin;
	}
	whitespace:
	{
		while (whitespace(*stream) || *stream == '\t') stream++;
		goto begin;
	}
	identifier:
	{
		int start_col		= (int)(stream - line);
		const char *start	= stream;
		while(identifier[*stream]) stream++;
		token = (TokEntity){
			.line	= tokenizer->data.line_c,
			.col	= (int)(stream - line),
			.type	= Identifier,
			.string = __sallocrange__(start, stream)
		};
		vec_push(tokenizer->tokens, token);
		goto begin;
	}
	string:
	{
		stream++;
		int start_col = (int)(stream - line);
		char *start = stream;
		while (*stream != '"' && *stream != '\0') stream++;
		size_t len = stream - start;
		char *s_tok = __sallocrange__(start, stream);
		stream++;
		size_t pos = strcspn(s_tok, "\\");
		while (pos != 0 && pos < len - 1)
		{
			memmove(&*(s_tok + pos), &*((s_tok + pos + 1)), len * sizeof(*s_tok));
			*(s_tok + pos) = char_escape2[*(s_tok + pos)];
			pos = strcspn(s_tok, "\\");
			len--;
		}
		token = (TokEntity){
			.line	= tokenizer->data.line_c,
			.col	= (int)(stream - line),
			.type	= StringLiteral,
			.string = s_tok
		};
		vec_push(tokenizer->tokens, token);
		goto begin;
	}
	escape:
	{
		int start_col		= (int)(stream - line);
		stream++;
		if (!char_escape[*stream] && *stream != '0')
			error("Invalid Escape Character");
		const char *s_tok = __salloc__(char_escape[*stream]);
		token = (TokEntity){
			.line = tokenizer->data.line_c,
			.col = (int)(stream - line),
			.type = StringLiteral,
			.string = s_tok
		};
		vec_push(tokenizer->tokens, token);
		goto begin;
	}
	numbers: 
	{
		int start_col		= (int)(stream - line);
		const char *start	= stream;
		BOOL hex = false;

		// Hexadecimal Properties
		if (equ(*stream, '0'))
		{
			char c =  next();
			if (to_lower[c] == 'x')
			{
				hex = true;
				stream++;
				goto create_int;
			}
			else if (digit(c))
				error("Numerical digit cannot follow after 0");
			else
			{
				token = (TokEntity){
					.line	= tokenizer->data.line_c,
					.col	= (int)(stream - line),
					.type	= Int64,
					.int_v	= 0
				};
				vec_push(tokenizer->tokens, token);
				goto begin;
			}
		}
		goto create_int;
		create_int:
		{
			while (digit(*stream) || (hex && hex_chars[*stream]))
			{
				if (hex_chars[*stream] == 0 && *stream != '0' && *stream != '\0')
					error("Unexpected char for base!");
				stream++;
			}
			if (*stream == '.')
			{
				stream -= (stream - start);
				goto fp;
			}

			const char* s_tok = __sallocrange__(start, stream);
			int i;
			sscanf_s(s_tok, "%i", &i);
			token = (TokEntity){
				.line = tokenizer->data.line_c,
				.col = (int)(stream - line),
				.type = Int64,
				.int_v = i
			};
			vec_push(tokenizer->tokens, token);
			goto begin;
		}
	}
	fp:
	{
		int start_col = (int)(stream - line);
		const char *start  = stream;
		if (next() == '.')
		{
			if (zero)
			{
				stream++;
				goto create_float;
			}
			char a = next();
			if (a == '.')
			{
				token = (TokEntity) {
					.line	= tokenizer->data.line_c,
					.col	= (int)(stream - line),
					.type	= Ellipsis
					// .string = __salloc__("...")
				};
				vec_push(tokenizer->tokens, token);
				goto begin;
			}
			else error("Unexpected '.' in token");
		}
		if (zero) { stream--; goto numbers; } // Checks for example a case where floating point is 0.2 instead of .2 otherwise it gets processed the numbers label
		goto create_float;
		create_float:
		{
			int e = 0;
			int o = 0;
			while (digit(*stream) || *stream == '.')
			{
				if (equ(stream, 'e')) e++;
				if (equ(stream, '.')) o++;
				if (e > 1) error("Unexpected 'e' in creating floating point token!");
				if (o > 1) error ("Unexpected '. in creating floating point token!");
				stream++;
			}
			const char* s_tok = __sallocrange__(start, stream);
			double d;
			int c = sscanf_s(s_tok, "%lf", &d);
			if (c == EOF) error("Unexpected EOF!");
			token = (TokEntity){
				.line = tokenizer->data.line_c,
				.col = (int)(stream - line),
				.type = Double,
				.fp = d
			};
			zero = false;
			vec_push(tokenizer->tokens, token);
			goto begin;
		}
	}
	symbols:
	{
		int start_col		= (int)(stream - line);
		const char *start	= stream;
		while (is_symbol[*stream]) 
		{ 
			if (*stream == 0)
				error("Unexpected Symbol");
			stream++; 
		}
		char *s_tok = __sallocrange__(start, stream);
		size_t len = stream - start + 1;
		if (len > 3)
			error("Invalid Token Creation");
		int c = 0;
		goto check;
		check:
		{
			
			if (!is_valid_symbol(s_tok))
			{
				if (c == len) 
					error("Invalid Token Creation");
				stream --;
				c++;
				s_tok = __sallocrange__(start, stream);
				goto check;
			}
			goto create_token;
		}

		create_token:
		{
			s_tok = __sallocrange__(start, stream);
			token = (TokEntity){
					.line = tokenizer->data.line_c,
					.col = (int)(stream - line),
					.type = Symbol,
					.string = s_tok
			};
			vec_push(tokenizer->tokens, token);
			goto begin;
		}
	}
}