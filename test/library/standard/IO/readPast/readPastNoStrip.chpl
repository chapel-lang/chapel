use IO, List;

// string
writeln(
  geneList(openreader("./genes.txt"), "ATG") ==
  geneListViaSplit(openreader("./genes.txt"), "ATG")
);

// bytes
writeln(
  geneList(openreader("./genes.txt"), b"ATG") ==
  geneListViaSplit(openreader("./genes.txt"), b"ATG")
);

// collect a list of genes via readPast
proc geneList(r, stopCodon: ?t) {
  var s : t,
      l = new list(t);
  while r.readPast(s, stopCodon) {
    const gene = s.strip();
    if !gene.isEmpty() {
      assert(gene.endsWith(stopCodon));
      l.append(gene);
    }
  }
  return l;
}

// do the same using readAll and string.split
proc geneListViaSplit(r, stopCodon: ?t) {
  var l = new list(t);
  for gene in r.readAll(t).split(stopCodon) do
    if !gene.strip().isEmpty() then
      l.append(gene + stopCodon);
  return l;
}
