
proc test() {
  var i:int;

  for i in 1..10 {
      defer { writeln("defer writeln i ", i); }
      writeln(i);
  }
}

test();
