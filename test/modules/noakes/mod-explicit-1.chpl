module OuterModule {
  module M1 {
    record Something {
      var x : int;
      var y : real;
    }
  }

  proc main() {
    var s = new M1.Something();

    showIt(s);
  }


  // This function needs to 'use' M1 to access writeThis etc
  proc showIt(s) {
    use M1;

    writeln('s = ', s);
  }
}
