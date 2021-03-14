proc printtype(param name, type t) {
  writeln(name, " = ", t:string);
}

config const option = true;

proc main() {
  type a;
  a = int;
  printtype("a", a);

  type b;
  {
    b = complex(64);
  }
  printtype("b", b);

  type c;
  if option { // you can have any color as long as it's black
    c = real;
  } else {
    c = real;
  }
  printtype("c", c);
}
