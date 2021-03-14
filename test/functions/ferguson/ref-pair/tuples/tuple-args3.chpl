var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc writesOneNested( tup )
{
  tup(1)(1) = 1;
}

writeln("writesOneNested");
writesOneNested( ((getter(globalArray), getter(globalArray)),
                  (getter(globalArray), getter(globalArray))) );

