
// Tests usage of standard module types/functions/methods, which have to
// co-exist with untyped converted AST

use Print;
use CTypes;

proc main() {
  var x : int = 5;
  var y = c_ptrTo(x);
  println(y.deref());

  {
    var a : atomic int;
    a.add(1);
    a.add(5);
    println(a.read());
  }
}

