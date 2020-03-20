module M1 {
  public import M1.Helper;
  module Helper {
    config const debug = false;
  }
}

module M2 {
  public import M2.Helper;
  public module Helper {
    config const debug = false;
  }

  proc main() {
    use M1;
    writeln((M1.Helper.debug, M2.Helper.debug));
  }
}
