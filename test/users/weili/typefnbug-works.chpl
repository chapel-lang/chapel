proc fun(type x) type {
  if (x==bool) then return int;
  else if (x==int) then return real(32);
  else return real(64);
}
writeln(fun(int):string);
