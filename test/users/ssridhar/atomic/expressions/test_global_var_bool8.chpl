var x: bool(8) = true;
proc main() {
  coforall i in 1..1000 do
    atomic x = x ^ false;
  writeln(x);
}
