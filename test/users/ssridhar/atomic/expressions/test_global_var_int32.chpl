var x: int(32);
proc main() {
  coforall i in 1..1000 do
    atomic x = x + 1;
  writeln(x);
}

