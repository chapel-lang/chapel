
function g(x) {
 if (x > 19)
   return x;
  return g(x + 1);
}

function f(x) {
 if (x > 9)
   return g(x);
  return f(x + 1);
}


writeln(f(1));
writeln(f(1.0));
