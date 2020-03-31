

class C {
  var x: int;

  proc matches(other) {
    if this == other then
      writeln("same!");
    else
      writeln("different!");
  }
}

proc main() {
  var a: [1..3] owned C?;
  for i in 1..3 do
    a[i] = new owned C(i);
  
  writeln(a);
}
