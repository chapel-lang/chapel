class MyClass {
  var x: int;
  proc this(i:int) {
    return x;
  }
}

proc test() {
  var c = new owned MyClass(1);
  var b = c:borrowed MyClass?;
  writeln(b!.x);
  writeln(b!.this(1));
  writeln(b!(1));
  writeln(b![1]);
}
test();
