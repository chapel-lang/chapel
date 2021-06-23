// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "miscTypedefUnion.h";

// Note: Generated with fake std headers

use CPtr;
extern record simpleUnion {
  var a : c_int;
  var b : c_char;
  var c : c_void_ptr;
  var d : my_int;
  var e : my_string;
}

extern proc retUnion(a : my_int, b : my_int, in r : renamedUnion) : fancyUnion;

extern proc tdPointer(ref a : fancyUnion, ref b : c_ptr(renamedUnion)) : void;

extern proc tdPointer(a : c_ptr(fancyUnion), b : c_ptr(c_ptr(renamedUnion))) : void;



extern record forwardUnion {
  var a : c_int;
  var b : c_int;
}

// ==== c2chapel typedefs ====

extern record fancyUnion {
  var a : c_int;
  var b : c_int;
  var c : renamedUnion;
  var d : simpleUnion;
}

extern type fwdUnion = forwardUnion;

extern type my_int = c_int;

extern type my_string = c_string;

// Typedef'd pointer to union
extern type recp;

extern type renamedFancy = fancyUnion;

extern type renamedUnion = simpleUnion;

