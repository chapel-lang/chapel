module M1 {

  module M2 {
    def main() {
      foo();
    }
  }

  def foo() {
    M2();
  }
}

def M2() {
  writeln("this should be shadowed");
}
