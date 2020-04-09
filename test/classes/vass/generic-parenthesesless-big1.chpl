class C {
  param x;
  proc f1: int {
//    compilerWarning(this.type:string);
    return 11;
  }
}

proc C.f2: int {
//  compilerWarning(this.type:string);
  return 22;
}

proc C.f3(): int {
//  compilerWarning(this.type:string);
  return 33;
}

proc C.g1: int return f1;
proc C.g2: int return f2;
proc C.g3: int return f3();

proc C.h1(): int return f1;
proc C.h2(): int return f2;
proc C.h3(): int return f3();

var c = new borrowed C(1);

writeln((
         c.g1,
         c.g2,
         c.g3,
         c.h1(),
         c.h2(),
         c.h3()
         ));
