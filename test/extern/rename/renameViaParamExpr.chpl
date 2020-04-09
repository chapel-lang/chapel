require "ccode.h";

extern "ty" + "pe" var cVarType: c_int;

proc recordName() param {
  return "S";
}

extern recordName() record R {
  var okname: c_int;
  extern "t" + "ype" var cFieldType: c_int;
}

extern "print" + recordName() proc foo(ref myS: R);

extern "i" + "nt" type myCInt = c_int;

var x: myCInt = 23;

var myR: R;
myR.okname = cVarType;
myR.cFieldType = x;

foo(myR);

