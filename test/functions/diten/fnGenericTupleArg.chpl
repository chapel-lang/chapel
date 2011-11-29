class C {
  type t;
}

proc f(a: 3*C(?)) {
  writeln(typeToString(a.type));
}

f((new C(int), new C(real), new C(complex)));
