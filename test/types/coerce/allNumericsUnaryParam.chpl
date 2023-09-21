proc f(x: int(8)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: int(16)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: int(32)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: int(64)) {
  writeln("  f(", x.type:string, ")");
}

proc f(x: uint(8)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: uint(16)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: uint(32)) {
  writeln("  f(", x.type:string, ")");
}
proc f(x: uint(64)) {
  writeln("  f(", x.type:string, ")");
}

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

proc f(x) {
  writeln("  f(generic)");
}


// next, call 'f' with all combinations of numeric types
proc callF(param p) {
  writef(" actual %<12s -> ", p.type:string);
  f(p);
}

proc main() {
  // default-sized params
  callF(1);
  callF(1.0);
  callF(2.0i);
  callF(1.0+2.0i);

  // other-sized params
  callF(1:int(8));
  callF(1:int(16));
  callF(1:int(32));
  callF(1:int(64));

  callF(1:uint(8));
  callF(1:uint(16));
  callF(1:uint(32));
  callF(1:uint(64));

  callF(1.0:real(32));
  callF(1.0:real(64));

  callF(2.0i:imag(32));
  callF(2.0i:imag(64));

  callF((1.0+2.0i):complex(64));
  callF((1.0+2.0i):complex(128));
}
