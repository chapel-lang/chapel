// copied from test/arrays/deitz/part5/test_promote_constructor.chpl
class C {
  var a: int;
}

var A: [1..5] int;
var B: [1..5] shared C?;

B = new shared C(A);

writeln(B);
