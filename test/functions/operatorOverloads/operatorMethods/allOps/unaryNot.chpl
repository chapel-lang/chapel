// Test ~ operator as an operator method
record Foo {
  var x: int;

  operator ~(arg: Foo) {
    writeln("In Foo.~");
    return new Foo(~arg.x);
  }
}

proc main() {
  var a = new Foo(3);
  var b = new Foo(7);
  var c = ~a;
  writeln(~a);
  writeln(~b);
  writeln(~c);
}
