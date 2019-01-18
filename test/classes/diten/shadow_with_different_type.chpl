class Base {
  var s = "Base";
}

class Sub: Base {
  var s = 1;
}

proc main() {
  var sub         = new unmanaged Sub();
  var base:unmanaged Base() = sub;
  var base2       = new unmanaged Base();

  base.s = "Base";

  writeln(sub.s);
  writeln(base.s);
  writeln(base2.s);

  delete base2;
  delete sub;
}
