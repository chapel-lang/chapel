

record X {
  param s;
}

record Y {
  param s;
  param i : int;
}

proc main() {
  const x : X("foo");
  writeln(typeToString(x.type));

  const y : X("this 'is' /a/ test!!!01234567890123456789012345678901234567890123456789");
  writeln(typeToString(y.type));

  const z : X("this 'is' /a/ test!!!01234567890123456789777777777777777777890123456789");
  writeln(typeToString(z.type));

  const a : Y("0123456789012345678901234567890..............",0);
  writeln(typeToString(a.type));

  const b : Y("0123456789012345678901234567890..............",1);
  writeln(typeToString(b.type));

  const d : Y("01234567890123",0);
  writeln(typeToString(d.type));

  const e : Y("01234567890123",1);
  writeln(typeToString(e.type));

}
