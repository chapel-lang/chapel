
/*****
      ex_genproc.chpl -
      Examples of generic procedures.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/


proc castto(type t, val : real) {

  return val : t;
}

writeln("Demo type argument");
writeln("  castto 314.159 to int32  ", castto(int(32), 314.159));
writeln("  castto 314.159 to uint8  ", castto(uint(8), 314.159));

proc filltuple(param cnt: int, x) {
  var result : cnt*x.type;
  for param i in 0..(cnt-1) do result(i) = x;
  return result;
}

writeln("Demo param argument and unspecified argument type");
var ex1 = filltuple(4, 2.178);
writeln("  fill 4-tuple ", ex1);
writeln("  type is ", ex1.type : string);
