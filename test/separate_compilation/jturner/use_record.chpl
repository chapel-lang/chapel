record Foo {
  var x : int;
  var y : int;
}

proc dothis(f:Foo):Foo;

proc main() {
  var r = new Foo(3, 4);
  var r2 = dothis(r);
  writeln(r2.x);
  writeln(r2.y);
}
