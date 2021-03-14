config const n = 10;
config const l = n/2;

var A: [1..n] int = 1;
on (Locales(0)) do {
  local {
    A(l) = l;
  }
 }

writeln(A);
