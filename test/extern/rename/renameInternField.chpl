record S {
  var okname: int;
  extern "type" var ctype: int;
}

var myS: S;
myS.okname = 23;
myS.ctype = 42;
writeln(myS);

