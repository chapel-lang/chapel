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
proc callF(type t1, type t2) {
  writef(" Second actual %-12s -> ", t2:string);
  var x: t1;
  var y: t2;
  f(x, y);

  writef("      Reversed %-12s -> ", "");
  f(y, x);

  writef("               %-12s ->   ", "+");
  writeln((x+y).type:string);
}

proc callFVaryT2(type t1) {
  writeln("First actual ", t1:string);
  writeln();

  callF(t1, int(8));
  callF(t1, int(16));
  callF(t1, int(32));
  callF(t1, int(64));
  writeln();

  callF(t1, uint(8));
  callF(t1, uint(16));
  callF(t1, uint(32));
  callF(t1, uint(64));
  writeln();

  callF(t1, real(32));
  callF(t1, real(64));
  writeln();

  callF(t1, imag(32));
  callF(t1, imag(64));
  writeln();

  callF(t1, complex(64));
  callF(t1, complex(128));
  writeln();

  writeln();
  writeln();
}

proc main() {
  callFVaryT2(int(8));
  callFVaryT2(int(16));
  callFVaryT2(int(32));
  callFVaryT2(int(64));

  callFVaryT2(uint(8));
  callFVaryT2(uint(16));
  callFVaryT2(uint(32));
  callFVaryT2(uint(64));

  callFVaryT2(real(32));
  callFVaryT2(real(64));

  callFVaryT2(imag(32));
  callFVaryT2(imag(64));

  callFVaryT2(complex(64));
  callFVaryT2(complex(128));
}

