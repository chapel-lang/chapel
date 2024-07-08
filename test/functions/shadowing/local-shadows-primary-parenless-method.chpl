// Example 5a (primary method variant) (based on issue 21668)
module M {
  record r {
    proc foo { writeln("in A.r.foo"); return 5; }
  }
  proc r.method() {
    var foo: int;
    foo; // is this referring to the int or the parenless method?
         // production compiler refers to the local variable
    writeln(foo); // prints out the local variable
  }

  proc main() {
    var x: r;
    x.method();
  }
}
