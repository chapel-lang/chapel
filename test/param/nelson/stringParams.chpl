

record X {
  param s;
}

record Y {
  param s;
  param i : int;
}

proc main() {
  const x : X("foo");
  writeln(x.type:string);

  const y : X("this 'is' /a/ test!!!01234567890123456789012345678901234567890123456789");
  writeln(y.type:string);

  const z : X("this 'is' /a/ test!!!01234567890123456789777777777777777777890123456789");
  writeln(z.type:string);

  const a : Y("0123456789012345678901234567890..............",0);
  writeln(a.type:string);

  const b : Y("0123456789012345678901234567890..............",1);
  writeln(b.type:string);

  const d : Y("01234567890123",0);
  writeln(d.type:string);

  const e : Y("01234567890123",1);
  writeln(e.type:string);

}
