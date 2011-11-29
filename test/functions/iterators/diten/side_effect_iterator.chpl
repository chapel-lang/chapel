iter itr(i) {
  var temp = i;
  while temp > 0 {
    writeln(temp);
    yield temp;
    temp -= 1;
  }
}

proc main {
  writeln("Capturing the result of itr(5)");
  var array = itr(5);
  writeln("Using the result of itr(5) in an empty loop");
  for x in itr(5) { }
  writeln("Not capturing the result of itr(5)");
  itr(5);
}
