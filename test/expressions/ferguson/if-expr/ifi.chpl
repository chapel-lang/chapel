config const switch = true;
proc test() {
  var x = 1;
  var y = 2;
  (if switch then x else y) = 3;
  writeln(x);
  writeln(y);
}
test();
