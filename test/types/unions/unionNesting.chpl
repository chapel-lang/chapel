

record R1 {
  var x: int = 1;
  var y: real = 2.0;
  proc deinit() {
    writeln("deinit'ing R1");
  }
}
record R2 {
  var x: int = 3;
  var y: real = 4.0;
  proc deinit() {
    writeln("deinit'ing R2");
  }
}

union U {
  var r1: R1;
  var r2: R2;
}


class C1 {
  var x: int = 1;
  var y: real = 2.0;
  proc deinit() {
    writeln("deinit'ing C1");
  }
}
class C2 {
  var x: int = 3;
  var y: real = 4.0;
  proc deinit() {
    writeln("deinit'ing C2");
  }
}
union UU {
  var c1: owned C1?;
  var c2: shared C2?;
}

proc main() {
  {
    var u: U;
    writeln(u.type:string, " ", u);
    u.r1 = new R1();
    writeln(u);
    u.r2 = new R2();
    writeln(u);
  }

  {
    var uu: UU;
    writeln(uu.type:string, " ", uu);
    uu.c1 = new C1();
    writeln(uu);
    uu.c2 = new shared C2();
    writeln(uu);
    writeln(uu.type:string);
    writeln(uu.c2.type:string);
    writeln(uu.c1.type:string);
  }
}
