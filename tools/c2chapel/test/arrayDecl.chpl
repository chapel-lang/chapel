// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "arrayDecl.h";

// Note: Generated with fake std headers

use CPtr;
extern var intArr : c_ptr(c_int);

extern var stringList : c_ptr(c_string);

extern proc args(a : c_ptr(c_int), ref b : c_int) : void;

extern proc args(a : c_ptr(c_int), b : c_ptr(c_int)) : void;

extern proc sized(x : c_ptr(c_int), ref y : c_int) : void;

extern proc sized(x : c_ptr(c_int), y : c_ptr(c_int)) : void;

extern record foobar {
  var x : c_int;
  var y : c_ptr(c_int);
  var z : c_ptr(c_ptr(c_int));
}

