module A {
  module B {
    record R {
      var x: int;

      proc foo() {
        return x;
      }
    }
  }
}
var B = "blah";
var r1: A.B.R;        // error: unresolved call 'R.init()'
var r2 = new A.B.R(); // But this one works, so that's obnoxious.  Should it?
writeln(B);
writeln(r1.foo());
writeln(r2.foo());
