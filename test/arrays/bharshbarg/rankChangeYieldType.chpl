use BlockDist;

proc main() {
  var D = {1..4, 1..4};
  var Space = D dmapped Block(D);

  forall i in Space[1, ..] {
    writeln(i.type:string);
  }
}
