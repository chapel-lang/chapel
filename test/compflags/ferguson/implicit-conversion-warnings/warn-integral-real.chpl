proc test1fn(arg: real(32)) { }

proc test1() {
  var myInt: int = 22;
  test1fn(myInt);
}
test1();

proc test2fn(arg: real(64)) { }

proc test2() {
  var myUint16: uint(16) = 22;
  test2fn(myUint16);
}
test2();
