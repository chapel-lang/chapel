class Base {
  var s = "Base";
  def get_s():string {
    return s;
  }
}

class Sub: Base {
  var s = "Sub";
}

def main() {
  var sub = Sub();
  var base = sub;
  var base2= Base();
  writeln(sub.get_s());
  writeln(base.get_s());
  writeln(base2.get_s());
}
