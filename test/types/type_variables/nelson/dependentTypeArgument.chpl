class A {
  def hello() { writeln("hello from A!"); }
}

def bar(type T : A) {
  var t = new T();
  t.hello();
}


def main() {
  bar(A);
}
