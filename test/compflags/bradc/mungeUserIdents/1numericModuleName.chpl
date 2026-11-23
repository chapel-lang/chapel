record Box {
  var value: int;

  proc getValue() {
    return value;
  }
}

proc increment(x: int) {
  return x + 1;
}

var box = new Box(41);
writeln(increment(box.getValue()));
