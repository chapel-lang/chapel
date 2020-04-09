proc increment(ref rcv: int, x: int) {
  rcv += x;
}

proc noop(const ref rcv: int, x: int) {
}

config const n = 5000;
config const verbose = false;

proc test1() {
  var x = 42;
  noop(x, 1);
  increment(x, 1); // works as expected
  writeln("1A expect ", 43);
  writeln("1A    got ", 43);

  var A: [1..n] int = 1..n;
  var B: [1..n] int;
  noop(B, A);
  increment(B, A); // works as expected
  writeln("1B expect ", + reduce A);
  writeln("1B    got ", + reduce B);
  if verbose then
    writeln(B);

  noop(x, A);
  increment(x, A); // racy!
  writeln("1C expect ", 43 + (+ reduce A));
  writeln("1C    got ", x);
}
test1();

record R {
  var value:int;
}

proc noop(const ref rcv: R, a:R, b:R) {
}

proc addem(ref rcv: R, a:R, b:R) {
  rcv.value = a.value + b.value;
}

proc test2() {
  var A:[1..n] R = [i in 1..n] new R(i);
  var B:[1..n] R = [i in 1..n] new R(i);
  var C:[1..n] R;

  noop(C, A, B); //OK
  addem(C, A, B); //OK
  writeln("2A expect ", 2 * (+ reduce A.value));
  writeln("2A    got ", + reduce C.value);

  var result:R;

  noop(result, A, B);
  addem(result, A, B); // racy!
  writeln("2B expect ", 2 * (+ reduce A.value));
  writeln("2B    got ", result.value);
}
test2();
