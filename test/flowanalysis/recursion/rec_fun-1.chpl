
function f(x) {
 if (x > 9)
   return x;
  return f(x + 1);
}

writeln(f(1));
writeln(f(1.0));
