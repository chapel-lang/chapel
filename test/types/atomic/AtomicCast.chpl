proc test0() {
  inline proc testWithType(type t) {
    var a: t;
    var x = a : atomic t;
    writeln(x.read());
  }

  testWithType(int(8));
  testWithType(int(16));
  testWithType(int(32));
  testWithType(int(64));
  testWithType(uint(8));
  testWithType(uint(16));
  testWithType(uint(32));
  testWithType(uint(64));
  testWithType(bool);
}

proc main() {
  test0();
}
