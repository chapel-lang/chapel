record GR { type t; var field: t; }

proc foo() type { return GR; }

record R1 {
  var genericFieldX: GR;
}

record R2 {
  var genericFieldY: foo();
}

proc main() {
  var x = new R1(new GR(int, 0));
  var y = new R2(new GR(int, 0));

  var myvar1: GR;
  var myvar2: GR(?);
  var myvar3: foo();
  var myvar4: foo()(?);
  myvar1 = new GR(int, 1);
  myvar2 = new GR(int, 2);
  myvar3 = new GR(int, 3);
  myvar4 = new GR(int, 4);
  compilerError("done");
}
