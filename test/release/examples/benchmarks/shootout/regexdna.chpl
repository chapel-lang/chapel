/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Ben Harshbarger
   derived from the GNU C++ RE2 version by Alexey Zolotov
*/
use IO;

use Regexp;

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

  var data: string;
  stdin.readstring(data); // read in the entire file
  const initLen = data.size;

  // remove newlines
  data = compile(">.*\n|\n").sub("", data);

  var copy = data; // make a copy so we can perform replacements in parallel

  var results: [variants.domain] int;

  sync {
    // fire off a task to perform replacements
    begin with (ref copy) {
      for (f, r) in subst do
        copy = compile(f).sub(r, copy);
    }

    // count patterns
    forall (pattern, result) in zip(variants, results) do
      for m in compile(pattern).matches(data) do
        result += 1;
  }

  // print results
  for (p,r) in zip(variants, results) do
    writeln(p, " ", r);
  writeln();

  writeln(initLen);
  writeln(data.size);
  writeln(copy.size);
}
