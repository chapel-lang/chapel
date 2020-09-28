// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "chapelVarargs.h";

// Note: Generated with fake std headers

use CPtr;
extern proc printf_wrapper(format : c_string, c__varargs ...) : void;

// Overload for empty varargs
extern proc printf_wrapper(format : c_string) : void;

extern proc multiArgs(a : c_int, b : c_char, c__varargs ...) : c_int;

// Overload for empty varargs
extern proc multiArgs(a : c_int,b : c_char) : c_int;

