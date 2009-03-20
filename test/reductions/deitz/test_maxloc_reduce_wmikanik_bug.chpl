def main() {
  var k = 2;
  writeln(maxloc reduce ([i in 1..2] i + k, 1..2));
}
