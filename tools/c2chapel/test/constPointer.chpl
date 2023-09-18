// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "constPointer.h";

// Note: Generated with fake std headers

use CTypes;
extern proc foo(const ref x : c_int) : c_ptrConst(c_int);

extern proc foo(x : c_ptrConst(c_int)) : c_ptrConst(c_int);

