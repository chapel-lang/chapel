
proc g(x) {
 if (x > 19) then
   return x;
  return g(x + 1);
}

proc f(x) {
  return g(x);
}


writeln(f(1));
writeln(f(1.0));
