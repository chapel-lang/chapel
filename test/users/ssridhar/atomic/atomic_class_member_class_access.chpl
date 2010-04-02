var x: int;

class Foo {
  var x: int;
  var y: 2*int;
  var z: [1..3] int;
}

class Bar {
  var x:int;
  var f: Foo;
}

def main {
  begin atomic x = x + 1;
  foobar;
}

def foobar  {
  var i: int;
  var c = new Bar(x = 4, new Foo(5, (1,2)));
  var d: Bar = c;
  atomic d.x = d.x + 1; 
  atomic d.f.x = d.f.x + 1;
  [i in 1..3] d.f.z(i) = 0;
  [i in 1..3] atomic d.f.z(i) = d.f.z(i) + 1;
  atomic [i in 1..3] d.f.z(i) = d.f.z(i) + 1;
  writeln("c = ", c);
  writeln("d = ", d);
}
