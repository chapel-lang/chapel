// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "fileGlobals.h";

// Note: Generated with fake std headers

use CPtr;
extern var globalInt : c_int;

extern var globalChar : c_char;

extern var globalPointer : c_ptr(c_int);

extern record mytype {
  var a : c_int;
  var b : c_char;
  var c : c_void_ptr;
}

extern var globalStruct : mytype;

