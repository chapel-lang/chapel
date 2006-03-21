class G {
  fun readit(): int {
    return 1;
  }
}

fun main() {
  var goo = G();
  var gooResult = goo.readit();
  writeln("gooResult = ", gooResult);
}
