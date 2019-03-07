
var A: [1..3] int;

var b, c: int;

forall a in A with (+ reduce b) {
  (b+c) reduce= a;
}
