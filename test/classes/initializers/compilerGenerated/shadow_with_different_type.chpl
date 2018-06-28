// Copied from test/classes/diten/shadow_with_different_type.chpl
class Base {
  var s = "Base";
}

class Sub: Base {
  var s = 1;
}

proc main() {
  var sub         = new borrowed Sub();
  var base:borrowed Base() = sub;
  var base2       = new borrowed Base();

  base.s = "Base";

  writeln(sub.s);
  writeln(base.s);
  writeln(base2.s);
}
