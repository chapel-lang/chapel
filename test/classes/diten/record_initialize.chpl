record BBB {
  var a,b,c: int;
  def initialize() {
    writeln("BBB.initialize()");
  }
}
record AAA {
  var b: BBB;
  def initialize() {
    writeln("AAA.initialize()");
  }
}

def main() {
  var a: AAA;
  writeln(a);
}


