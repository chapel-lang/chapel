record GR { type t; var field: t; }

proc foo() type { return GR; }

record R1 {
  var genericFieldX: GR;
}

record R2 {
  var genericFieldY: foo();
}

record R3 {
  var genericFieldX: GR;
  proc init() {
    this.genericFieldX = new GR(int, 0);
  }
}

proc main() {
  var x = new R1(new GR(int, 0));
  var y = new R2(new GR(int, 0));
  var z = new R3();

  var myvar1: GR;
  var myvar2: GR(?);
  var myvar3: foo();
  var myvar4: foo()(?);
  myvar1 = new GR(int, 1);
  myvar2 = new GR(int, 2);
  myvar3 = new GR(int, 3);
  myvar4 = new GR(int, 4);

  var myvar5: GR = new GR(int, 5);
  var myvar6: GR(?) = new GR(int, 6);

  genericFn(GR(?));

  compilerError("done");
}

proc genericFn(type t) {
  var x: t;
  x = new GR(int, 5);
}
