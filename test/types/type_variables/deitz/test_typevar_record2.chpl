record foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f : foo of string;
f.x = "hello world";
writeln(f.x);

var g : foo of integer;
g.x = 2;
writeln(g.x);
