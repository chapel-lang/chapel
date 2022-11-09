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
  var s2: string = p2:string + " : " + p2.type:string;
  writef(" Second actual %-32s -> ", s2);
  f(p1, p2);

  writef("      Reversed %-32s -> ", "");
  f(p2, p1);

  writef("               %-32s ->   ", "+");
  writeln((p1+p2).type:string);
}

proc callFVaryP(param p1) {
  var s1: string = p1:string + " : " + p1.type:string;
  writeln("First actual ", s1);
  writeln();

  callF(p1, min(int(8)):int(8));
  callF(p1, max(int(8)):int(8));
  callF(p1, min(int(16)):int(16));
  callF(p1, max(int(16)):int(16));
  callF(p1, min(int(32)):int(32));
  callF(p1, max(int(32)):int(32));
  callF(p1, min(int(64)):int(64));
  callF(p1, max(int(64)):int(64));
  writeln();

  callF(p1, min(uint(8)):uint(8));
  callF(p1, max(uint(8)):uint(8));
  callF(p1, min(uint(16)):uint(16));
  callF(p1, max(uint(16)):uint(16));
  callF(p1, min(uint(32)):uint(32));
  callF(p1, max(uint(32)):uint(32));
  callF(p1, min(uint(64)):uint(64));
  callF(p1, max(uint(64)):uint(64));
  writeln();

  writeln();
  writeln();
}

proc main() {
  callFVaryP(min(int(8)):int(8));
  callFVaryP(max(int(8)):int(8));
  callFVaryP(min(int(16)):int(16));
  callFVaryP(max(int(16)):int(16));
  callFVaryP(min(int(32)):int(32));
  callFVaryP(max(int(32)):int(32));
  callFVaryP(min(int(64)):int(64));
  callFVaryP(max(int(64)):int(64));

  callFVaryP(min(uint(8)):uint(8));
  callFVaryP(max(uint(8)):uint(8));
  callFVaryP(min(uint(16)):uint(16));
  callFVaryP(max(uint(16)):uint(16));
  callFVaryP(min(uint(32)):uint(32));
  callFVaryP(max(uint(32)):uint(32));
  callFVaryP(min(uint(64)):uint(64));
  callFVaryP(max(uint(64)):uint(64));
}

