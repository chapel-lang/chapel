// Example 20b from 21668
module A {
  record r { }
}

module B {
  import A.r;
  proc foo { writeln("in non-method foo"); }
  proc r.foo { writeln("in method foo"); }
  proc r.method() {
    foo; // does it call the method or non-method?
         // production compiler calls the non-method
  }

  proc main() {
    var x: r;
    x.method();
  }
}
