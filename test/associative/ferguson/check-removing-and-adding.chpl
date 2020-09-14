config const n=100;
config const m=5;

proc main() {
  var D:domain(int);

  for i in 1..n {
    for j in 1..m {
      var num = i*j + i + j;
      assert(!D.contains(num));
      D += num;
      assert(D.contains(num));
    }
    for j in 1..m {
      var num = i*j + i + j;
      assert(D.contains(num));
      D -= num;
      assert(!D.contains(num));
      if j < m {
        var num2 = i*(j+1) + i + (j+1);
        assert(D.contains(num2));
        D += num2;
        assert(D.contains(num2));
      }
    }
  }

  writeln("OK");
}
