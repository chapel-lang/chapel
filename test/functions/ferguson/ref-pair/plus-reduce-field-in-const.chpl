// a test of const-checking

record R {
  var x:int;
}

const A:[1..3] R;

writeln(+ reduce A.x);
