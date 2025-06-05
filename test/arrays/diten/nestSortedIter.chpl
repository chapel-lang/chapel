import Sort;
var D: domain(int);
D += 1; D += 2; D+=4; D += 3;
for a in Sort.sorted(D) {
  for b in Sort.sorted(D) {
    writeln((a,b));
  }
}
