proc test1fn(arg: real) { }

proc test1() {
  var myReal32: real(32) = 2.0;
  test1fn(myReal32);
}
test1();
