proc main() {
  var x: uint(64);
  coforall i in 1..10 {
    atomic {
      x = x + 1;
    }

  }
  writeln(x);
}

