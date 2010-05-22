class C {
  type t;
}

def f(a: 3*C(?)) {
  writeln(typeToString(a.type));
}

f((new C(int), new C(real), new C(complex)));
