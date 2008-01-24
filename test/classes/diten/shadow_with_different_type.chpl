class Base {
  var s = "Base";
}

class Sub: Base {
  var s = 1;
}

def main() {
  var sub = Sub();
  var base:Base() = sub;
  var base2 = Base();
  base.s = "Base";
  writeln(sub.s);
  writeln(base.s);
  writeln(base2.s);
}
