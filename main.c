#include <stdio.h>
#include "tok.h"
#include "error.h"
#include "parser.h"

void __main__(int argc, char** argv, const char * const envp, const char *m_name, Ast **stmts)
{
	static LLVMBuilderRef	builder;
	static LLVMModuleRef	mod;
	builder = LLVMCreateBuilder();
	mod		= LLVMModuleCreateWithName(m_name);
	/**LLVMMemoryBufferRef mem;
	LLVMCreateMemoryBufferWithContentsOfFile("test.bc", &mem, NULL);
	LLVMModuleRef mod;
	LLVMParseBitcode2(mem, &mod);*/
	char* error = NULL;

	// Declare Malloc Function
	LLVMTypeRef malloc_args[] = { LLVMInt64Type() };
	LLVMTypeRef mallocty = LLVMFunctionType (
		LLVMPointerType(LLVMInt8Type(), 0),
		malloc_args,
		1,
		false
	);
	LLVMValueRef malloc_fn = __add__("malloc", LLVMAddFunction(mod, "malloc", mallocty));

	// Declare PrintF Function
	LLVMTypeRef printf_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef printfty = LLVMFunctionType(
		LLVMInt32Type(),
		printf_args,
		0,
		true // VarArgs ...
	);
	LLVMValueRef printf_fn = __add__("printf", LLVMAddFunction(mod, "printf", printfty)); 

	// Declare StrCat Function
	LLVMTypeRef strcat_args[] = { LLVMPointerType(LLVMInt8Type(), 0),  LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef strcat = LLVMFunctionType(
		LLVMPointerType(LLVMInt8Type(), 0),
		strcat_args,
		2,
		false
	);
	__add__("strcat", LLVMAddFunction(mod, "strcat", strcat));

	LLVMTypeRef strcmp_args[] = { LLVMPointerType(LLVMInt8Type(), 0),  LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef strcmp = LLVMFunctionType(
		LLVMInt32Type(),
		strcmp_args,
		2,
		false
	);
	__add__("strcmp", LLVMAddFunction(mod, "strcmp", strcmp));


	// Declare Gets Function
	LLVMTypeRef gets_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef gets = LLVMFunctionType (
		LLVMPointerType(LLVMInt8Type(), 0),
		gets_args,
		1,
		false
	);
	LLVMValueRef get = __add__("gets", LLVMAddFunction(mod, "gets", gets));
	LLVMValueRef input_fn = add_input(mod);

	LLVMTypeRef srand_args[] = { LLVMInt32Type()};
	LLVMTypeRef srandty = LLVMFunctionType(
		LLVMVoidType(),
		srand_args,
		1,
		false
	);
	LLVMValueRef srand = __add__("srand", LLVMAddFunction(mod, "srand", srandty));

	LLVMTypeRef rand_args[] = { LLVMVoidType() };
	LLVMTypeRef randty = LLVMFunctionType(
		LLVMInt32Type(),
		rand_args,
		0,
		false
	);
	LLVMValueRef rand = __add__("rand", LLVMAddFunction(mod, "rand", randty));
	

	LLVMTypeRef sprintf_args[] = { LLVMPointerType(LLVMInt8Type(), 0),  LLVMPointerType(LLVMInt8Type(), 0) };
	LLVMTypeRef sprintfty = LLVMFunctionType(
		LLVMInt32Type(),
		sprintf_args,
		0,
		true // VarArgs ...
	);

	LLVMValueRef sprintf_fn = __add__("sprintf", LLVMAddFunction(mod, "sprintf", sprintfty));

	LLVMExecutionEngineRef engine;
	LLVMCreateExecutionEngineForModule(&engine, mod, &error);

	LLVMTypeRef main_type		= LLVMFunctionType (
		LLVMVoidType(),
		NULL,
		0,
		false
	);
	LLVMValueRef main_function	= __add__("main", LLVMAddFunction(mod, "main", main_type));
	LLVMBasicBlockRef block		= LLVMAppendBasicBlock(main_function, "entry");
	LLVMPositionBuilderAtEnd(builder, block);


	for (int i = 0; i < vec_size(stmts); i++)
		resolve_ast(engine, builder, stmts[i]); 

	LLVMBuildRetVoid(builder);
	printf("Module: %s", LLVMPrintModuleToString(mod));
	//LLVMPrintModuleToFile(mod, "mains.ll", &error);
	LLVMWriteBitcodeToFile(mod, "main.bc");
	system("lli main.bc");
}

char *read_file(const char *file)
{
	// Create and Open File
	FILE *f = NULL;
	fopen_s(&f, file, "r");

	// Null Check 
	if (!f)
	{
		fatal("File could not be opened");
		return EXIT_FAILURE;
	}

	// Get File Length
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);

	// Create Buffer
	char* buf = calloc(len, 1);
	if (buf)
		fread(buf, 1, len, f);
	fclose(f);
	buf[len] = '\0';
	return buf;
}

int main(int argc, char** argv, char* const* envp) 
{
	
	// Init Reserved Tokens
	__initreserved__();

	// Create a parser instance
	Parser parser = __parser__();

	// Read Whole File
	char *buf = read_file("main.ice");

	// Read all tokens
	__readtok__(&parser.tokenizer, buf);

	// Run Code
	__main__(argc, argv, envp, "mod", __parse__(&parser));

	// Free all allocated
	__sfree__(); 
}