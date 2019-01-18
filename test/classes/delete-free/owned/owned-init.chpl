

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
  const a = [i in 1..3] new owned C(i);
  
  writeln(a);
}
