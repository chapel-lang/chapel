// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "fnints.h";

// Note: Generated with fake std headers

use CPtr;
extern proc firstFunc(a : c_int, b : c_int, c : c_int) : void;

extern proc secondFunc(a : c_int, ref b : c_int, ref c : c_ptr(c_int)) : void;

extern proc secondFunc(a : c_int, b : c_ptr(c_int), c : c_ptr(c_ptr(c_int))) : void;

extern proc thirdFunc(a : c_int, b : c_int) : c_int;

extern proc fourthFunc(a : c_int, b : c_int, c : c_int) : void;

extern proc fifthFunc(arg0 : c_int, arg1 : c_int, arg2 : c_int) : void;

extern proc noArgs() : c_int;

extern proc voidArgs() : c_int;

