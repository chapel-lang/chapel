record foo {
  type eltType;
  var x : eltType;
  proc print() {
    writeln(x);
  }
}

var f : foo(string);
f.x = "hello world";
writeln(f.x);

var g : foo(int);
g.x = 2;
writeln(g.x);
