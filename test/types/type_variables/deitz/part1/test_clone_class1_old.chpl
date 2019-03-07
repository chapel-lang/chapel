class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f : borrowed foo(2) = new borrowed foo(2);

f.print();
