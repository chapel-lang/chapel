// Example 5a
module M {
  record r { }
  proc r.method() {
    var foo: int;
    proc r.foo { writeln("in A.r.foo"); return 23; }
    foo; // is this referring to the int or the parenless method?
         // production compiler refers to the local variable
    writeln(foo); // prints out the local variable
  }

  proc main() {
    var x: r;
    x.method();
  }
}
