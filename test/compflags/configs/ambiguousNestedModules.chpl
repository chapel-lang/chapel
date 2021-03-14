module M1 {
  module Helper {
    config param debug = false;
  }
}

module M2 {
  module Helper {
    config param debug = false;
  }

  proc main() {
    use M1;
    use M1.Helper;
    writeln((M1.Helper.debug, M2.Helper.debug));
  }
}
