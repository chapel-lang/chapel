// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "anonymousStruct.h";

// Note: Generated with fake std headers

use CPtr;
use SysCTypes;
use SysBasic;
extern "struct foobar" record foobar {
  var a : c_int;
  var b : c_int;
  var c : c_int;
}

extern "union namedUnion" union namedUnion {
  var a : c_int;
}

// ==== c2chapel typedefs ====

extern union anonymousUnion {
  var a : c_int;
}

extern record sports {
  var a : c_int;
}

