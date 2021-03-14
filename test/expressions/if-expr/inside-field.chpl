class C {}

record R1 {
  type T;
  var data: if isClass(T) then [1..2] owned T? else [1..2] T;
}

record R2 {
  type T;
  var one: [1..2] if isClass(T) then owned T? else T;
  var two: [1..2] [1..2] if isClass(T) then owned T? else T;
  var three: [1..2] [1..2] [1..2] if isClass(T) then owned T? else T;
}

record R3 {
  type T;
  var multiple: [1..2] if !isClass(T) then 2*T
                       else [1..2] if isClass(T) then owned T? else T;
}

proc test(type T) {
  var x : T;
  writeln(x);
  writeln();
}

proc main() {
  test(R1(int));
  test(R1(borrowed C));

  test(R2(int));
  test(R2(borrowed C));

  test(R3(int));
  test(R3(borrowed C));
}
