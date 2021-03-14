proc iter() {
  for i in 1..4 {
    yield i;
  }       
}

var f = iter;

for x in f() {
  writeln(x);
}