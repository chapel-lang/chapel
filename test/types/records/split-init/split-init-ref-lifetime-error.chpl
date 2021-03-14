proc test() {

  ref x;
  {
    var innerInt = 4;
    x = innerInt;
  }

  writeln(x);
}
test();
