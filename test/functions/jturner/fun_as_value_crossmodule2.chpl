module mod1 {
  use mod2;

  proc addme(x:int, y:int) {
    return x+y;
  }

  proc dothat() {
    var s = subtractme;
    s = addme;

    return s(1, 2);
  }
}

module mod2 {
  use mod1;

  proc subtractme(x:int, y:int) {
    return x-y;
  }

  proc main() {
    var a = addme;
    a = subtractme;

    writeln(a(10,5));
    writeln(dothat());
  }
}
