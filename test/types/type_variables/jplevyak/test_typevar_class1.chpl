class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f : borrowed foo(int) = new borrowed foo(t=int);

f.x = 2;

f.print();
