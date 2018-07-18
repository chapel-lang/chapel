// Ensures the pragma only applies to classes and records
var x = 10; // You cannot use it on a var

type t = string; // You cannot use it with a char

param p: int; // You cannot use it with an int

const c: uint = 10; // Even with an explicit init

module It { // No module it

  proc foo() { // Nor function writ
    writeln("something something");
  }

  iter someIter() { // You cannot use it, Sam I am
    for i in 1..10 {
      yield i;
    }
  }

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
