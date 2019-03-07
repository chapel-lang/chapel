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
}

testit(C);
testit(unmanaged C);
testit(borrowed C);
testit(owned C);
testit(shared C);
