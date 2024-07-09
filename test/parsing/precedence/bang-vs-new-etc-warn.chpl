// Contains parts of bang-vs-new-etc that generate a warning

class C { }
class D { type t; }

proc type C.tmeth type do  return D(int);
class X { var fld; }
proc type C.xmeth type do  return X;

proc check(param name, const ref arg, param answer) {
  if (arg.type):string == answer then /*ok*/;
  else compilerWarning(name, "  expected: ", answer, "  actual: ", (arg.type):string);
}

proc main {
  // treated as new (C().tmeth)()
  var z20a = new C().tmeth();
  check("z20a = new C().m", z20a, "owned D(int(64))");

  var z20c = new C()?.tmeth();
  check("z20c = new C()?.tmeth", z20c, "owned D(int(64))");

  var z20d = new unmanaged (C()?.tmeth)();  // like z20a
  check("z20d = new unmanaged C()?.tmeth", z20d, "unmanaged D(int(64))");

  // same as z20 series, with parens at end
  var z21a = new C().xmeth(5.5);
  check("z21a = new C().xmeth(5.5)", z21a, "owned X(real(64))");

  var z21c = new C()?.xmeth(5.5);
  check("z21c = new C()?.xmeth(5.5)", z21c, "owned X(real(64))");

  var z21d = new unmanaged C()?.xmeth(5.5);  // like z20a
  check("z21d = new unmanaged C()?.xmeth(5.5)", z21d, "unmanaged X(real(64))");

  compilerError("done", 0);
}
