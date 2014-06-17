class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f = new foo(2);

f.print();

f.x = 3.2;

f.print();
