module M1 {

  var x: int = 1;

  module M2 {

    var x: int = 2;

    module M3 {

      def foo() {
        writeln(x);
      }

    }

  }

}

module M {
  def main() {
    use M1.M2.M3;
    foo();
  }
}
