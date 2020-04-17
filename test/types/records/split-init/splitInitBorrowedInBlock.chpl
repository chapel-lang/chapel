class C { var x: int = 0; }

proc test() {
  var x: borrowed C?;
  if false {
    x = new borrowed C?(128);
  } else {
    x = new borrowed C?(256);
  }
  writeln(x);
}
test();
