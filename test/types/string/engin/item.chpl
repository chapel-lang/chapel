var s = "my string";

for i in 0..#s.length {
  write(s.item(i));
}
writeln();

for c in s.items() {
  write(c);
}
writeln();


