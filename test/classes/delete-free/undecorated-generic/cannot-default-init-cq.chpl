class C { var x: int; }

proc test() {
  var bb: C?;
  writeln(bb.type:string);
}
test();
