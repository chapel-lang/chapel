module a {
  var x = 2;
  writeln("a");
  module b { var aaa = x; writeln("a.b ", aaa); }
}

module b { writeln("b"); }

def main() {
  use a.b;
  foo();
}

def foo() {
  use b;
}

