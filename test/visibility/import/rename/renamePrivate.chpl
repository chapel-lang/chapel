module M {
  private var bar: bool;
}
module User {
  import M.{bar as foo}; // Should fail, bar is private

  proc main() {
    writeln(foo);
  }
}
