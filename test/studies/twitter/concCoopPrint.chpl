config var s = 'Hello World!\n';
var a: atomic(int);
a.write(0);

coforall c in 0..#s.length {
  a.waitFor(c);
  write(s[c]);
  a.add(1);
}
