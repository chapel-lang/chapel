class C {
  var a: int;
}
class D: C {
  var b: int;
}

var c: unmanaged C?;
var c2: unmanaged C?;

c = new unmanaged D(1, 2);

on Locales(1) {
  c2 = new unmanaged D(1, 2);
}

writeln("First local block");
local {
  var d = c:D;
  writeln(d.b);
}

writeln("Second local block");
local {
  var d2 = c2:D; // error, non-local dynamic cast
  writeln(d2.b); // also would be an error
}
