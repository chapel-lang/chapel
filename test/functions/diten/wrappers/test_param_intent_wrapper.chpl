proc f(param a: int, param b: int) param {
  return a+b;
}

param x = f(b=2, a=1);
writeln(x);
