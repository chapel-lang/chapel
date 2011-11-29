iter foo() {
  yield (1, 0);
  yield (2, 0);
  yield (3, 0);
}

iter bar() {
  yield (0, 1);
  yield (0, 2);
  yield (0, 3);
}

var x = [i in foo()] (i + [j in bar()] j);
writeln(x);
