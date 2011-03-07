class foo {
  type t;
  var x : t;
  proc initialize() {
    x += 10;
  }
  proc print() {
    writeln(x);
  }
}

var f : foo(t=int) = new foo(t=int, x=3);

f.print();
