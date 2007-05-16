def main() {
  var A: [i in 1..2**10] int = i;

  var x, y, z: int;

  cobegin {
    x = + reduce A;
    y = min reduce A;
    z = max reduce A;
  }

  writeln((x, y, z));
}
