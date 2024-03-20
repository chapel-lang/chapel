// Example 5 from 21668
module A {
  record r { }
  proc r.foo { writeln("in A.r.foo"); }
}
module B {
  use A;
  var foo: int;

  proc r.tertiary() {
    foo; // is this A.r.foo or B.foo?
         // production compiler calls A.r.foo
  }

  proc main() {
    var x: r;
    x.tertiary();
  }
}
