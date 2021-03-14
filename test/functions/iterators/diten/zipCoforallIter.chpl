iter f(n) {
  coforall i in 1..n {
    yield i;
  }
}

var x: sync int = 0;
for (i, j) in zip(f(4), f(4)) {
  x.writeEF(x.readFE() + i + j);
}

writeln(x.readFF());
