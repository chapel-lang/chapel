extern proc foo(x: [] int, ref y: [] int);

proc main() {
  var A,B: [1..10] int;
  foo(A, B);
}
