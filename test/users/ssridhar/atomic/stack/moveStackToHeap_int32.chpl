proc main() {
  var x: int(32);
  coforall i in 1..1000 {
    atomic {
      x = x + 1;
    }

  }
  writeln(x);
}

