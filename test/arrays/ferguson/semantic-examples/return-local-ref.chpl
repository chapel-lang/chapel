use myrecord;
proc f() ref {
  var localArray:[1..2] R;
  return localArray;
}

writeln(f()[1]);

