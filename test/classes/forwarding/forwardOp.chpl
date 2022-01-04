record Foo {
  var x: Bar;
  forwarding x only +;
}

record Bar {
  var x: int;

  operator +(x: Bar, y: Bar) {
    writeln("In B.+");
    return new Bar(x.x + y.x);
  }
}

proc main() {
  var f1 = new Foo(new Bar(3));
  var f2 = new Foo(new Bar(7));
  var b3 = f1 + f2;
  writeln(b3.type: string);
  writeln(b3);
}
