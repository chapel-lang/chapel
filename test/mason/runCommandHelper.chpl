use Random;

//
// helper for runCommand to just print lots of garbage
//

config const numLines = 1;
config const numPerLine = 1;
config const randomValues = false;

proc main() {
  for 0..#numLines {
    var a: [1..#numPerLine] real;
    if randomValues then
      fillRandom(a);
    else
      forall i in a.domain do
        a[i] = i:real;
    writeln(a);
  }
}
