use CPtr;
proc main() {
  var x: c_array(int, 4);

  writeln(x);

  for i in 0..3 do
    x[i] = i;

  var elt = x[1];

  writeln(elt);

  writeln(x);

  var y = x;
  writeln(y);

  var z: c_array(int, 4);
  z = x;
  writeln(z);
}
