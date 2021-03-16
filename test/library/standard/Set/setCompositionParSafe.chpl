use Set;

config const lo = 1;
config const hi = 8;

proc addToSets(a, b) {
  for i in lo..hi {
    ref addTo = if !(i % 2) then a else b;
    addTo.add(i);
  }
}

proc display(op, s) {
  param space = ' ';
  write(op, space, ':', space);
  for i in lo..hi do
    if s.contains(i) then
      write(i, space);
  writeln();
}

proc test1() {
  var a, b, c: set(int, parSafe=true);
  addToSets(a, b);
  c = a | b;
  display('|', c);
}
test1();

proc test2() {
  var a, b, c: set(int, parSafe=true);
  addToSets(a, b);
  c = a + b;
  display('+', c);
}
test2();

proc test3() {
  var a, b, c: set(int, parSafe=true);
  addToSets(a, b);
  c = a - b;
  display('-', c);
}
test3();

proc test4() {
  var a, b, c: set(int, parSafe=true);
  addToSets(a, b);
  c = a & b;
  display('&', c);
}
test4();

proc test5() {
  var a, b, c: set(int, parSafe=true);
  addToSets(a, b);
  c = a ^ b;
  display('^', c);
}
test5();

proc test6() {
  var a, b: set(int, parSafe=true);
  addToSets(a, b);
  a |= b;
  display('|=', a);
}
test6();

proc test7() {
  var a, b: set(int, parSafe=true);
  addToSets(a, b);
  a += b;
  display('+=', a);
}
test7();

proc test8() {
  var a, b: set(int, parSafe=true);
  addToSets(a, b);
  a -= b;
  display('-=', a);
}
test8();

proc test9() {
  var a, b: set(int, parSafe=true);
  addToSets(a, b);
  a &= b;
  display('&=', a);
}
test9();

proc test10() {
  var a, b: set(int, parSafe=true);
  addToSets(a, b);
  a ^= b;
  display('^=', a);
}
test10();

