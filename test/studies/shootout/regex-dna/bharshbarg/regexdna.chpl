proc main() {
  var variants = [
    "agggtaaa|tttaccct",
    "[cgt]gggtaaa|tttaccc[acg]",
    "a[act]ggtaaa|tttacc[agt]t",
    "ag[act]gtaaa|tttac[agt]ct",
    "agg[act]taaa|ttta[agt]cct",
    "aggg[acg]aaa|ttt[cgt]ccct",
    "agggt[cgt]aa|tt[acg]accct",
    "agggta[cgt]a|t[acg]taccct",
    "agggtaa[cgt]|[acg]ttaccct"
  ];

  var subst = [
    ("B", "(c|g|t)"), ("D", "(a|g|t)"), ("H", "(a|c|t)"), ("K", "(g|t)"),
    ("M", "(a|c)"), ("N", "(a|c|g|t)"), ("R", "(a|g)"), ("S", "(c|g)"),
    ("V", "(a|c|g)"), ("W", "(a|t)"), ("Y", "(c|t)")
  ];

  var total : string;
  stdin.readstring(total);
  const initLen = total.length;

  var noLines = compile(">.*\n|\n");
  total = noLines.sub("", total);
  const noLineLen = total.length;

  var results : [variants.domain] int;

  forall (pattern, result) in zip(variants, results) {
    var re = compile(pattern);
    for match in re.matches(total) do result += 1;
  }

  for (p,r) in zip(variants, results) do writeln(p, " ", r);

  for (f, r) in subst {
    const re = compile(f);
    total = re.sub(r, total);
  }
  writeln("\n", initLen);
  writeln(noLineLen);
  writeln(total.length);
}
