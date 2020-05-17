/** ------------------------------- LibMLH.cpp -------------------------------
* Copyright (c) Ashish Bailkeri 2020 -- Work completed at a MLH Hackathon
* View License information at LICENSE.txt.
*** ------------------------------- LibMLH.cpp -------------------------------
*/

#include "pch.h"

extern "C" static bool cleanup_symbols;

static std::set<std::string> interns; 

static std::set<void*>				ptrs;
static std::map<std::string, void*> nodes;

/** Interns the string using hidden C++ methods that will not be visible in C */
template <class ... Args> // Allows multiple entries
auto intern(Args ... args) -> const char*
{
	// Sets can be filtered without duplicated strings.
	if (cleanup_symbols) { interns.clear(); return ""; }
	return (*interns.emplace(args...).first).c_str(); // Yes, the parenthesis matter, it gets a const std::string then gets const char *
}

auto add_node(std::string key, void *p) -> void *
{
	nodes.emplace(key, (*ptrs.emplace(p).first));
	return p;
}

auto get_node(std::string key) -> void*
{
	auto p = nodes.at(key);
	return (*ptrs.emplace(p).first);
}


/** --------------------------------------------- C - Interface --------------------------------------------- */

extern "C" bool __strequ__(const char *a, const char *b)
{
	return std::string(a) == std::string(b);
}

extern "C" const char *__strcat__(const char *a, const char *b)
{
	return (std::string(a) + std::string(b)).c_str();
}


extern "C" void *__add__(const char *key, void *p)
{
	return add_node(key, p);
}

extern "C" void* __get__(const char* key)
{
	return get_node(key);
}

extern "C" bool __find__(const char *key)
{
	return nodes.find(key) != nodes.end();
}


extern "C" void __sfree__()
{
	cleanup_symbols = true; // Allows for symbol freeing to get rid of all allocating strings
	intern(""); // Force cleanup
}

extern "C" const char* __salloc__(const char* a)
{
	return intern(a);
}

extern "C" const char* __sallocrange__(const char* start, const char* end)
{
	return intern(start, end);
}
