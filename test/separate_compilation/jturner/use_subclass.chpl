class Parent {
  var i : int;

  proc dothat(j:int) {
    return j + 3;
  }
}

class Child : Parent {
  proc dothat(j:int) {
    return j + 10;
  }
}

proc dothis(p:Parent) : int;

proc main() {
  var p:Parent = new Child();
  p.i = 100;
  writeln(dothis(p));
}
