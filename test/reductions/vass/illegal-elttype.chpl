
record RRR { }

var AAA: [1..100] RRR;

proc main {
  var result = + reduce AAA;
  writeln(result);
}
