// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "simpleRecords.h";

// Note: Generated with fake std headers

extern record allInts {
  var a : c_int;
  var b : c_uint;
  var c : c_longlong;
}

extern record misc {
  var a : c_char;
  var b : c_string;
  var c : c_void_ptr;
  var d : c_ptr(c_int);
}

extern record composition {
  var m : misc;
  var i : c_ptr(allInts);
}

extern proc structArgs(in a : misc, in b : misc) : void;

extern proc retStruct(in a : misc) : allInts;

extern proc structPointer(ref a : misc) : void;

extern proc structPointer(a : c_ptr(misc)) : void;

