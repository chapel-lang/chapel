proc makeTuple() {
  var s = "hello"*2;
  return (0,s);
}

iter myiter(offset=makeTuple()) {
  for i in 1..10 {
    yield offset;
  }
}

proc test() {

  writeln(makeTuple());

  for x in myiter() {
    writeln(x);
  }
}

test();
