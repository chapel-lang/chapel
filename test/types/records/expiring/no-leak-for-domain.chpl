proc main() {
  var sum = 0;
  for n in 1..4 {
    for ij in {1..n, n..n+2} {
      var i = ij(1);
      var j = ij(2);
      sum += 100*i + j;
      if i == 2 then
        break;
    }
  }
  writeln(sum);
}
