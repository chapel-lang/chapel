require "ccode.h";

use CTypes;

extern record S {
  var okname: c_int;
  extern "type" var ctype: c_int;
}

extern proc printS(ref myS: S);

var myS: S;
myS.okname = 23;
myS.ctype = 42;
printS(myS);
