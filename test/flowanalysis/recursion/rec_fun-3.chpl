
def g(x) {
 if (x > 19) then
   return x;
  return g(x + 1);
}

def f(x) {
 if (x > 9) then
   return g(x);
  return f(x + 1);
}


writeln(f(1));
writeln(f(1.0));
