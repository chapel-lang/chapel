use LinkedLists;

proc foo() {
  var s: LinkedList(int) = makeList( 1, 2, 3 );

  writeln("s is: ", s);

  return s;
}

var a = foo();

writeln("a is: ", a);

a.destroy();
