require "ccode.h";

extern record S {
  var okname: int;
  extern "type" var ctype: int;
}

extern proc printS(ref myS: S);

var myS: S;
myS.okname = 23;
myS.ctype = 42;
printS(myS);
