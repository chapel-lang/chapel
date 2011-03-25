proc main() {
  var x: uint(64);
  coforall i in 1..1000 {
    atomic {
      x = x + 1;
    }

  }
  writeln(x);
}

