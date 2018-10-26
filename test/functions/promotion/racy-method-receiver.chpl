proc ref int.increment(x: int) {
  this += x;
}

proc const ref int.noop(x: int) {
}

config const n = 5000;
config const verbose = false;

proc test1() {
  var x = 42;
  x.noop(1);
  x.increment(1); // works as expected
  writeln("1A expect ", 43);
  writeln("1A    got ", 43);

  var A: [1..n] int = 1..n;
  var B: [1..n] int;
  B.noop(A);
  B.increment(A); // works as expected
  writeln("1B expect ", + reduce A);
  writeln("1B    got ", + reduce B);
  if verbose then
    writeln(B);

  x.noop(A);
  x.increment(A); // racy!
  writeln("1C expect ", 43 + (+ reduce A));
  writeln("1C    got ", x);
}
test1();

record R {
  var value:int;
}

proc R.addem(a:R, b:R) {
  this.value = a.value + b.value;
}

proc R.noop(a:R, b:R) {
}

proc test2() {
  var A:[1..n] R = [i in 1..n] new R(i);
  var B:[1..n] R = [i in 1..n] new R(i);
  var C:[1..n] R;

  C.noop(A, B); //OK
  C.addem(A, B); //OK
  writeln("2A expect ", 2 * (+ reduce A.value));
  writeln("2A    got ", + reduce C.value);

  var result:R;

  result.noop(A, B); // OK
  result.addem(A, B); // racy!
  writeln("2B expect ", 2 * (+ reduce A.value));
  writeln("2B    got ", result.value);
}
test2();
