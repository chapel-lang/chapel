proc main() {
  var A: [1..2**10] int = [i in 1..2**10] i;

  var x, y, z: int;

  cobegin ref(x,y,z) {
    x = + reduce A;
    y = min reduce A;
    z = max reduce A;
  }

  writeln((x, y, z));
}
