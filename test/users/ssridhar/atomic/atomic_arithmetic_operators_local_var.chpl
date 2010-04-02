def main() {
  var x: uint(64);
  coforall i in 1..10 do
    atomic {
      x = x + 1;
      x = x - 1;
      x = x * 1;
      x = x / 1;
      x += 1;
      x -= 1;
    }
  writeln(x);
}

