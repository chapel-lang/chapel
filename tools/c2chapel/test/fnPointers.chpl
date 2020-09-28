// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "fnPointers.h";

// Note: Generated with fake std headers

use CPtr;
extern proc fnPointerArgs(callback : c_fn_ptr, msg : c_string) : c_int;

extern proc foo(a : myFunctionPointer, b : c_int) : c_int;

// ==== c2chapel typedefs ====

extern record io_methods {
  var close : c_fn_ptr;
  var open : c_fn_ptr;
}

extern type myFunctionPointer = c_fn_ptr;

