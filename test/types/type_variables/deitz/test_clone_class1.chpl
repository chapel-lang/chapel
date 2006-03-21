class foo {
  var x;
  fun print() {
    writeln(x);
  }
}

var f : foo = foo();

f.x = 2;

f.print();
