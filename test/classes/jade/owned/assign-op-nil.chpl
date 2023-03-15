class A {
  var x: int;
}

{
  var owned_nonil: owned A = new A(8);
  owned_nonil = nil;
  compilerWarning(owned_nonil.type:string);
  writeln(owned_nonil.x); // error, dead
}

{
  var owned_nil: owned A? = new A(9);
  owned_nil = nil;
  compilerWarning(owned_nil.type:string);
  writeln(owned_nil!.x); // error, dead and nil
}
