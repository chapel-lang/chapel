proc my_exterior(D, off) {
  return D.exterior(off);
}

proc dit(D, lo, hi) {
  writeln("D: ", D);
  for i in lo..hi {
    write("D.exterior(", i, ") = ");
    writeln(my_exterior(D, i));
  }
}

config const n = 10:int(8);
config const un = 10:uint(8);
{
  writeln("case 1 ", n.type:string, ":");
  const D = {-n/2..n};
  writeln(n.type:string, ":");
  dit(D, -n/2, n/2);
}


{ // debugging -- same as uint case with int
  writeln("case 2 ", n.type:string, ":");
  const D2 = {n/2..2*n};
  writeln(n.type:string, ":");
  dit(D2, -n/2, n/2);
}

{ // debugging -- same as uint case but with range
  writeln("case 3 ", un.type:string, ":");
  var r = un/2..2*un;
  writeln("r: ", r);
  var lo = -n/2;
  var hi = n/2;
  for i in lo..hi {
    write("r.exterior(", i, ") = ");
    writeln(r.exterior(i));
  }
}

{
  writeln("case 4 ", un.type:string, ":");
  const uD = {un/2..2*un};
  dit(uD, -n/2, n/2);
}
