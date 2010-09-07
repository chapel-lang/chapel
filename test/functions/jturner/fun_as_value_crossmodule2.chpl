module mod1 {
  use mod2;

  def addme(x:int, y:int) {
    return x+y;
  }

  def dothat() {
    var s = subtractme;
    s = addme;

    return s(1, 2);
  }
}

module mod2 {
  use mod1;

  def subtractme(x:int, y:int) {
    return x-y;
  }

  def main() {
    var a = addme;
    a = subtractme;

    writeln(a(10,5));
    writeln(dothat());
  }
}
