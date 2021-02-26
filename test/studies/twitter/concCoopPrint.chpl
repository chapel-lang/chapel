config var s = 'Hello World!\n';
var a: atomic(int);
a.write(0);

coforall c in 0..<s.size {
  a.waitFor(c);
  write(s[c]);
  a.add(1);
}
