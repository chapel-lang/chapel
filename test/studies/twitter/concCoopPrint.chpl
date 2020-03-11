config var s = 'Hello World!\n';
var a: atomic(int);
a.write(1);

coforall c in 1..s.size {
  a.waitFor(c);
  write(s[c]);
  a.add(1);
}
