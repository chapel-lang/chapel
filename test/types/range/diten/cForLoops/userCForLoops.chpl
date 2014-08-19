iter ii() {
  var i = 0;
  const start = 0;
  const end = 10;
  const stride = 1;
  while __primitive("C for loop",
                    __primitive("move", i, start),
                    __primitive("<=", i, end),
                    __primitive("move", i, __primitive("+", i, stride))) {
    yield i;
  }
}

proc main {
  var sum = 0;
  for i in ii() {
    sum += i;
  }
  writeln(sum);
}
