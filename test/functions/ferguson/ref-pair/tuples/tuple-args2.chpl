var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

config const n = 1;

proc writesAll( tup )
{
  tup = (globalArray, globalArray);
}

proc writesComputed( tup )
{
  tup(n) = 1;
}

proc writesComputedNested( tup )
{
  tup(n)(n) = 1;
}


writeln("writesComputed");
writesComputed( (getter(globalArray), getter(globalArray)) );
writeln("writesAll");
writesAll( (getter(globalArray), getter(globalArray)) );

writeln("writesComputed");
writesComputedNested( ((getter(globalArray), getter(globalArray)),
                       (getter(globalArray), getter(globalArray))) );


