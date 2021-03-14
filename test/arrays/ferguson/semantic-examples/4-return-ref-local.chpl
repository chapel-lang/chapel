use myrecord;
proc f() {
  var localArray:[1..2] R;
  ref r = localArray;
  return r;
}

writeln(f()[1].x);

// Is f() be a valid function? Or an error?
