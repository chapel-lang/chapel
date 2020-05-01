class C { var x: int = 0; }

proc test(type T) {
  // Trivially evaluate to second branch.
  var a = if isTuple(T) then 128 else (128, new T(256));
  writeln(a);
  return;
}

type T1 = owned C;
type T2 = owned C?;
type T3 = borrowed C?;

proc main() {
  test(T1);
  test(T2);
  test(T3);
  return;
}
