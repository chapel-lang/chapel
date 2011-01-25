proc main() {
  {
    proc foo() {
      writeln("bad");
    }
    {
      proc foo() {
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
