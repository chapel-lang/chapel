proc printparam(param name, param p) {
  writeln(name, ": ", p.type:string, " = ", p);
}

proc main() {
  param a;
  a = 23;
  printparam("a", a);

  param b:int(8);
  b = 99;
  printparam("b", b);

  param c;
  {
    c = 172;
  }
  printparam("c", c);

  param d:int(32);
  {
    d = 256:int(16);
  }
  printparam("d", d);
}
