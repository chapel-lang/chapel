proc my_interior(D, off) {
  return D.interior(off);
}

proc dit(D, lo, hi) {
  writeln("D: ", D);
  for i in lo..hi do
    writeln(my_interior(D, i));
}

config const n = 10:int(8);
{
  const D = {-n/2..n};
  writeln(n.type:string, ":");
  dit(D, -n/2, n/2);
}


config const un = 10:uint(8);
{
  const uD = {-un/2..un};
  writeln(un.type:string, ":");
  dit(uD, -n/2, n/2);
}
