#include "ast.h"

/** NOTES: 
	alloca for local variables 
	LLVMBuild... to build IR
	LLVMAdd for compile time evaluation
*/

extern const char * requesting_var = "";
extern const char * requesting_fn = "main";
LLVMValueRef* variables = NULL;
#define RND_STR_MAX_LEN	4095
void rnd_str(char *src)
{
	int c, z = 0;
	srand(RND_STR_MAX_LEN); // Putting in a seed
	src = malloc(RND_STR_MAX_LEN + 1); // Allocating for string
	for (int i = 0; i < RND_STR_MAX_LEN; i++)
		if ((c = rand()) <= 'A' && c >= 'Z')
			src[z++] = c;
}

LLVMValueRef resolve_literal(LLVMBuilderRef builder, Ast *ast)
{
	LiteralAst lit = ast->literalast;
	if (lit.literal.type == StringLiteral)
	{
		//return LLVMConstString(lit.literal.string, strlen(lit.literal.string), false);
		LLVMValueRef value = LLVMBuildGlobalStringPtr(builder, lit.literal.string, lit.literal.string);
		encode(AstValue, StringValue, value, 1);
		encode(char*, lit.literal.string, value, 2);
		return value;
	}
	else if (lit.literal.type == Int64)
	{
		LLVMValueRef value = LLVMConstInt(LLVMInt32Type(), lit.literal.int_v, true);
		encode(AstValue, IntValue, value, 1);
		return value;
	}
	else if (lit.literal.type == Double)
	{
		LLVMValueRef value = LLVMConstReal(LLVMDoubleType(), lit.literal.fp);
		encode(AstValue, DoubleValue, value, 1);
		return value;
	}
	else if (lit.literal.string == keyword_true)
	{
		LLVMValueRef val = LLVMConstInt(LLVMInt1Type(), true, false);
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (lit.literal.string == keyword_false)
	{
		LLVMValueRef val = LLVMConstInt(LLVMInt1Type(), false, false);
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (lit.literal.type == Identifier)
	{
		LLVMValueRef var = __get__(lit.literal.string);
		LLVMValueRef v = LLVMBuildLoad(builder, var, lit.literal.string);
		encode(AstValue, decode(AstValue, var, 1), v, 1);
		return v;
	}
}

LLVMValueRef resolve_binaryexpr(LLVMExecutionEngineRef engine, LLVMBuilderRef builder, Ast *ast)
{
	BinaryAst binaryast = ast->binaryast;
	if (binaryast.op.string == symbol_plus)
	{
		BOOL fp = false;
		// Both Sides of Expression
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		// Compile Time String evaluation
		if (lhstype == StringValue && rhstype == StringValue)
		{
			char* a = decode(char*, lhs, 2);
			char* b = decode(char*, rhs, 2);

			/**size_t len = strlen(a) + strlen(b) + 1;
			char* cc = malloc(len);
			if (!cc) { fatal("malloc failed on string allocation of strings: %s and %s", a, b); return NULL; }
			sprintf_s(cc, len, "%s%s", a, b);
			LLVMValueRef cat = LLVMBuildGlobalStringPtr(builder, cc, cc);
			encode(AstValue, StringValue, cat, 1);
			encode(char*, cc, cat, 2);
			return cat; */
			LLVMValueRef malloc_fn = __get__("malloc");
			LLVMValueRef malloc_a[] = { LLVMConstInt(LLVMInt64Type(), 11, true) };
			LLVMValueRef malloc_f = LLVMBuildCall(
				builder,
				malloc_fn,
				malloc_a,
				1,
				""
			);
			LLVMValueRef sprintf_fn = __get__("sprintf");
			LLVMValueRef args[] = { malloc_f, LLVMBuildGlobalStringPtr(builder, "%s%s", ""),  lhs, rhs };
			LLVMValueRef v = LLVMBuildCall(
				builder,
				sprintf_fn,
				args,
				4,
				"sprintf"
			); 
			encode(AstValue, StringValue, malloc_f, 1); 
			return malloc_f; 
		}
		else if (lhstype == StringValue || rhstype == StringValue)
			warning("Attempting to add a string value to an non string value, may end up in a seg fault");
		else if (lhstype == DoubleValue || rhstype == DoubleValue)
			fp = true;
		LLVMValueRef val;
		if (fp)
		{
			val = LLVMBuildFAdd(builder, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
			encode(AstValue, DoubleValue, val, 1);
		}
		else
		{
			val = LLVMBuildAdd(builder, lhs, rhs, "");
			encode(AstValue, IntValue, val, 1);
		}
		return val;
	}
	else if (binaryast.op.string == symbol_minus)
	{
		BOOL fp = false;
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		if (lhstype == DoubleValue || rhstype == DoubleValue) fp = true; // Modifies AstValue
		else if (lhstype == StringValue || rhstype == StringValue)
		{
			// Fatal!
		}
		LLVMValueRef val;
		if (fp)
		{
			val = LLVMBuildFSub(builder, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
			encode(AstValue, DoubleValue, val, 1);
		}
		else
		{
			val = LLVMBuildSub(builder, lhs, rhs, "");
			encode(AstValue, IntValue, val, 1);
		}
		return val;
	}
	else if (binaryast.op.string == symbol_star)
	{
		BOOL fp = false;
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		if (lhstype == DoubleValue || rhstype == DoubleValue) fp = true; // Modifies AstValue
		else if (lhstype == StringValue || rhstype == StringValue)
		{
			// Fatal!
		}
		LLVMValueRef val;
		if (fp)
		{
			val = LLVMBuildFMul(builder, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
			encode(AstValue, DoubleValue, val, 1);
		}
		else
		{
			val = LLVMBuildMul(builder, lhs, rhs, "");
			encode(AstValue, IntValue, val, 1);
		}
		return val;
	}
	else if (binaryast.op.string == symbol_div)
	{
		BOOL fp = false;
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		if (lhstype == DoubleValue || rhstype == DoubleValue) fp = true; // Modifies AstValue
		else if (lhstype == StringValue || rhstype == StringValue)
		{
			// Fatal!
		}
		LLVMValueRef val;
		if (fp)
		{
			val = LLVMBuildFDiv(builder, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
			encode(AstValue, DoubleValue, val, 1);
		}
		else
		{
			val = LLVMBuildSDiv(builder, lhs, rhs, "");
			encode(AstValue, IntValue, val, 1);
		}
		return val;
	}
	else if (binaryast.op.string == symbol_equalequal)
	{
	// Make this a function for string comparison or use strcmp
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		if (lhstype == StringValue && rhstype == StringValue)
		{
			LLVMValueRef strcmp_fn = __get__("strcmp");
			LLVMValueRef strcmp_args[] = { lhs, rhs };
			LLVMValueRef cmp = LLVMBuildCall (
				builder,
				strcmp_fn,
				strcmp_args,
				2,
				""
			);
			LLVMValueRef val = LLVMBuildICmp(builder, LLVMIntEQ, cmp, LLVMConstInt(LLVMInt32Type(), 0, false), "");
			encode(AstValue, IntValue, cmp, 1);
			return val;
		}

		LLVMValueRef val = LLVMBuildFCmp(builder, LLVMRealOEQ, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == symbol_notequal)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		LLVMValueRef val = LLVMBuildFCmp(builder, LLVMRealOGT, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == symbol_gtr)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);


		LLVMValueRef val = LLVMBuildFCmp(builder, LLVMRealOGT, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == symbol_gtrorequal)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		LLVMValueRef val = LLVMBuildFCmp(builder, LLVMRealOGE, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == symbol_less)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		LLVMValueRef val = LLVMBuildFCmp(builder, LLVMRealOLT, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == symbol_lessorequal)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		LLVMValueRef val = LLVMBuildFCmp(builder, LLVMRealOLE, LLVMBuildCast(builder, LLVMSIToFP, lhs, LLVMDoubleType(), ""), LLVMBuildCast(builder, LLVMSIToFP, rhs, LLVMDoubleType(), ""), "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == keyword_and)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		LLVMValueRef val = LLVMBuildBinOp(builder, LLVMAnd, lhs, rhs, "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
	else if (binaryast.op.string == keyword_or)
	{
		LLVMValueRef lhs = resolve_ast(engine, builder, binaryast.lhs);
		AstValue lhstype = decode(AstValue, lhs, 1);

		LLVMValueRef rhs = resolve_ast(engine, builder, binaryast.rhs);
		AstValue rhstype = decode(AstValue, rhs, 1);

		LLVMValueRef val = LLVMBuildBinOp(builder, LLVMOr, lhs, rhs, "");
		encode(AstValue, IntValue, val, 1);
		return val;
	}
}

LLVMValueRef resolve_ifexpr(LLVMExecutionEngineRef engine, LLVMBuilderRef builder, Ast *ast)
{
	IfAst ifast = ast->ifast;
	LLVMValueRef cond = resolve_ast(engine, builder, ifast.condition);

	// Function for If Statement
	LLVMValueRef fn = __get__(requesting_fn);

	// True If Block
	LLVMBasicBlockRef true_b = LLVMAppendBasicBlock(fn, "");
	LLVMBasicBlockRef else_b = NULL;
	LLVMBasicBlockRef elif_b = NULL;
	LLVMBasicBlockRef end = LLVMAppendBasicBlock(fn, "");
	if (ifast.else_ == NULL && ifast.elif == NULL) LLVMBuildCondBr(builder, cond, true_b, end); // NOTE: This line must be placed here!
	else if (ifast.else_ == NULL)
	{
		elif_b = LLVMAppendBasicBlock(fn, "");
		LLVMBuildCondBr(builder, cond, true_b, elif_b);
	}
	else if (ifast.elif == NULL)
	{
		else_b = LLVMAppendBasicBlock(fn, "");
		LLVMBuildCondBr(builder, cond, true_b, else_b);
	}

	LLVMBuilderRef	  if_builder = LLVMCreateBuilder();
	LLVMBuilderRef	  else_builder = LLVMCreateBuilder();
	LLVMBuilderRef	  elif_builder = LLVMCreateBuilder();
	LLVMPositionBuilderAtEnd(if_builder, true_b);
	LLVMPositionBuilderAtEnd(else_builder, else_b);
	LLVMPositionBuilderAtEnd(elif_builder, elif_b);
	LLVMPositionBuilderAtEnd(builder, end);

	resolve_ast(engine, if_builder, ifast.body);
	LLVMBuildBr(if_builder, end);

	if (ifast.elif != NULL)
	{
		resolve_ast(engine, elif_builder, ifast.elif);
		LLVMBuildBr(elif_builder, end);
	}

	if (ifast.else_ != NULL)
	{
		resolve_ast(engine, else_builder, ifast.else_);
		LLVMBuildBr(else_builder, end);
	}

	return NULL;
}

LLVMValueRef resolve_ast(LLVMExecutionEngineRef engine, LLVMBuilderRef builder, Ast *ast)
{
	switch (ast->type)
	{
		case LiteralAST:
			return resolve_literal(builder, ast);
		case BinaryAST: 
			return resolve_binaryexpr(engine, builder, ast);
		case AssignmentAST:
		{
			AssignmentAst assignmentast = ast->assignmentast;
			LLVMValueRef var;
			LLVMValueRef binary = resolve_ast(engine, builder, assignmentast.expr);
			if (__find__(assignmentast.name.string))
			{
				var = __get__(assignmentast.name.string);
				LLVMBuildStore(builder, binary, var);
				return var;
			}
			else
			{
				var = LLVMBuildAlloca(builder, LLVMTypeOf(binary), assignmentast.name.string);
				LLVMBuildStore(builder, binary, var);
				encode(AstValue, decode(AstValue, binary, 1), var, 1);
			}
			return __add__(assignmentast.name.string, var);
		}
		case PrintAST:
		{
			PrintAst printast = ast->printast;
			LLVMValueRef expr = resolve_ast(engine, builder, printast.expr);
			size_t args_len = 2;
			LLVMValueRef fmt = NULL;

			if (LLVMGetTypeKind(LLVMTypeOf(expr)) == LLVMIntegerTypeKind)
			{
				fmt = LLVMBuildGlobalStringPtr(builder, "%i", "%i");
				expr = LLVMBuildIntCast2(builder, expr, LLVMInt32Type(), false, "");
			}
			else if (LLVMGetTypeKind(LLVMTypeOf(expr)) == LLVMDoubleTypeKind)
				fmt = LLVMBuildGlobalStringPtr(builder, "%f", "%f");
			else fmt = LLVMBuildGlobalStringPtr(builder, "%s", "%s");

			LLVMValueRef args[] = { fmt, expr};
			LLVMValueRef printf_fn = __get__("printf");
			printf("%p\n", printf_fn);
			return LLVMBuildCall(
				builder,
				printf_fn,
				args,
				args_len,
				""
			);
		}
		case InputAST:
		{
			InputAst inputast = ast->inputast;
			LLVMValueRef expr = resolve_ast(engine, builder, inputast.expr);

			LLVMValueRef input_fn = __get__("input");
			LLVMValueRef args[] = { LLVMBuildGlobalStringPtr(builder, decode(char*, expr, 2), decode(char*, expr, 2)) };
			LLVMValueRef input = LLVMBuildCall (
				builder,
				input_fn,
				args,
				1,
				""
			);
			encode(AstValue, StringValue, input, 1);
			return input;
		}
		case BlockAST: 
		{
			BlockAst blockast = ast->blockast;
			if (blockast.stmts == NULL) return NULL;
			for (int i = 0; i < vec_size(blockast.stmts); i++)
				resolve_ast(engine, builder, blockast.stmts[i]);
			return NULL;
		}
		case ElseAST:
		{
			ElseAst elseast = ast->elseast;
			resolve_ast(engine, builder, elseast.body);
			return NULL;
		}
		case IfAST:
			return resolve_ifexpr(engine, builder, ast);
		case WhileAST:
		{
			WhileAst whileast = ast->whileast;
			LLVMValueRef cond = resolve_ast(engine, builder, whileast.condition);

			LLVMValueRef fn = __get__(requesting_fn);

			LLVMBuilderRef b = LLVMCreateBuilder();
			LLVMBasicBlockRef  true_b = LLVMAppendBasicBlock(fn, "");
			LLVMBasicBlockRef end = LLVMAppendBasicBlock(fn, "");
			LLVMBuildCondBr(builder, cond, true_b, end);		
			LLVMPositionBuilderAtEnd(b, true_b);
			LLVMPositionBuilderAtEnd(builder, end);
			
			
			resolve_ast(engine, b, whileast.body);
			cond = resolve_ast(engine, b, whileast.condition);
			LLVMBuildCondBr(b, cond, true_b, end);
			return NULL;
		}
		case RandAST: 
		{
			RandAst randast = ast->randast;
			LLVMValueRef min = resolve_ast(engine, builder, randast.min);
			AstValue lhstype = decode(AstValue, min, 1);

			LLVMValueRef max = resolve_ast(engine, builder, randast.max);
			AstValue rhstype = decode(AstValue, max, 1);

			LLVMValueRef seed = LLVMConstInt(LLVMInt32Type(), time(0), false);

			LLVMValueRef srand_fn = __get__("srand");
			LLVMValueRef srand_args[] = { seed };
			LLVMBuildCall (
				builder,
				srand_fn,
				srand_args,
				1,
				""		
			);
			LLVMValueRef rand_fn = __get__("rand");
			//LLVMValueRef rand_args[] = {LLVMVo };
			LLVMValueRef r = LLVMBuildCall(
				builder,
				rand_fn,
				NULL,
				0,
				""
			);
			LLVMValueRef upl = LLVMBuildSub(builder, max, min, "");
			LLVMValueRef upi = LLVMBuildAdd(builder, upl, LLVMConstInt(LLVMInt32Type(), 1, true), "");
			LLVMValueRef upm = LLVMBuildSRem(builder, r, upi, "");
			LLVMValueRef upf = LLVMBuildAdd(builder, upm, min, "");
			encode(AstValue, IntValue, upf, 1);
			return upf;
 		}
		default:
			break;
	} 
}