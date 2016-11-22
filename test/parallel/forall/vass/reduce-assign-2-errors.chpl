
var A: [1..3] int;

var b, c: int;

b reduce= c;

forall a1 in A with (+ reduce b) {
  forall a2 in A {
    b reduce= a1;
  }
}
