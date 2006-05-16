class foo {
  var x;
  fun print() {
    writeln(x);
  }
}

var f : foo = foo(3.2);

f.print();

f.x = 2;

f.print();
