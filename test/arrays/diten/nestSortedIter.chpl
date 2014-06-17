var D: domain(int);
D += 1; D += 2; D+=4; D += 3;
for a in D.sorted() {
  for b in D.sorted() {
    writeln((a,b));
  }
}
