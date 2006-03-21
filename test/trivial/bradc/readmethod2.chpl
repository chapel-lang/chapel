class G {
  fun read(): int {
    return 1;
  }
}

fun main() {
  var goo = G();
  var gooResult = goo.read();
  writeln("gooResult = ", gooResult);
}
