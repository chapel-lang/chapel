record foo {
  type elt_type;
  var x : elt_type;
  fun print() {
    writeln(x);
  }
}

var f : foo of string;
f.x = "hello world";
writeln(f.x);

var g : foo of int;
g.x = 2;
writeln(g.x);
