record R {
  var a: int;
}

proc fac(n: int): R {
  if n == 0 then return new R(1);
  else return new R(n*fac(n-1).a);
}

writeln(fac(5));
