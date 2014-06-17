class Base {
  var s = "Base";
  proc get_s():string {
    return s;
  }
}

class Sub: Base {
  var s = "Sub";
}

proc main() {
  var sub = new Sub();
  var base = sub;
  var base2= new Base();
  writeln(sub.get_s());
  writeln(base.get_s());
  writeln(base2.get_s());
}
