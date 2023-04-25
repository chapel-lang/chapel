module M {
  proc someProc() {
    return 0;
  }
}

module N {
  use M.someProc;

  proc main() {
    writeln("here");
  }
}
