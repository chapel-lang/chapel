const dom = {1..2, 20..30};
writeln(dom.dim(1));
writeln(dom.dim(2));

for i in dom.dim(1) {
  writeln(i);
}
writeln();
for i in dom.dim(2) {
  writeln(i);
}
