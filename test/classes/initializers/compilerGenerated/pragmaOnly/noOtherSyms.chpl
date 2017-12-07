// Ensures the pragma only applies to classes and records
pragma "use default init"
var x = 10; // You cannot use it on a var

pragma "use default init"
type t = string; // You cannot use it with a char

pragma "use default init"
param p: int; // You cannot use it with an int

pragma "use default init"
const c: uint = 10; // Even with an explicit init

pragma "use default init"
module It { // No module it

  pragma "use default init"
  proc foo() { // Nor function writ
    writeln("something something");
  }

  pragma "use default init"
  iter someIter() { // You cannot use it, Sam I am
    for i in 1..10 {
      yield i;
    }
  }

  pragma "use default init"
  enum anEnum {green, eggs, and, ham} // You cannot use green eggs and ham.

  proc main() {
    foo();
    for i in someIter() {
      writeln(i);
    }
    var e = anEnum.ham;
    writeln(e);
  }
}
