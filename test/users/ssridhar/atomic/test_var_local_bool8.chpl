def main() {
  var x: bool(8);
  coforall i in 1..10 {
    atomic {
      x = true;
    }

  }
  writeln(x);
}

