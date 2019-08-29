class C {
}

proc testit(type t) {
  writeln(t:string);

  writeln(isClass(t));
  writeln(isClassType(t));
  //  writeln(isClassValue(t));

  var c: t = new t();
  writeln(isClass(c));
  //  writeln(isClassType(c));
  writeln(isClassValue(c));

  writeln();
  if (t == unmanaged C) then
    delete c;
}

{
  writeln("undecorated C");
  // These would require generic actuals
  //writeln(C:string);
  //writeln(isClass(C));
  //writeln(isClassType(C));
  var c: C = new C();
  writeln(isClass(c));
  writeln(isClassValue(c));
  writeln();
}

testit(unmanaged C);
testit(borrowed C);
testit(owned C);
testit(shared C);
