proc testit(type t) {
  var r = 1:t..10:t;
  writeln(r.size, ": ", r.size.type:string);
}

for param i in 0..3 {
  testit(int(8*2**i));
  testit(uint(8*2**i));
}
