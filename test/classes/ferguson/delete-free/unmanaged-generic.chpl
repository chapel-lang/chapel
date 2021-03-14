
class C {
  type t;
  var x: t;
}

proc f(arg: unmanaged C) {
  writeln(arg.type:string, " ", arg);
}

var myC: unmanaged C = new unmanaged C(int, 1);
writeln(myC.type:string, " ", myC);
f(myC);
delete myC;
