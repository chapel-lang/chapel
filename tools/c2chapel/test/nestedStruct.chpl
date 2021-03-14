// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "nestedStruct.h";

// Note: Generated with fake std headers

use CPtr;
extern record first {
  var a : c_int;
  var b : c_string;
}

extern record second {
  var a : c_ptr(c_int);
  var b : c_ptr(c_int);
}

extern record Outer {
  var structField : first;
  var fieldPtr : c_ptr(second);
}

