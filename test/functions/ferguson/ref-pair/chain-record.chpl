

record R {
  var x: int;
  proc ref foo() ref {
    writeln("REF");
    return x;
  }

  proc const foo() const ref {
    writeln("CONST REF");
    return x;
  }
}

record RR {
  var x:R;
  proc ref bar() ref {
    writeln("REF");
    return x;
  }

  proc const ref bar() const ref {
    writeln("CONST REF");
    return x;
  }
}


proc outputX(x) {
  writeln(x);
}

proc setX(ref x) {
  x = 1;
}

var r = new R(1);
var rr = new RR(new R(2));

writeln("writeX: ");
outputX(r.foo());
outputX(rr.bar().foo());
outputX(rr.bar().x);

writeln("setX: ");
setX(r.foo());
setX(rr.bar().foo());
setX(rr.bar().x);
