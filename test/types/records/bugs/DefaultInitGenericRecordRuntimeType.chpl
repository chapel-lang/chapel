
record foo {
  type T;
  proc doSomething(in val: T) {
    writeln(val);
  }
}

proc test() {
  var obj: foo(domain(3));
  obj.doSomething({0..1, 2..3, 4..5});
}
test();

