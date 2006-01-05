class foo {
  var x;
  function print() {
    writeln(x);
  }
}

var f = foo();

f.x = "howdy";

f.print();

var f2 = foo();

f2.x = 3.2;

f2.print();
