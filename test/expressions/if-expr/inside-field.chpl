class C {}

record R1 {
  type T;
  var data: if isClass(borrowed T) then [1..2] owned T else [1..2] borrowed T;
}

record R2 {
  type T;
  var one: [1..2] if isClass(borrowed T) then owned T else borrowed T;
  var two: [1..2] [1..2] if isClass(borrowed T) then owned T else borrowed T;
  var three: [1..2] [1..2] [1..2] if isClass(borrowed T) then owned T else borrowed T;
}

record R3 {
  type T;
  var multiple: [1..2] if !isClass(borrowed T) then 2*borrowed T
                       else [1..2] if isClass(borrowed T) then owned T else borrowed T;
}

proc test(type T) {
  var x : T;
  writeln(x);
  writeln();
}

proc main() {
  test(R1(int));
  test(R1(C));

  test(R2(int));
  test(R2(C));

  test(R3(int));
  test(R3(C));
}

