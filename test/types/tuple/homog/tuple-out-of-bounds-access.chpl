var tup: 23*int;

for i in 0..<tup.size {
  tup[i] = i;
}

proc printelt(i: int) {
  writeln(tup(i));
}

printelt(0);
printelt(22);
printelt(23);
