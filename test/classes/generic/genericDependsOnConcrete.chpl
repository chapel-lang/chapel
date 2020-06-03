
class C {
  type T;
  param x : int;

  var next : unmanaged C(T, x);

  param flag : bool = if next.type.T == int then true else false;
}

proc main() {
  var x : C(int, 5) = new C(int, 5);
  writeln(x);
}
