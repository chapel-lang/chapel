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
{
  var r1: A.B.R;
  writeln(B); // Ensures the B above isn't hidden by the nested module
  writeln(r1.foo());
}
