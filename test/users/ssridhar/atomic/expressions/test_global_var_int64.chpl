var x: int(64);
proc main() {
  coforall i in 1..1000 do
    atomic x = x + 1;
  writeln(x);
}

