/* testing getter and setter funs that will be inlined */
class Foo {
  var x : int;
  var y : int;
  var z : int;
}

proc main() {
  var f : borrowed Foo = new borrowed Foo();

  f.x = 1;
  f.y = 2;
  f.z = 3;

  writeln(f.y);
}
