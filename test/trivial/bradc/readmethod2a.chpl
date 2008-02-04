class G {
  def readit(): int {
    return 1;
  }
}

def main() {
  var goo = new G();
  var gooResult = goo.readit();
  writeln("gooResult = ", gooResult);
}
