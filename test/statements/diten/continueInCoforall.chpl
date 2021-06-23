var s: sync int = 0;
coforall i in 1..4 {
  for j in 1..10 {
    if j == 5 then
    continue;
    s.writeEF(s.readFE() + 1);
  }
}
writeln(s.readFF());
