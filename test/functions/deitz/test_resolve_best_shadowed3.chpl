def main() {
  {
    def foo() {
      writeln("bad");
    }
    {
      def foo() {
        writeln("good");
      }
      {
        {
          foo();
        }
      }
    }
  }
}
