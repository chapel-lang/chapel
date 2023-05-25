proc f(x: real(32)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: real(64)) {
  writeln("  f(", x.type:string, ")");
}

proc f(x: imag(32)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: imag(64)) {
  writeln("  f(", x.type:string, ")");
}

proc f(x: complex(64)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: complex(128)) {
  writeln("  f(", x.type:string, ")");
}

// next, call 'f' with all combinations of numeric types
proc callF(type t1) {
  writef(" actual %-12s -> ", t1:string);
  var x: t1;
  f(x);
}

proc main() {
  callF(int(8));
  callF(int(16));
  callF(int(32));
  callF(int(64));

  callF(uint(8));
  callF(uint(16));
  callF(uint(32));
  callF(uint(64));

  callF(real(32));
  callF(real(64));

  callF(imag(32));
  callF(imag(64));

  callF(complex(64));
  callF(complex(128));
}
