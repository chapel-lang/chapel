class G {
  def read(): int {
    return 1;
  }
}

def main() {
  var goo = new G();
  var gooResult = goo.read();
  writeln("gooResult = ", gooResult);
}
