// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "anonymousStruct.h";

// Note: Generated with fake std headers

use CTypes;
extern "struct foobar" record foobar {
  var a : c_int;
  var b : c_int;
  var c : c_int;
}

extern "union namedUnion" union namedUnion {
  var a : c_int;
}

// Anonymous union or struct was encountered within and skipped.
extern "struct nestedAnon" record nestedAnon {}

// Anonymous union or struct was encountered within and skipped.
extern "struct nestedAnonWithUnion" record nestedAnonWithUnion {}

// ==== c2chapel typedefs ====

extern union anonymousUnion {
  var a : c_int;
}

// Anonymous union or struct was encountered within and skipped.
extern record nestedAnonWithUnion_t {}

// Anonymous union or struct was encountered within and skipped.
extern record nestedAnon_t {}

extern record sports {
  var a : c_int;
}

