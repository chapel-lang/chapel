config const n = 10000;

var x = 0;

for i in 1..n {
  on Locales(1) {
    x += 1;
  }
}

writeln(x);
