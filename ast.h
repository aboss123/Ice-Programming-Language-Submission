#pragma once
#include "tok.h"
#include <llvm-c/Core.h>
#include <llvm-c/DataTypes.h>
#include <llvm-c/Types.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/BitReader.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Object.h>
#include <llvm-c/OrcBindings.h>

/** All Ast Node Types */
typedef enum {
	BinaryAST,
	LiteralAST,
	AssignmentAST,
	CastAST,
	UnaryAST,
	ReturnAST,
	BlockAST,
	IfAST,
	ElseAST,
	ElifAST,
	ForAST,
	WhileAST,
	BreakAST,
	ContinueAST,
	PrintAST,
	InputAST,
	RandAST,
} AstType;


typedef enum {
	StringValue = 10,
	UIntValue = 11,
	IntValue = 12,
	DoubleValue = 13,
	PointerValue = 14
} AstValue;


/** Generic Container Type */
typedef struct Ast Ast;

/** Container for direct literal */
typedef struct {
	TokEntity literal;
} LiteralAst;

typedef struct {
	TokEntity op;
	Ast *expr;
} UnaryAst;

/** Recursive Value Resolving */
typedef struct BinaryAst BinaryAst;
typedef struct BinaryAst {
	Ast			*lhs;
	TokEntity	op;
	Ast			*rhs;
} BinaryAst;

/** Variable Assignment */
typedef struct {
	TokEntity	name;
	Ast			*expr;
} AssignmentAst;

/** Generic Printing */
typedef struct {
	Ast *expr;
} PrintAst;

typedef struct {
	Ast *expr;
} InputAst;

typedef struct {
	Ast **stmts;
} BlockAst;

typedef struct {
	BlockAst *body;
} ElseAst;

typedef struct {
	Ast			*condition;
	BlockAst	*body; 
	ElseAst		*else_;
	Ast			*elif;
} IfAst;

typedef struct {
	Ast			*condition;
	BlockAst	*body;
} WhileAst;

typedef struct {
	Ast *min;
	Ast *max;
} RandAst;

typedef struct Ast {
	AstType type;
	union {
		LiteralAst		literalast;
		UnaryAst		unaryast;
		BinaryAst		binaryast;
		AssignmentAst	assignmentast;
		PrintAst		printast;
		InputAst		inputast;
		BlockAst		blockast;
		ElseAst			elseast;
		IfAst			ifast;
		WhileAst		whileast;
		RandAst			randast;
	};
} Ast;

extern const char * requesting_var;
extern const char * requesting_fn;
LLVMValueRef *variables;
void rnd_str(char *s);

static LLVMValueRef add_input(LLVMModuleRef mod)
{
	LLVMTypeRef input_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef input_ty = LLVMFunctionType (
		LLVMPointerType(LLVMInt8Type(), 0),
		input_args,
		1,
		false
	);

	LLVMValueRef input = __add__("input", LLVMAddFunction(mod, "input", input_ty));
	LLVMValueRef msg = LLVMGetParam(input, 0);
	LLVMSetValueName(msg, "msg");

	LLVMBuilderRef builder = LLVMCreateBuilder();

	LLVMBasicBlockRef entry = LLVMAppendBasicBlock(input, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);

	LLVMValueRef printf_fn = __get__("printf");
	LLVMValueRef printf_args[] = { LLVMBuildGlobalStringPtr(builder, "%s", "%s"), msg };
	LLVMBuildCall (
		builder,
		printf_fn,
		printf_args,
		2,
		""
	);
	

	LLVMValueRef malloc_fn = __get__("malloc");
	LLVMValueRef malloc_args[] = { LLVMConstInt(LLVMInt64Type(), 1, true) };
	LLVMValueRef buf = LLVMBuildCall (
		builder,
		malloc_fn,
		malloc_args,
		1,
		"x"
	);
	LLVMValueRef gets_fn = __get__("gets");
	LLVMValueRef gets_args[] = { buf };
	LLVMValueRef t = LLVMBuildCall(
		builder,
		gets_fn,
		gets_args,
		1,
		""
	); 
	LLVMBuildRet(builder, buf); // Note: do not LLVMBuildLoad
	return input;
}


LLVMValueRef resolve_ast(LLVMExecutionEngineRef engine, LLVMBuilderRef builder, Ast *type);