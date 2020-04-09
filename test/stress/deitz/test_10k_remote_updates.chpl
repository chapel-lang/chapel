config const n = 10000;

var x = 0;

on Locales(1) {
  for i in 1..n {
    x += 1;
  }
}

writeln(x);
