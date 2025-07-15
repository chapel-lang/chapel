//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;

/* START_EXAMPLE */
class Parent {
  var x : int;
}

class Child: Parent {
  var y : real;
}
/* STOP_EXAMPLE */

var c = new Child(x=5, y=2);
stdout.withSerializer(defaultSerializer).writeln(c);
