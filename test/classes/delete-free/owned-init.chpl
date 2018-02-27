use OwnedObject;

class C {
  var x: int;
}

proc main() {
  const a = [i in 1..3] new Owned(new C(i));
  
  writeln(a);
}
