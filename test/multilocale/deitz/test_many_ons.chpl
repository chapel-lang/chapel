config const n = 100;
config const k = 10;

var x = 0;

for i in 1..n {
  for j in 1..k {
    on Locales(1) {
      x += 1;
    }
  }
  _debugWrite(x, " ");
}
