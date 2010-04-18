def main() {
  var x: int(32);
  coforall i in 1..10 {
    atomic {
      x = 1;
    }

  }
  writeln(x);
}

