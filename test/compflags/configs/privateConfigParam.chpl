module M {
  private config param foo = false;
  proc writeFoo() {
    writeln(foo);
  }
}

module M2 {
  config param foo = false;
  proc main() {
    use M;
    writeFoo();
    writeln(foo);
  }
}
