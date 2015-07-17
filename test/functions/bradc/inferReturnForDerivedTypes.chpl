class C {
}

class D: C {
  var x: int;
}

class E: C {
  var x: real;
}

var d = new D();
var e = new E();

proc B(i) /* WORKAROUND is: ': C' */ {
  if i == 1 then return d;
  if i == 2 then return e;
  halt("Unexpected value of i");
}

var myc = B(1);
writeln("myc = ", myc);
myc = B(2);
writeln("myc = ", myc);

