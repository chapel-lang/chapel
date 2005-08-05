/* testing getter and setter functions that will be inlined */
class Foo {
  var x : integer;
  var y : integer;
  var z : integer;
}

function main() {
  var f : Foo = Foo();
  f.x = 1;
  f.y = 2;
  f.z = 3; 

  writeln(f.y);
}