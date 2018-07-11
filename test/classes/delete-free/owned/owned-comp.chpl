use OwnedObject;

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
  var a = new Owned(new C(1));
  var b = new Owned(new C(2));

  a.matches(b);
  a.matches(a);
}
