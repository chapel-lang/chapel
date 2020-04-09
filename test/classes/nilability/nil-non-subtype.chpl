
class C {
  var x : int;
}

class G { 
  type T;
  var x : T;
}

class D : C {
  var y : real;
}

proc test(type A, type B) {
  const sub = isSubtype(A, B);
  writeln(A:string, " subtype of ", B:string, ": ", sub);
  assert(sub != isSubtype(B, A));
}

proc main() {
  test(C, C?);
  test(G, G?);
  test(G(int), G(int)?);
  test(D, C?);
}
