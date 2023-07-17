class A {
  var x: int;
}

{
  var s: shared A = new shared A(8);
  s = nil;
  compilerWarning(s.type:string);
  writeln(s.x); // error, dead
}

{
  var s: shared A? = new shared A(9);
  s = nil;
  compilerWarning(s.type:string);
  writeln(s!.x); // error, dead and nil
}

{
  var s1: shared A = new shared A(10);
  var s2 = s1;
  s1 = nil;
  writeln(s1.x); // error
  writeln(s2.x); // not an error
}
