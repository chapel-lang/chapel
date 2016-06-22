class C {
  type t;
}

proc f(a: 3*C(?)) {
  writeln(a.type:string);
}

f((new C(int), new C(real), new C(complex)));
