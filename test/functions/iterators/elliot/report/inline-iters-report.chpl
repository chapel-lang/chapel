iter oneYieldIter() {
  for i in 1..10 do yield i;
}

iter twoYieldIter() {
  for i in  1..10 do yield i;
  for i in 11..10 do yield i;
}

iter tenYieldIter() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
  yield 5;
  yield 6;
  yield 7;
  yield 8;
  yield 9;
  yield 10;
}

iter multiTenYieldIter() {
  for i in tenYieldIter() do yield i;
  for i in tenYieldIter() do yield i;
}

for i in oneYieldIter() do writeln(i); writeln();

for i in twoYieldIter() do writeln(i); writeln();

for i in tenYieldIter() do writeln(i); writeln();

for i in multiTenYieldIter() do writeln(i);
