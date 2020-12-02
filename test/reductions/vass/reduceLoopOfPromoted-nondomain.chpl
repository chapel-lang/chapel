
iter myiter() {
  yield 1;
  yield 2;
}

var z = + reduce ([i in 1..0] [j in myiter()] 1);
writeln(z);
