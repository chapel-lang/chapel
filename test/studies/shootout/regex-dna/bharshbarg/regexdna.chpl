/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger
   based on C++ RE2 implementation by Alexey Zolotov
*/

proc main(args: [] string) {
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

  var data, copy : string;
  stdin.readstring(data); // reads the entire file in
  const initLen = data.length;

  // remove newlines
  data = compile(">.*\n|\n").sub("", data);
  const noLineLen = data.length;

  copy = data; // grab a copy so we can replace in parallel

  var results : [variants.domain] int;

  // waits for tasks to finish
  sync {
    // fire off a thread to do replacing
    begin with (ref copy) {
      for (f, r) in subst {
        const re = compile(f);
        copy = re.sub(r, copy);
      }
    }


    // count patterns
    forall (pattern, result) in zip(variants, results) do
      for m in compile(pattern).matches(data) do result += 1;
  }

  // print results
  for (p,r) in zip(variants, results) do writeln(p, " ", r);

  writeln("\n", initLen);
  writeln(noLineLen);
  writeln(copy.length);
}
