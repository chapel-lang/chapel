// This is a study of what our parser does at the moment.

// The .good contains a few lines where "expected" and "actual" are the same
// string. Once #13946 is fixed, these lines should disappear from the output.

class C { }
class D { type t; }
proc tm() type return C;
proc tn(type arg) type return D(arg);
type CN = C?;

param Cm = "C.meth";
proc C.meth param  return Cm;
proc type C.tmeth type  return D(int);
proc type (shared C).tmeth type  return D(real);
class X { var fld; }
proc type C.xmeth type  return X;

param Dtm = "D.tdmeth";
proc type D.tdmeth param  return Dtm;

var xx = new C();
var yy: owned C?;
var dd = new D(int);

proc check(param name, type tp, param answer) {
  if tp:string == answer then /*ok*/;
  else compilerWarning(name, "  expected: ", answer, "  actual: ", tp:string);
}

proc check(param name, const ref arg, param answer) {
  if (arg.type):string == answer then /*ok*/;
  else compilerWarning(name, "  expected: ", answer, "  actual: ", (arg.type):string);
}

proc checkCm(param name, param answer) {
  if answer == Cm then /*ok*/;
  else compilerWarning(name, "  expected: ", Cm, "  actual: ", answer);
}

proc checkDtm(param name, param answer) {
  if answer == Dtm then /*ok*/;
  else compilerWarning(name, "  expected: ", Dtm, "  actual: ", answer);
}

proc main {
  var z01a = new C()?!;
  check("z01a = new C()?!", z01a, "borrowed C");

  var z01b = new owned C()?!;
  check("z01b = new owned C()?!", z01b, "borrowed C");

  var z01c = new unmanaged C()?!;
  check("z01c = new unmanaged C()?!", z01c, "unmanaged C");

  var z02 = new C?()!;
  check("z02 = new C?()!", z02, "borrowed C");

  // var z = new C()!?;  -- error "? on a value"
  // var z = new C()??; -- syntax error

  var z03 = new C?()?;
  check("z03 = new C?()?", z03, "owned C?");

  var z04 = new (owned tm())();
  check("z04 = new (owned tm())()", z04, "owned C");

  var z05 = new owned (tm())();
  check("z05 = new owned (tm())()", z05, "owned C");

  var z06 = new (owned tm())()?;
  check("z06 = new (owned tm())()?", z06, "owned C?");

  var z11 = new owned (tm())()?;
  check("z11 = new owned (tm())()?", z11, "owned C?");

  var z07 = new owned (tm())()?!;
  check("z07 = new owned (tm())()?!", z07, "borrowed C");

  var z08 = new (owned tm())?();
  check("z08 = new (owned tm())?()", z08, "owned C?");

  var z12 = new (tm())?();
  check("z12 = new (tm())?()", z08, "owned C?");

  var z09 = new owned (tm()?)()!;
  check("z09 = new owned (tm()?)()!", z09, "borrowed C");

  var z10 = new owned (tm()?)()?;
  check("z10 = new owned (tm()?)()?", z10, "owned C?");

  var z13a = (xx:CN)!;
  check("z13a = (xx:CN)!", z13a, "borrowed C");

  var z13b = xx:(CN!);
  check("z13b = xx:(CN!)", z13b, "borrowed C");

  var z13c = xx:CN!;
  check("z13c = xx:CN!", z13c, "borrowed C");
  // 'bang-vs-cast.chpl' helps see whether this does z13a or z13b

  var z14c = xx:tm()!;
  check("z14c = xx:tm()!", z14c, "borrowed C");

  var z15a  = xx: unmanaged C;
  check("z15a  = xx: unmanaged C", z15a, "unmanaged C");

  var z15b = xx: unmanaged C?;
  check("z15b = xx: unmanaged C?", z15b, "unmanaged C?");

  // var z15c = xx: unmanaged C!; // parsed 'unmanaged (C!)' -- illegal
  // check("z15c = xx: unmanaged C!", z15c, "unmanaged C");

  var z16a = xx!: unmanaged C;
  check("z16a = xx!: unmanaged C", z16a, "unmanaged C");

  var z16b = xx!: unmanaged C?;
  check("z16b = xx!: unmanaged C?", z16b, "unmanaged C?");

  // var z16c = xx!: unmanaged C!; // like z15c
  // check("z16c = xx!: unmanaged C!", z16c, "unmanaged C");

  var z17a = xx : class? : unmanaged class; // cast binds most loosely, left associative
  check("z17a = xx : class?: unmanaged class", z17a, "unmanaged C");

  var z17b = xx : class? : unmanaged C; // same as z17a
  check("z17b = xx : class? : unmanaged C", z17b, "unmanaged C");

  // z18a,b are not interesting, just making sure the compiler accepts them
  var z18a = xx : class? : unmanaged class?;
  check("z18a = xx : class : unmanaged class?", z18a, "unmanaged C?");

  var z18b = xx : class? : unmanaged C?;
  check("z18b = xx : class? : unmanaged C?", z18b, "unmanaged C?");

  // var z = new C() : unmanaged(3,5); -- errors all around

  var z19a = new C() : unmanaged;
  check("z19a = newC() : unmanaged", z19a, "unmanaged C");

  var z19b = new C()? : unmanaged;
  check("z19b = new C()? : unmanaged", z19b, "unmanaged C?");

  var z19c = new C?() : unmanaged;
  check("z19c = new C?() :unmanaged", z19c, "unmanaged C?");

  // var z = new C() : unmanaged?; -- error: cannot make _uninstantiated into a unmanaged class

  var z19d = new C(): unmanaged class?;
  check("z19d = new C(): unmanaged class?", z19d, "unmanaged C?");

  var z19e = new borrowed C() : unmanaged class?;
  check("z19e = new borrowed C() :unmanaged class?", z19e, "unmanaged C?");

  // '.' binds more tightly than 'new'
  // param z = new C().meth;  -- error: C.meth is not a type method

  // treated as new (C().tmeth)()
  var z20a = new C().tmeth();
  check("z20a = new C().m", z20a, "owned D(int(64))");

  param z20b = new C()!.meth;  // '.' is applied last
  checkCm("z20b = new C()!.meth", z20b);

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

  type z22a = (unmanaged C).tmeth;
  check("z22a = (unmanaged C).tmeth", z22a, "D(int(64))");

  type z22b = (shared C).tmeth;
  check("z22b = (shared C).tmeth", z22b, "D(real(64))");

  type z22c = shared C.tmeth;  // '.' binds more tightly than 'shared'
  check("z22c = shared C.tmeth", z22c, "shared D(int(64))");

  param z23a = D(int).tdmeth;
  checkDtm("z23a = D(int).tdmeth", z23a);

  param z23b = D(int)?.tdmeth;
  checkDtm("z23b = D(int)?.tdmeth", z23b);

  param z23c = D?(int).tdmeth;
  checkDtm("z23c = D?(int).tdmeth", z23c);

  type z24 = tm().tmeth?;
  check("z24 = tm().tmeth?", z24, "D(int(64))?");

  type z25a = shared D(int)?;
  check("z25a = shared D(int)?", z25a, "shared D(int(64))?");

  type z25b = unmanaged D?(string);
  check("z25b = unmanaged D?(string)", z25b, "unmanaged D(string)?");

  compilerError("done", 0);
}
