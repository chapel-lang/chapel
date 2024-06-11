record R {
  type t;
  param rank: int = 2;
}

proc printType(type t) {
  writeln(t:string);
  if t.t == ? then
    writeln(".t is uninstantiated");
  else
    writeln(".t: ", t.t:string);
  if t.rank == ? then
    writeln(".rank is uninstantiated");
  else
    writeln(".rank: ", t.rank:string);
}

printType(R(?));
printType(R());
