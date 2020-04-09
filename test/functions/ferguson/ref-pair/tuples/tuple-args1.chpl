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

proc readsOne( tup )
{
  writeln(tup(0));
}
proc readsComputed( tup )
{
  writeln(tup(n-1));
}
proc writesOne( tup )
{
  tup(0) = 1;
}

proc writesOneNested( tup )
{
  tup(0)(1) = 1;
}

writeln("readsOne");
readsOne( (getter(globalArray), getter(globalArray)) );
writeln("readsComputed");
readsComputed( (getter(globalArray), getter(globalArray)) );

writeln("writesOne");
writesOne( (getter(globalArray), getter(globalArray)) );
writeln("readsOne (nested)");
readsOne( ((getter(globalArray), getter(globalArray)),
           (getter(globalArray), getter(globalArray))) );
writeln("readsComputed (nested)");
readsComputed( ((getter(globalArray), getter(globalArray)),
               (getter(globalArray), getter(globalArray))) );


