use Vector;
var v = new vector(int);
for i in 0 .. 4 {
  v.append(i);
  writeln(v);
  assert(v.size == i+1);
}
