// from #9982

proc main() {
  const x = 12;
  (x,) = (x + 1,); // x += 1;
  writeln(x);
}
