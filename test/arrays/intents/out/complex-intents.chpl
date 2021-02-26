proc f1(out a, b = a, c:b.type, d, in e:[d] int) {
  writeln("in f1");
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
  writeln("d = ", d);
  writeln("e = ", e, " ", e.domain);
}
proc f2(d, in a:[d] int, b = a, c:b.type) {
  writeln("in f2");
  writeln("d = ", d);
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
}
proc f3(out a, in b = a, c:b.type, d, in e:[d] int) {
  writeln("in f3");
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
  writeln("d = ", d);
  writeln("e = ", e, " ", e.domain);
}
proc f4(d, in a:[d] int, in b = a, c:b.type) {
  writeln("in f4");
  writeln("d = ", d);
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
}


proc f5(out a, b = a, c:b.type, ref d, in e:[d] int) {
  writeln("in f5");
  d = {0..1};
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
  writeln("d = ", d);
  writeln("e = ", e, " ", e.domain);
}
proc f6(ref d, in a:[d] int, b = a, c:b.type) {
  writeln("in f6");
  d = {1..2};
  writeln("d = ", d);
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
}
proc f7(out a, in b = a, c:b.type, ref d, in e:[d] int) {
  writeln("in f7");
  d = {0..1};
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
  writeln("d = ", d);
  writeln("e = ", e, " ", e.domain);
}
proc f8(ref d, in a:[d] int, in b = a, c:b.type) {
  writeln("in f8");
  d = {1..2};
  writeln("d = ", d);
  writeln("a = ", a, " ", a.domain);
  writeln("b = ", b, " ", b.domain);
  writeln("c = ", c, " ", c.domain);
}

proc f9(D = {1..3}, in A: [D] int) {
  writeln("in f9");
  writeln("A = ", A, " ", A.domain);
}

proc f10(in D = {1..3}, in A: [D] int) {
  writeln("in f10");
  writeln("A = ", A, " ", A.domain);
  D = {1..2};
  writeln("A = ", A, " ", A.domain);
}


proc main() {
  {
    var d = {1..1};
    var a: [d] int = 2;
    var c: [d] int = 3;
    var e: [d] int = 4;

    f1(a=a, /*b default*/ c=c, d=d, e=e);
  }

  {
    var d = {1..1};
    var a: [d] int = 2;
    var c: [d] int = 3;
    var e: [d] int = 4;

    f2(d, a, /* b default*/ c=c);
  }

  {
    var d = {1..1};
    var a: [d] int = 2;
    var c: [d] int = 3;
    var e: [d] int = 4;

    f3(a=a, /*b default*/ c=c, d=d, e=e);
  }

  {
    var d = {1..1};
    var a: [d] int = 2;
    var c: [d] int = 3;
    var e: [d] int = 4;

    f4(d, a, /* b default*/ c=c);
  }

  {
    var d = {1..1};
    var a: [2..2] int = 2;
    var c: [2..2] int = 3;
    var e: [1..1] int = 4;

    f5(a=a, /*b default*/ c=c, d=d, e=e);
  }

  {
    var d = {1..1};
    var a: [d] int = 2;
    var c: [d] int = 3;
    var e: [1..1] int = 4;

    f5(a=a, /*b default*/ c=c, d=d, e=e);
  }

  {
    var d = {1..1};
    var a: [1..1] int = 2;
    var c: [1..1] int = 3;

    f6(d, a, /* b default*/ c=c);
  }

  {
    var d = {1..1};
    var a: [2..2] int = 2;
    var c: [2..2] int = 3;
    var e: [1..1] int = 4;

    f7(a=a, /*b default*/ c=c, d=d, e=e);
  }

  {
    var d = {1..1};
    var a: [d] int = 2;
    var c: [d] int = 3;
    var e: [1..1] int = 4;

    f7(a=a, /*b default*/ c=c, d=d, e=e);
  }

  {
    var d = {1..1};
    var a: [1..1] int = 2;
    var c: [1..1] int = 3;

    f8(d, a, /* b default*/ c=c);
  }

  {
    var A: [1..3] int = 9;
    f9(A=A);
  }

  {
    var A: [1..3] int = 10;
    f10(A=A);
  }
}
