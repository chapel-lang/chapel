class foo {
  var x;
  fun print() {
    writeln(x);
  }
}

var f : foo = foo();

f.x = 3.2;

f.print();

f.x = 2;

f.print();
