// Example 5' from 21668 née 15937
record R {
  proc field { return 1; }
  var field = 2;

  proc foo() { writeln(field);} // calls the parenless proc
}

var r: R;
r.foo();
