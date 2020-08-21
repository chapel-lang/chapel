// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "miscTypedef.h";

// Note: Generated with fake std headers

extern record simpleStruct {
  var a : c_int;
  var b : c_char;
  var c : c_void_ptr;
  var d : my_int;
  var e : my_string;
}

extern proc retStruct(a : my_int, b : my_int, in r : renamedStruct) : fancyStruct;

extern proc tdPointer(ref a : fancyStruct, ref b : c_ptr(renamedStruct)) : void;

extern proc tdPointer(a : c_ptr(fancyStruct), b : c_ptr(c_ptr(renamedStruct))) : void;



extern record forwardStruct {
  var a : c_int;
  var b : c_int;
}

// ==== c2chapel typedefs ====

extern record fancyStruct {
  var a : c_int;
  var b : c_int;
  var c : renamedStruct;
  var d : simpleStruct;
}

extern type fwdStruct = forwardStruct;

extern type my_int = c_int;

extern type my_string = c_string;

// Typedef'd pointer to struct
extern type recp;

extern type renamedFancy = fancyStruct;

extern type renamedStruct = simpleStruct;

