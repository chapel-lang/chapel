// first, declare a function 'f' to report which overload is called,
// with a version having both arguments of the same type,
// but one of those for each numeric type

proc f(x: int(8), y: int(8)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: int(16), y: int(16)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: int(32), y: int(32)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: int(64), y: int(64)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}

proc f(x: uint(8), y: uint(8)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: uint(16), y: uint(16)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: uint(32), y: uint(32)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: uint(64), y: uint(64)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}

proc f(x: real(32), y: real(32)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: real(64), y: real(64)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}

proc f(x: imag(32), y: imag(32)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: imag(64), y: imag(64)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}

proc f(x: complex(64), y: complex(64)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}
proc f(x: complex(128), y: complex(128)) {
  writeln("  f(", x.type:string, ",", y.type:string, ")");
}

// next, call 'f' with all combinations of numeric types
proc callF(param p1, param p2) {
  writef(" Second actual %-12s -> ", p2.type:string);
  f(p1, p2);

  writef("      Reversed %-12s -> ", "");
  f(p2, p1);

  writef("               %-12s ->   ", "+");
  writeln((p1+p2).type:string);
}

proc callFVaryP(param p1) {
  writeln("First actual ", p1.type:string);
  writeln();

  // default-sized params
  callF(p1, 1);
  callF(p1, 1.0);
  callF(p1, 2.0i);
  callF(p1, 1.0+2.0i);
  writeln();

  // other-sized params
  callF(p1, 1:int(8));
  callF(p1, 1:int(16));
  callF(p1, 1:int(32));
  callF(p1, 1:int(64));
  writeln();

  callF(p1, 1:uint(8));
  callF(p1, 1:uint(16));
  callF(p1, 1:uint(32));
  callF(p1, 1:uint(64));
  writeln();

  callF(p1, 1.0:real(32));
  callF(p1, 1.0:real(64));
  writeln();

  callF(p1, 2.0i:imag(32));
  callF(p1, 2.0i:imag(64));
  writeln();

  callF(p1, (1.0+2.0i):complex(64));
  callF(p1, (1.0+2.0i):complex(128));
  writeln();

  writeln();
  writeln();
}

proc main() {
  // default-sized params
  callFVaryP(1);
  callFVaryP(1.0);
  callFVaryP(2.0i);
  callFVaryP(1.0+2.0i);

  // other-sized params
  callFVaryP(1:int(8));
  callFVaryP(1:int(16));
  callFVaryP(1:int(32));
  callFVaryP(1:int(64));

  callFVaryP(1:uint(8));
  callFVaryP(1:uint(16));
  callFVaryP(1:uint(32));
  callFVaryP(1:uint(64));

  callFVaryP(1.0:real(32));
  callFVaryP(1.0:real(64));

  callFVaryP(2.0i:imag(32));
  callFVaryP(2.0i:imag(64));

  callFVaryP((1.0+2.0i):complex(64));
  callFVaryP((1.0+2.0i):complex(128));
}

