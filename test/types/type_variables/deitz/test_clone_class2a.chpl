class foo {
  var x;
  fun print() {
    writeln(x);
  }
}

var f = foo();

f.x = "howdy";

f.print();

var f2 = foo();

f2.x = 3.2;

f2.print();
