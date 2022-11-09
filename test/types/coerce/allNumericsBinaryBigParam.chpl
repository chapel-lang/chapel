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
proc callF(type t1, param p2) {
  var s: string = p2:string + " : " + p2.type:string;
  writef(" Second actual %-32s -> ", s);
  var x: t1;
  f(x, p2);

  writef("      Reversed %-32s -> ", "");
  f(p2, x);

  writef("               %-32s ->   ", "+");
  writeln((x+p2).type:string);
}

proc callFVaryP(type t1) {
  writeln("First actual ", t1:string);
  writeln();

  callF(t1, min(int(8)):int(8));
  callF(t1, max(int(8)):int(8));
  callF(t1, min(int(16)):int(16));
  callF(t1, max(int(16)):int(16));
  callF(t1, min(int(32)):int(32));
  callF(t1, max(int(32)):int(32));
  callF(t1, min(int(64)):int(64));
  callF(t1, max(int(64)):int(64));
  writeln();

  callF(t1, min(uint(8)):uint(8));
  callF(t1, max(uint(8)):uint(8));
  callF(t1, min(uint(16)):uint(16));
  callF(t1, max(uint(16)):uint(16));
  callF(t1, min(uint(32)):uint(32));
  callF(t1, max(uint(32)):uint(32));
  callF(t1, min(uint(64)):uint(64));
  callF(t1, max(uint(64)):uint(64));
  writeln();

  writeln();
  writeln();
}

proc main() {
  callFVaryP(int(8));
  callFVaryP(int(16));
  callFVaryP(int(32));
  callFVaryP(int(64));

  callFVaryP(uint(8));
  callFVaryP(uint(16));
  callFVaryP(uint(32));
  callFVaryP(uint(64));

  callFVaryP(real(32));
  callFVaryP(real(64));

  callFVaryP(imag(32));
  callFVaryP(imag(64));

  callFVaryP(complex(64));
  callFVaryP(complex(128));
}

