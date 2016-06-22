proc fun(type x) type {
  if (x==bool) then return int;
  if (x==int) then return real(32);
  return real(64);
}

writeln(fun(int):string);
