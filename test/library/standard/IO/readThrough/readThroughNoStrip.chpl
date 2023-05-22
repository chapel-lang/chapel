use IO, List;

// compare lists of strings
writeln(
  geneList(openReader("./genes.txt"), "ATG") ==
  geneListViaSplit(openReader("./genes.txt"), "ATG")
);

// compare lists of bytes
writeln(
  geneList(openReader("./genes.txt"), b"ATG") ==
  geneListViaSplit(openReader("./genes.txt"), b"ATG")
);

// collect a list of genes via readThrough
proc geneList(r, stopCodon: ?t) {
  var s : t,
      l = new list(t);
  while r.readThrough(stopCodon, s) {
    const gene = s.strip();
    if !gene.isEmpty() {
      assert(gene.endsWith(stopCodon));
      l.pushBack(gene);
    }
  }
  return l;
}

// do the same using readAll and string.split
proc geneListViaSplit(r, stopCodon: ?t) {
  var l = new list(t);
  for gene in r.readAll(t).split(stopCodon) do
    if !gene.strip().isEmpty() then
      l.pushBack(gene + stopCodon);
  return l;
}
