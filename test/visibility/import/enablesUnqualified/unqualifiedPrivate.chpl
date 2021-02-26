module M {
  private var x: bool;
}
module User {
  import M.x;

  proc main() {
    writeln(x);
  }
}
