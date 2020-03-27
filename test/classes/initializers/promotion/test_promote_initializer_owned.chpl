// copied from test/arrays/deitz/part5/test_promote_constructor.chpl
class C {
  var a: int;
}

var A: [1..5] int;
var B: [1..5] owned C?;

B = new owned C(A);

writeln(B);
