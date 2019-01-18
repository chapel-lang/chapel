record R {
  var x:int;
}

const A:[1..3] R = new R(1);

writeln(+ reduce A.x);

var B:[1..3] R;

B.x = A.x;

writeln(A);
writeln(B);
