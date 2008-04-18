
def g(x) {
 if (x > 19) then
   return x;
  return g(x + 1);
}

def f(x) {
  return g(x);
}


writeln(f(1));
writeln(f(1.0));
