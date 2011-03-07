iter basic() : int {
  var i : int;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}

writeln( basic());
