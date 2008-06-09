def iter(i) {
  var temp = i;
  while temp > 0 {
    writeln(temp);
    yield temp;
    temp -= 1;
  }
}

def main {
  writeln("Capturing the result of iter(5)");
  var array = iter(5);
  writeln("Using the result of iter(5) in an empty loop");
  for x in iter(5) { }
  writeln("Not capturing the result of iter(5)");
  iter(5);
}
