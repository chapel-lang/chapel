proc f() ref {
  var localArray:[1..100] int;
  return localArray;
}

writeln(f()[1]);

