use UnrolledLinkedList;
var l: unrolledLinkedList(int, false);
for i in 0..33 {
  l.append(i);
}
writeln(l);
l.insert(1, -1);
writeln(l);

for i in 0..#32 {
  l.append(-1);
}
writeln(l);
l.remove(-1, 1);
writeln(l);
l.remove(-1, 0);
writeln(l);
