
class C {
  param i : uint;
  def initialize() { 
    writeln("hello from id ", i, "!"); 
  }
}

def f(param i : uint) {
  writeln("hello from id ", i, "!");
}

def main() {
  var a = new C(0 : uint); // this works
  var b = new C(1:uint);        // this doesn't
  f(2); // this also works
}